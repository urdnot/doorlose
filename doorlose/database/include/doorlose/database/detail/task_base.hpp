#pragma once

#include <doorlose/database/detail/common.hpp>

#include <filesystem>
#include <string_view>
#include <vector>

namespace doorlose {
namespace database {
namespace detail {

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
    explicit task_base(const std::uint64_t task_size, const std::uint64_t granularity);

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
    std::string_view get(std::uint64_t id) const;

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
     * Strong exception guaranty.
     * @throws std::runtime_error in case std::ofstream error
     */
    void serialize(const std::filesystem::path &to) const;

    /**
     * Strong exception guaranty
     * @throws std::runtime_error in case std::ifstream error,
     * @throws std::bad_alloc in case allocation error
     * @throws std::invalid_argument in case invalid task base settings
     */
    void deserialize(const std::filesystem::path &from);

private:
    std::uint8_t *get_task_entry(std::uint64_t id);
    const std::uint8_t *get_task_entry(std::uint64_t id) const;
    void init(const std::uint64_t task_size, const std::uint64_t granularity,
        const std::uint64_t capacity, const std::uint64_t task_count);
    void expand();

private:
    std::uint64_t max_task_size_{};
    std::uint64_t task_entry_size_{};
    std::uint64_t granularity_{};
    std::uint64_t capacity_{};
    std::uint64_t task_count_{};

    std::vector<std::uint8_t> base_;
};

} // namespace detail
} // namespace database
} // namespace doorlose