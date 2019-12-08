#include "task_base.hpp"

#include <cassert>

namespace doorlose {
namespace {

struct task_header
{
    bool removed;
    std::uint8_t body[];
};

} // namespace

task_base::task_base(const std::size_t task_size, const std::size_t start_task_count)
    : task_size_(task_size)
    , start_task_count_(start_task_count)
{
    assert(task_size_ > sizeof(task_header));
    base_.resize(task_size_ * start_task_count_);
}

std::string_view task_base::get() const noexcept
{
    return std::string_view();
}

std::size_t task_base::max_task_size() const noexcept
{
    return task_size_ - sizeof(task_header);
}

void task_base::add()
{
}

void task_base::remove()
{
}

void task_base::update()
{
}

void task_base::serialize()
{
}

void task_base::deserialize()
{
}


} // namespace doorlose