#include <doorlose/database.hpp>

#include <gtest/gtest.h>

namespace doorlose {
namespace database {
namespace {

class task_base_test : public ::testing::Test
{
};

TEST_F(task_base_test, abc)
{
    EXPECT_EQ(4, 2*2);
}

} // namespace
} // namespace database
} // namespace doorlose