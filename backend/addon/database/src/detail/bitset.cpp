#include <addon/database/detail/bitset.hpp>

#include <cassert>
#include <ctime>

#include <bitset>

namespace addon {
namespace database {
namespace detail {

std::uint64_t bitset::popcnt_64(const std::uint64_t *num) noexcept
{
    auto bs = std::bitset<64>(*num);
    return bs.count();
}

std::uint64_t bitset::set_zero_bit_64(std::uint64_t *num, std::uint64_t n) noexcept
{
    std::uint64_t v = ~(*num);                   // Input value to find position with rank r.
    unsigned int r = (unsigned int)n + 1;        // Input: bit's desired rank [1-64].
    unsigned int s;                              // Output: Resulting position of bit with rank r [1-64]
    std::uint64_t a, b, c, d;                    // Intermediate temporaries for bit count.
    unsigned int t;                              // Bit count temporary.

    a =  v - ((v >> 1) & ~std::uint64_t(0)/3);
    b = (a & ~std::uint64_t(0)/5) + ((a >> 2) & ~std::uint64_t(0)/5);
    c = (b + (b >> 4)) & ~std::uint64_t(0)/0x11;
    d = (c + (c >> 8)) & ~std::uint64_t(0)/0x101;
    t = (d >> 32) + (d >> 48);
    
    s  = 64;
    s -= ((t - r) & 256) >> 3; r -= (t & ((t - r) >> 8));
    t  = (d >> (s - 16)) & 0xff;
    s -= ((t - r) & 256) >> 4; r -= (t & ((t - r) >> 8));
    t  = (c >> (s - 8)) & 0xf;
    s -= ((t - r) & 256) >> 5; r -= (t & ((t - r) >> 8));
    t  = (b >> (s - 4)) & 0x7;
    s -= ((t - r) & 256) >> 6; r -= (t & ((t - r) >> 8));
    t  = (a >> (s - 2)) & 0x3;
    s -= ((t - r) & 256) >> 7; r -= (t & ((t - r) >> 8));
    t  = (v >> (s - 1)) & 0x1;
    s -= ((t - r) & 256) >> 8;
    s = 65 - s;

    *num |= std::uint64_t(1) << (64 - s);

    return std::uint64_t(s - 1);
}

std::uint64_t bitset::get_random(std::uint8_t *mask, std::uint64_t bit_size) noexcept
{
    assert(((std::uint64_t)(mask) % 8) == 0); // check that pointer 8-byte align

    const auto mask_64 = reinterpret_cast<std::uint64_t *>(mask);
    const auto rem_64 = 64 - bit_size % 64;
    const auto size_64 = (bit_size / 64) + (rem_64 ? 1 : 0);

    std::uint64_t all_pop = 0;
    for (std::uint64_t i = 0; i < size_64; ++i)
    {
        all_pop += popcnt_64(mask_64 + i);
    }
    std::uint64_t all_zero = bit_size - all_pop;

    if (!all_zero)
    {
        std::memset(mask, 0, size_64 * 8);
        all_zero = bit_size;
    }

    std::srand((unsigned int)(std::time(nullptr)));
    const std::uint64_t rid = std::rand() % all_zero;
    std::uint64_t tmp = rid;
    std::uint64_t result = 0;

    for (std::uint64_t i = 0; i < size_64; ++i)
    {
        const auto pop = 64 - popcnt_64(mask_64 + i);
        if (tmp < pop)
        {
            result += set_zero_bit_64(mask_64 + i, tmp);
            break;
        }
        else
        {
            tmp -= pop;
            result += 64;
        }
    }

    return result;
}

} // namespace detail
} // namespace database
} // namespace addon