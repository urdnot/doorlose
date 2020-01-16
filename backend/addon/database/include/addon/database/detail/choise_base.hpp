#pragma once

#include <addon/database/detail/common.hpp>

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
    explicit choise_base(const std::uint64_t start_mask_size,
        std::uint64_t mask_granularity, const std::uint64_t record_granularity);
    
    /**
     *
     */
    std::uint64_t size() const noexcept;

    /**
     *
     */
    std::uint64_t capacity() const noexcept;

    /**
     *
     */
    std::uint64_t mask_size() const noexcept;

    /**
     *
     */
    std::uint64_t mask_capacity() const noexcept;

    /**
     * Add client
     */
    std::uint64_t add();

    /**
     * Choose random task
     */
    std::uint64_t choose(std::uint64_t client_id);

    /**
     * Expand choise's masks
     */
    void increase_mask(std::uint64_t delta);

    /**
     *
     */
    void rollback_add() noexcept;

    /**
     *
     */
    void serialize(const std::filesystem::path &to) const;

    /**
     *
     */
    void deserialize(const std::filesystem::path &from);

private:
    std::uint8_t *get_entry(std::uint64_t id) noexcept;
    const std::uint8_t *get_entry(std::uint64_t id) const noexcept;
    void expand(std::uint64_t delta);
    void expand_mask(std::uint64_t delta);

private:
    std::uint64_t mask_size_{};
    std::uint64_t mask_granularity_{};
    std::uint64_t mask_capacity_{};
    std::uint64_t record_size_{};
    std::uint64_t record_granularity_{};
    std::uint64_t record_capacity_{};
    std::vector<std::uint8_t> base_;
};

} // namespace detail
} // namespace database
} // namespace addon