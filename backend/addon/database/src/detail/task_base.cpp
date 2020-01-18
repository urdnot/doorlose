#include <addon/database/detail/task_base.hpp>
#include <addon/database/detail/utils.hpp>

#include <cassert>

#include <algorithm>
#include <fstream>

namespace addon {
namespace database {
namespace detail {
namespace {

struct task_entry_t
{
    std::uint32_t removed : 1;
    std::uint32_t size : 31;
    std::uint8_t body[];
};

struct serialization_header_t
{
    std::uint64_t max_task_size;
    std::uint64_t task_count;
    std::uint64_t granularity;
    std::uint64_t capacity;
};

} // namespace

task_base::task_base()
{
}

task_base::task_base(const std::uint64_t max_task_size, const std::uint64_t granularity)
{
    //
    // Checks
    //
    check_non_zero(max_task_size, "task_base::ctor(): `task_size` cannot be zero");
    check_non_zero(granularity, "task_base::ctor(): `granularity` cannot be zero");
    check_mult(max_task_size, 4, "task_base::ctor(): `task_size` should be multiple of 4 for alignment");

    //
    // Inits
    //
    max_task_size_ = max_task_size;
    task_entry_size_ = max_task_size + sizeof(task_entry_t);
    granularity_ = granularity;
    capacity_ = granularity;
    task_count_ = 0;

    base_.resize(task_entry_size_ * capacity_);
}

std::uint64_t task_base::max_task_size() const noexcept
{
    return max_task_size_;
}

std::uint64_t task_base::capacity() const noexcept
{
    return capacity_;
}

std::uint64_t task_base::size() const noexcept
{
    return task_count_;
}

std::uint8_t *task_base::get_task_entry(std::uint64_t id)
{
    return base_.data() + task_entry_size_ * id;
}

const std::uint8_t *task_base::get_task_entry(std::uint64_t id) const
{
    return base_.data() + task_entry_size_ * id;
}

void task_base::expand()
{
    base_.resize(base_.size() + granularity_ * task_entry_size_);
    capacity_ += granularity_;
}

std::uint64_t task_base::add(std::string_view task)
{
    check_limit(task.size(), max_task_size_, "task_base::add(): Task size is too big");

    if (task_count_ == capacity_)
    {
        expand();
    }

    const std::uint64_t result_id = task_count_;

    const auto task_entry = reinterpret_cast<task_entry_t *>(get_task_entry(result_id));
    task_entry->removed = 0;
    task_entry->size = task.size();
    std::memcpy(task_entry->body, task.data(), task.size());

    task_count_++;
    return result_id;
}

std::string_view task_base::get(std::uint64_t id) const
{
    check_limit(id, task_count_, "task_base::get(): Task index out of range");

    const auto task_entry = reinterpret_cast<const task_entry_t *>(get_task_entry(id));

    if (task_entry->removed)
    {
        return std::string_view();
    }

    return std::string_view(reinterpret_cast<const char *>(task_entry->body), task_entry->size);
}

void task_base::remove(std::uint64_t id)
{
    check_limit(id, task_count_, "task_base::remove(): Task index out of range");

    const auto task_entry = reinterpret_cast<task_entry_t *>(get_task_entry(id));
    task_entry->removed = 1;
}

void task_base::update(std::uint64_t id, std::string_view task)
{
    check_limit(task.size(), max_task_size_, "task_base::update(): Task size is too big");
    check_limit(id, task_count_, "task_base::update(): Task index out of range");

    const auto task_entry = reinterpret_cast<task_entry_t *>(get_task_entry(id));
    if (task_entry->removed)
    {
        throw invalid_argument("task_base::update(): Attempt to update removed task");
    }

    task_entry->size = task.size();
    std::memcpy(reinterpret_cast<char *>(task_entry->body), task.data(), task.size());
}

void task_base::serialize(const std::filesystem::path &to) const
{
    std::ofstream output(to, std::ios_base::out | std::ios_base::trunc);

    check_ostream(output, "task_base::serialize(): cannot open output file for serialize");

    serialization_header_t header;
    header.max_task_size = max_task_size_;
    header.task_count = task_count_;
    header.granularity = granularity_;
    header.capacity = capacity_;

    output.write(reinterpret_cast<const char *>(&header), sizeof(serialization_header_t));
    check_ostream(output, "task_base::serialize(): cannot serialize header");

    for (std::uint64_t i = 0; i < task_count_; ++i)
    {
        const auto task_entry = reinterpret_cast<const task_entry_t *>(get_task_entry(i));
        const auto usefull_size = sizeof(task_entry_t) + task_entry->size;
        output.write(reinterpret_cast<const char *>(task_entry), usefull_size);
        check_ostream(output, "task_base::serialize(): cannot serialize task");
    }
}

void task_base::deserialize(const std::filesystem::path &from)
{
    if (!std::filesystem::exists(from))
    {
        throw invalid_argument("task_base::deserialize(): file for deserialize do not exist");
    }

    std::ifstream input(from);

    check_istream(input, "task_base::deserialize(): cannot open input file for deserialize");

    serialization_header_t header;
    input.read(reinterpret_cast<char *>(&header), sizeof(serialization_header_t));
    check_istream(input, "task_base::deserialize(): cannot deserialize header");

    //
    // Checks
    //
    check_non_zero(header.max_task_size,
        "task_base::deserialize(): `max_task_size` cannot be zero");
    check_non_zero(header.granularity,
        "task_base::deserialize(): `granularity` cannot be zero");
    check_mult(header.max_task_size, 4,
        "task_base::deserialize(): `max_task_size` should be multiple of 4 for alignment");
    check_mult(header.capacity, header.granularity,
        "task_base::deserialize(): `capacity` should be multiple of `granularity`");

    std::vector<std::uint8_t> base;
    const std::uint64_t task_entry_size = header.max_task_size + sizeof(task_entry_t);
    base.resize(header.capacity * task_entry_size);

    for (std::uint64_t i = 0; i < header.task_count; ++i)
    {
        const auto task_entry =
            reinterpret_cast<task_entry_t *>(base.data() + i * task_entry_size);
        input.read(reinterpret_cast<char *>(task_entry), sizeof(task_entry_t));
        input.read(reinterpret_cast<char *>(task_entry->body), task_entry->size);
        check_istream(input, "task_base::deserialize(): cannot deserialize task");
    }

    max_task_size_ = header.max_task_size;
    task_entry_size_ = task_entry_size;
    granularity_ = header.granularity;
    capacity_ = header.capacity;
    task_count_ = header.task_count;

    base_.swap(base);
}

} // namespace detail
} // namespace database
} // namespace addon