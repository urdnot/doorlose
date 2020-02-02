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
    EXPECT_EQ(TEST_TASK, res.first);
    EXPECT_EQ(0, res.second);

    const auto res1 = db.get_task(0, 0);
    EXPECT_EQ(TEST_TASK, res1.first);
    EXPECT_EQ(0, res1.second);

    const auto res2 = db.get_task(database::UNDEFINED_CLIENT_ID, 0);
    EXPECT_EQ(TEST_TASK, res2.first);
    EXPECT_EQ(1, res2.second);
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
    EXPECT_EQ(TEST_TASK_1, res.first);
    const auto res1 = db.get_task(res.second, 0);
    EXPECT_EQ(TEST_TASK_1, res1.first);
    const auto res2 = db.get_task(res.second, 0);
    EXPECT_EQ(TEST_TASK_1, res2.first);
}

TEST_F(database_test, task_count_group_id_out_of_range)
{
    database db;
    db.add_task(0, TEST_TASK);
    EXPECT_THROW(db.task_count(10), invalid_argument);
}

TEST_F(database_test, task_count_check)
{
    database db;
    db.add_task(0, TEST_TASK);
    db.update_task(0, 0, true);
    db.add_task(0, TEST_TASK);

    db.add_task(1, TEST_TASK_1);
    
    EXPECT_EQ(2, db.task_count(0));
    EXPECT_EQ(1, db.task_count(1));
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

TEST_F(database_test, update_task_group_id_out_of_range_remove_overload)
{
    database db;
    db.add_task(0, TEST_TASK);
    EXPECT_THROW(db.update_task(10, 0, true), invalid_argument);
}

TEST_F(database_test, update_task_remove_task_usage)
{
    database db;
    db.add_task(0, TEST_TASK);
    db.update_task(0, 0, true);
    const auto res = db.examine_task(0, 0);
    EXPECT_EQ(TEST_TASK, res.first);
    EXPECT_TRUE(res.second);
    db.update_task(0, 0, false);
    const auto res1 = db.examine_task(0, 0);
    EXPECT_EQ(TEST_TASK, res1.first);
    EXPECT_FALSE(res1.second);
}

TEST_F(database_test, update_task_group_id_out_of_range_update_overload)
{
    database db;
    db.add_task(0, TEST_TASK);
    EXPECT_THROW(db.update_task(10, 0, TEST_TASK_1), invalid_argument);
}

TEST_F(database_test, update_task_change_text)
{
    database db;
    db.add_task(0, TEST_TASK);
    db.update_task(0, 0, TEST_TASK_1);
    const auto res = db.examine_task(0, 0);
    EXPECT_EQ(TEST_TASK_1, res.first);
    EXPECT_FALSE(res.second);
}

TEST_F(database_test, add_task_group_id_out_of_range)
{
    database db;
    EXPECT_THROW(db.add_task(10, TEST_TASK), invalid_argument);
}

TEST_F(database_test, add_task_check)
{
    database db;
    db.add_task(0, TEST_TASK);
    const auto res = db.get_task(database::UNDEFINED_CLIENT_ID, 0);
    EXPECT_EQ(TEST_TASK, res.first);
}

TEST_F(database_test, serialize_create_folder_and_files)
{
    const uint_t GROUPS_COUNT = 6;
    database db;
    for (uint_t i = 0; i < GROUPS_COUNT; ++i)
    {
        db.add_task(i, TEST_TASK);
        db.add_task(i, TEST_TASK_1);
    }

    std::vector<std::pair<std::string_view, uint_t>> res;

    for (uint_t i = 0; i < GROUPS_COUNT; ++i)
    {
        res.push_back(db.get_task(database::UNDEFINED_CLIENT_ID, i));
    }

    db.serialize(TEST_FOLDER_PATH);
    EXPECT_TRUE(std::filesystem::exists(TEST_FOLDER_PATH));
    EXPECT_TRUE(std::filesystem::exists(TEST_FOLDER_PATH / "clients.db"));

    for (uint_t i = 0; i < GROUPS_COUNT; ++i)
    {
        const std::string choise_db = std::to_string(i) + "-choises.db";
        const std::string task_db = std::to_string(i) + "-tasks.db";
        EXPECT_TRUE(std::filesystem::exists(TEST_FOLDER_PATH / choise_db));
        EXPECT_TRUE(std::filesystem::exists(TEST_FOLDER_PATH / task_db));
    }
}

TEST_F(database_test, serialize_deserialize)
{
    const uint_t GROUPS_COUNT = 6;
    database db;
    for (uint_t i = 0; i < GROUPS_COUNT; ++i)
    {
        db.add_task(i, TEST_TASK);
        db.add_task(i, TEST_TASK_1);
    }

    std::vector<std::pair<std::string, uint_t>> res;

    for (uint_t i = 0; i < GROUPS_COUNT; ++i)
    {
        const auto tmp = db.get_task(database::UNDEFINED_CLIENT_ID, i);
        res.emplace_back(tmp.first, tmp.second);
    }

    db.serialize(TEST_FOLDER_PATH);
    db.deserialize(TEST_FOLDER_PATH);

    for (uint_t i = 0; i < GROUPS_COUNT; ++i)
    {
        const auto result = db.get_task(res[i].second, i);

        if (res[i].first == TEST_TASK_1)
        {
            EXPECT_EQ(TEST_TASK, result.first);
        }
        else
        {
            EXPECT_EQ(TEST_TASK_1, result.first);
        }
    }
}

TEST_F(database_test, deserialize_from_invalid_serialization)
{
    database db;
    EXPECT_THROW(db.deserialize("nonexistent_path"), invalid_argument);
}

} // namespace
} // namespace database
} // namespace addon