#pragma once

#include <addon/database/detail/common.hpp>
#include <addon/database/types.hpp>

#include <filesystem>
#include <vector>

namespace addon {
namespace database {
namespace detail {

/**
 *
 */
class ADDON_DATABASE_EXPORT choise_base
{
public:
    /**
     *
     */
    explicit choise_base();

    /**
     *
     */
    explicit choise_base(uint_t mask_granularity,
        const uint_t record_granularity);
    
    /**
     *
     */
    uint_t size() const noexcept;

    /**
     *
     */
    uint_t capacity() const noexcept;

    /**
     *
     */
    uint_t mask_size() const noexcept;

    /**
     *
     */
    uint_t mask_capacity() const noexcept;

    /**
     * Add client
     */
    uint_t add();

    /**
     * Choose random task
     */
    uint_t choose(uint_t client_id);

    /**
     * Expand choise's masks
     */
    void increase_mask(uint_t delta);

    /**
     *
     */
    void rollback_add() noexcept;

    /**
     *
     */
    void rollback_increase_mask() noexcept;

    /**
     *
     */
    void serialize(const std::filesystem::path &to) const;

    /**
     *
     */
    void deserialize(const std::filesystem::path &from);

    /**
     *
     */
    void swap(choise_base &cb) noexcept;

private:
    std::uint8_t *get_entry(uint_t id) noexcept;
    const std::uint8_t *get_entry(uint_t id) const noexcept;
    void expand(uint_t delta);
    void expand_mask(uint_t delta);

private:
    uint_t mask_size_{};
    uint_t prev_mask_size_{};
    uint_t mask_granularity_{};
    uint_t mask_capacity_{};
    uint_t record_size_{};
    uint_t record_granularity_{};
    uint_t record_capacity_{};

    std::vector<std::uint8_t> base_;
};

} // namespace detail
} // namespace database
} // namespace addon