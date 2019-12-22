#pragma once

#include <doorlose/database/common.hpp>

#include <filesystem>
#include <string_view>
#include <vector>

namespace doorlose {
namespace database {

/**
 *
 */
class DOORLOSE_DATABASE_EXPORT task_base
{
public:
    /**
     *
     */
    explicit task_base();

    /**
     *
     */
    explicit task_base(const std::uint64_t task_size, const std::uint64_t start_task_count);

    /**
     *
     */
    std::uint64_t max_task_size() const noexcept;

    /**
     *
     */
    std::uint64_t capacity() const noexcept;

    /**
     *
     */
    std::uint64_t size() const noexcept;

    /**
     *
     */
    std::string_view get(std::uint64_t id) const noexcept;

    /**
     *
     */
    std::uint64_t add(std::string_view task);

    /**
     *
     */
    void remove(std::uint64_t id);

    /**
     *
     */
    void update(std::uint64_t id, std::string_view task);

    /**
     *
     */
    void serialize(const std::filesystem::path &to);

    /**
     *
     */
    void deserialize(const std::filesystem::path &from);
private:
    std::uint64_t max_task_size_;
    std::uint64_t task_entry_size_;
    std::uint64_t base_capacity_;
    std::uint64_t task_count_;
    std::vector<std::uint8_t> base_;
};

} // namespace database
} // namespace doorlose