#include <doorlose/database/database.hpp>

#include <gtest/gtest.h>

namespace doorlose {
namespace database {
namespace {

class database_test : public ::testing::Test
{
public:
    database_test()
    {
    }
};

TEST_F(database_test, abc)
{
    SUCCEED();
}

} // namespace
} // namespace database
} // namespace doorlose