#include <doorlose/database/task_base.hpp>

#include <cassert>

#include <algorithm>
#include <stdexcept>

namespace doorlose {
namespace database {
namespace {

struct task_entry_t
{
    std::uint32_t removed : 1;
    std::uint32_t size : 31;
    std::uint8_t body[];
};

void check_positive_4_mult(std::uint64_t value, std::string_view error_msg)
{
    if (value && value % 4)
    {
        throw std::invalid_argument(error_msg.data());
    }
}

void check_limit(std::uint64_t value, std::uint64_t limit,
    std::string_view error_msg)
{
    if (value >= limit)
    {
        throw std::out_of_range(error_msg.data());
    }
}

} // namespace

task_base::task_base()
{
}

task_base::task_base(const std::uint64_t task_size, const std::uint64_t granularity)
{
    //
    // Checks
    //
    check_positive_4_mult(task_size, "Task size should be multiple of 4 and positive");

    //
    // Inits
    //
    max_task_size_ = task_size;
    task_entry_size_ = task_size + sizeof(task_entry_t);
    granularity_ = std::max(granularity, MIN_GRANULARITY);
    base_capacity_ = granularity_;
    task_count_ = 0;

    base_.resize(task_entry_size_ * base_capacity_);
}

std::uint64_t task_base::max_task_size() const noexcept
{
    return max_task_size_;
}

std::uint64_t task_base::capacity() const noexcept
{
    return base_capacity_;
}

std::uint64_t task_base::size() const noexcept
{
    return task_count_;
}

std::uint8_t *task_base::get_task_entry(std::uint64_t id) noexcept
{
    return base_.data() + task_entry_size_ * id;
}

const std::uint8_t *task_base::get_task_entry(std::uint64_t id) const noexcept
{
    return base_.data() + task_entry_size_ * id;
}

void task_base::expand()
{
    base_.resize(base_.size() + granularity_ * task_entry_size_);
    base_capacity_ += granularity_;
}

std::uint64_t task_base::add(std::string_view task)
{
    check_limit(task.size(), max_task_size_, "Task size is too big");

    if (task_count_ == base_capacity_)
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

std::string_view task_base::get(std::uint64_t id) const noexcept
{
    check_limit(id, task_count_, "Task index out of range");

    const auto task_entry = reinterpret_cast<const task_entry_t *>(get_task_entry(id));

    if (task_entry->removed)
    {
        return std::string_view();
    }

    return std::string_view(task_entry->body, task_entry->size);
}

void task_base::remove(std::uint64_t id)
{
}

void task_base::update(std::uint64_t id, std::string_view task)
{
}

void task_base::serialize(const std::filesystem::path &to)
{
}

void task_base::deserialize(const std::filesystem::path &from)
{
}

} // namespace database
} // namespace doorlose