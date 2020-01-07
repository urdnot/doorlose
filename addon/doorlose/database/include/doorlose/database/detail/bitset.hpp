#pragma once

#include <doorlose/database/detail/common.hpp>

#include <cstdint>

namespace doorlose {
namespace database {
namespace detail {

class DOORLOSE_DATABASE_EXPORT bitset
{
public:
    /**
     *
     */
    static std::uint64_t get_random(std::uint8_t *mask, std::uint64_t bit_size) noexcept;

private:
    static std::uint64_t popcnt_64(const std::uint64_t *num) noexcept;
    static std::uint64_t set_zero_bit_64(std::uint64_t *num, std::uint64_t n) noexcept;
};

} // namespace detail
} // namespace database
} // namespace doorlose