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

struct task_header_t
{
    uint_t max_task_size;
    uint_t task_count;
    uint_t granularity;
    uint_t capacity;
};

} // namespace

task_base::task_base()
{
}

task_base::task_base(const uint_t max_task_size, const uint_t granularity)
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

uint_t task_base::max_task_size() const noexcept
{
    return max_task_size_;
}

uint_t task_base::capacity() const noexcept
{
    return capacity_;
}

uint_t task_base::size() const noexcept
{
    return task_count_;
}

std::uint8_t *task_base::get_task_entry(uint_t id)
{
    return base_.data() + task_entry_size_ * id;
}

const std::uint8_t *task_base::get_task_entry(uint_t id) const
{
    return base_.data() + task_entry_size_ * id;
}

void task_base::expand()
{
    base_.resize(base_.size() + granularity_ * task_entry_size_);
    capacity_ += granularity_;
}

uint_t task_base::add(std::string_view task)
{
    check_limit(task.size(), max_task_size_, "task_base::add(): Task size is too big");

    if (task_count_ == capacity_)
    {
        expand();
    }

    const uint_t result_id = task_count_;

    const auto task_entry = reinterpret_cast<task_entry_t *>(get_task_entry(result_id));
    task_entry->removed = 0;
    task_entry->size = task.size();
    std::memcpy(task_entry->body, task.data(), task.size());

    task_count_++;
    return result_id;
}

std::pair<std::string_view, bool> task_base::get(uint_t id) const
{
    check_limit(id, task_count_, "task_base::get(): Task index out of range");

    const auto task_entry = reinterpret_cast<const task_entry_t *>(get_task_entry(id));
    return std::make_pair(
        std::string_view(reinterpret_cast<const char *>(task_entry->body), task_entry->size),
        task_entry->removed);
}

void task_base::update(uint_t id, bool removed)
{
    check_limit(id, task_count_, "task_base::update(): Task index out of range");
    const auto task_entry = reinterpret_cast<task_entry_t *>(get_task_entry(id));
    task_entry->removed = removed;
}

void task_base::update(uint_t id, std::string_view task)
{
    check_limit(task.size(), max_task_size_, "task_base::update(): Task size is too big");
    check_limit(id, task_count_, "task_base::update(): Task index out of range");

    const auto task_entry = reinterpret_cast<task_entry_t *>(get_task_entry(id));
    task_entry->size = task.size();
    std::memcpy(reinterpret_cast<char *>(task_entry->body), task.data(), task.size());
}

void task_base::serialize(const std::filesystem::path &to) const
{
    std::ofstream output(to,
        std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);

    check_ostream(output, "task_base::serialize(): cannot open output file for serialize");

    task_header_t header;
    header.max_task_size = max_task_size_;
    header.task_count = task_count_;
    header.granularity = granularity_;
    header.capacity = capacity_;

    output.write(reinterpret_cast<const char *>(&header), sizeof(task_header_t));
    check_ostream(output, "task_base::serialize(): cannot serialize header");

    for (uint_t i = 0; i < task_count_; ++i)
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

    std::ifstream input(from, std::ios_base::binary);

    check_istream(input, "task_base::deserialize(): cannot open input file for deserialize");

    task_header_t header;
    input.read(reinterpret_cast<char *>(&header), sizeof(task_header_t));
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
    const uint_t task_entry_size = header.max_task_size + sizeof(task_entry_t);
    base.resize(header.capacity * task_entry_size);

    for (uint_t i = 0; i < header.task_count; ++i)
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

void task_base::swap(task_base &tb) noexcept
{
    std::swap(max_task_size_, tb.max_task_size_);
    std::swap(task_entry_size_, tb.task_entry_size_);
    std::swap(granularity_, tb.granularity_);
    std::swap(capacity_, tb.capacity_);
    std::swap(task_count_, tb.task_count_);

    base_.swap(tb.base_);
}

} // namespace detail
} // namespace database
} // namespace addon