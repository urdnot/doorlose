#include <doorlose/database/task_base.hpp>

#include <cassert>

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

void check_4_mult(std::uint64_t value, std::string_view error_msg)
{
    if (value % 4)
    {
        throw std::invalid_argument(error_msg.data());
    }
}

void check_out_of_range(std::uint64_t value, std::uint64_t limit,
    std::string_view error_msg)
{
    if (value >= limit)
    {
        throw std::out_of_range(error_msg.data());
    }
}

task_entry_t *get_task_entry(const std::vector<std::uint8_t> &base)
{
    return nullptr;
}

} // namespace

task_base::task_base()
    : task_base(0, 0)
{
}

task_base::task_base(const std::uint64_t task_size, const std::uint64_t start_task_count)
    : max_task_size_(task_size)
    , task_entry_size_(task_size + sizeof(task_entry_t))
    , base_capacity_(start_task_count)
    , task_count_(0)
{
    //
    // Checks
    //
    check_4_mult(task_size, "Task size should be multiple of 4");

    //
    // Inits
    //
    max_task_size_ = task_size;
    task_entry_size_ = task_size + sizeof(task_entry_t);
    base_capacity_ = start_task_count;
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

std::string_view task_base::get(std::uint64_t id) const noexcept
{
    return std::string_view();
}

std::uint64_t task_base::add(std::string_view task)
{
    return 0;
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