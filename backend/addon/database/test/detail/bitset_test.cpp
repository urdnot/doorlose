#include <addon/database/detail/bitset.hpp>

#include <gtest/gtest.h>

#include <algorithm>
#include <bitset>
#include <vector>

namespace addon {
namespace database {
namespace detail {
namespace {

class bitset_test : public ::testing::Test
{
public:
    bitset_test()
    {
    }
};

TEST_F(bitset_test, basic)
{
    std::vector<std::uint64_t> buf(4);
    std::vector<uint_t> generated;
    const uint_t bit_size = buf.size() * 64 - 13;

    for (uint_t i = 0; i < bit_size; ++i)
    {
        const auto num =
            bitset::get_random(reinterpret_cast<std::uint8_t *>(buf.data()), bit_size);

        const auto begin = generated.begin();
        const auto end = generated.end();
        if (std::find(begin, end, num) != end)
        {
            FAIL();
            break;
        }
        else
        {
            generated.push_back(num);
        }
    }

    EXPECT_EQ(0xFFFFFFFFFFFFFFFF, buf[0]);
    EXPECT_EQ(0xFFFFFFFFFFFFFFFF, buf[1]);
    EXPECT_EQ(0xFFFFFFFFFFFFFFFF, buf[2]);
    EXPECT_EQ(0xFFFFFFFFFFFFE000, buf[3]);

    bitset::get_random(reinterpret_cast<std::uint8_t *>(buf.data()), bit_size);

    const auto result = std::bitset<64>(buf[0]).count() + std::bitset<64>(buf[1]).count() +
        std::bitset<64>(buf[2]).count() + std::bitset<64>(buf[3]).count();
    EXPECT_EQ(1, result);
}

} // namespace
} // namespace detail
} // namespace database
} // namespace addon