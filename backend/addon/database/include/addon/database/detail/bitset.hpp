#pragma once

#include <addon/database/detail/common.hpp>
#include <addon/database/types.hpp>

#include <cstdint>

namespace addon {
namespace database {
namespace detail {

class ADDON_DATABASE_EXPORT bitset
{
public:
    /**
     *
     */
    static uint_t get_random(std::uint8_t *mask, uint_t bit_size) noexcept;

private:
    static uint_t popcnt_64(const std::uint64_t *num) noexcept;
    static uint_t set_zero_bit_64(std::uint64_t *num, uint_t n) noexcept;
};

} // namespace detail
} // namespace database
} // namespace addon