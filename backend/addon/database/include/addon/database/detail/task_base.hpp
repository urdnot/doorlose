#pragma once

#include <addon/database/detail/common.hpp>
#include <addon/database/types.hpp>

#include <filesystem>
#include <string_view>
#include <vector>

namespace addon {
namespace database {
namespace detail {

/**
 *
 */
class ADDON_DATABASE_EXPORT task_base
{
public:
    /**
     *
     */
    explicit task_base();

    /**
     *
     */
    explicit task_base(const uint_t task_size, const uint_t granularity);

    /**
     *
     */
    uint_t max_task_size() const noexcept;

    /**
     *
     */
    uint_t capacity() const noexcept;

    /**
     *
     */
    uint_t size() const noexcept;

    /**
     *
     */
    std::pair<std::string_view, bool> get(uint_t id) const;

    /**
     *
     */
    uint_t add(std::string_view task);

    /**
     *
     */
    void update(uint_t id, bool removed);

    /**
     *
     */
    void update(uint_t id, std::string_view task);

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

    /**
     *
     */
    void swap(task_base &tb) noexcept;

private:
    std::uint8_t *get_task_entry(uint_t id);
    const std::uint8_t *get_task_entry(uint_t id) const;
    void expand();

private:
    uint_t max_task_size_{};
    uint_t task_entry_size_{};
    uint_t granularity_{};
    uint_t capacity_{};
    uint_t task_count_{};

    std::vector<std::uint8_t> base_;
};

} // namespace detail
} // namespace database
} // namespace addon