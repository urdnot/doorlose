#pragma once

#include <string_view>
#include <vector>

namespace doorlose {

class task_base
{
public:
    /**
     *
     */
    explicit task_base(const std::size_t task_size, const std::size_t start_task_count);

    /**
     *
     */
    std::string_view get() const noexcept;

    /**
     *
     */
    std::size_t max_task_size() const noexcept;

    /**
     *
     */
    void add();

    /**
     *
     */
    void remove();

    /**
     *
     */
    void update();

    /**
        *
        */
    void serialize();

    /**
        *
        */
    void deserialize();

private:
    const std::size_t task_size_;
    const std::size_t start_task_count_;
    std::vector<std::uint8_t> base_;
};

} // namespace doorlose