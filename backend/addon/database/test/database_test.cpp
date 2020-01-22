#include <addon/database/database.hpp>

#include <gtest/gtest.h>

#include <filesystem>

namespace addon {
namespace database {
namespace {

const std::string TEST_TASK = "Test task text.";
const std::string TEST_TASK_1 = "Another one test task";
const std::filesystem::path TEST_FOLDER_PATH = "test_folder";

class database_test : public ::testing::Test
{
public:
    database_test()
    {
        std::filesystem::remove_all(TEST_FOLDER_PATH);
    }
};

TEST_F(database_test, ctor_check)
{
    EXPECT_NO_THROW(database db);
}

TEST_F(database_test, get_task_client_id_out_of_range)
{
    database db;
    EXPECT_THROW(db.get_task(0, 0), invalid_argument);
}

TEST_F(database_test, get_task_group_id_out_of_range)
{
    database db;
    EXPECT_THROW(db.get_task(database::UNDEFINED_CLIENT_ID, 10), invalid_argument);
}

TEST_F(database_test, get_task_from_one_variant)
{
    database db;
    EXPECT_NO_THROW(db.add_task(0, TEST_TASK));
    const auto res = db.get_task(database::UNDEFINED_CLIENT_ID, 0);
    EXPECT_EQ(0, res.first);
    EXPECT_EQ(TEST_TASK, res.second);

    const auto res1 = db.get_task(0, 0);
    EXPECT_EQ(0, res1.first);
    EXPECT_EQ(TEST_TASK, res1.second);

    const auto res2 = db.get_task(database::UNDEFINED_CLIENT_ID, 0);
    EXPECT_EQ(1, res2.first);
    EXPECT_EQ(TEST_TASK, res2.second);
}

TEST_F(database_test, get_task_except_removed)
{
    database db;
    db.add_task(0, TEST_TASK);
    db.add_task(0, TEST_TASK);
    db.add_task(0, TEST_TASK_1);
    db.update_task(0, 0, true);
    db.update_task(0, 1, true);

    const auto res = db.get_task(database::UNDEFINED_CLIENT_ID, 0);
    EXPECT_EQ(TEST_TASK_1, res.second);
    const auto res1 = db.get_task(res.first, 0);
    EXPECT_EQ(TEST_TASK_1, res1.second);
    const auto res2 = db.get_task(res.first, 0);
    EXPECT_EQ(TEST_TASK_1, res2.second);
}

TEST_F(database_test, examine_task_group_id_out_of_range)
{
    database db;
    db.add_task(0, TEST_TASK);
    EXPECT_THROW(db.examine_task(10, 0), invalid_argument);
}

TEST_F(database_test, examine_task_check_all_cases)
{
    database db;
    db.add_task(0, TEST_TASK);
    db.update_task(0, 0, true);
    db.add_task(0, TEST_TASK_1);

    const auto res = db.examine_task(0, 0);
    EXPECT_EQ(TEST_TASK, res.first);
    EXPECT_TRUE(res.second);

    const auto res1 = db.examine_task(0, 1);
    EXPECT_EQ(TEST_TASK_1, res1.first);
    EXPECT_FALSE(res1.second);
}

} // namespace
} // namespace database
} // namespace addon