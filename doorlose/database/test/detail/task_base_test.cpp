#include <doorlose/database/detail/task_base.hpp>

#include <gtest/gtest.h>

namespace doorlose {
namespace database {
namespace detail {
namespace {

const std::string TEST_TASK = "Test task text.";
const std::string TEST_TASK_1 = "Another one test task";
const std::filesystem::path FILE_PATH = "task_test.db";

class task_base_test : public ::testing::Test
{
public:
    task_base_test()
    {
        std::filesystem::remove(FILE_PATH);
    }
};

TEST_F(task_base_test, empty_ctor)
{
    task_base tb;
    EXPECT_EQ(0, tb.max_task_size());
    EXPECT_EQ(0, tb.capacity());
    EXPECT_EQ(0, tb.size());
}

TEST_F(task_base_test, ctor_base_initialize_check)
{
    task_base tb(1024, 10);
    EXPECT_EQ(1024, tb.max_task_size());
    EXPECT_EQ(10, tb.capacity());
    EXPECT_EQ(0, tb.size());
}

TEST_F(task_base_test, invalid_init_task_size)
{
    EXPECT_THROW(task_base tb(123, 10), std::invalid_argument);
}

TEST_F(task_base_test, add_adds_to_the_end)
{
    task_base tb(1024, 10);
    EXPECT_EQ(0, tb.add(TEST_TASK));
    EXPECT_EQ(1, tb.add(TEST_TASK));
}

TEST_F(task_base_test, add_change_size)
{
    task_base tb(1024, 10);

    EXPECT_EQ(0, tb.size());
    EXPECT_EQ(10, tb.capacity());
    tb.add(TEST_TASK);
    EXPECT_EQ(1, tb.size());
    EXPECT_EQ(10, tb.capacity());
}

TEST_F(task_base_test, add_more_capacity_increase_capacity)
{
    const std::uint64_t START_CAPACITY = 10;
    task_base tb(1024, START_CAPACITY);


    EXPECT_EQ(0, tb.size());
    EXPECT_EQ(START_CAPACITY, tb.capacity());

    for (std::uint64_t i = 0; i < START_CAPACITY; ++i)
    {
        tb.add(TEST_TASK);
        EXPECT_EQ(i + 1, tb.size());
        EXPECT_EQ(START_CAPACITY, tb.capacity());
    }

    tb.add(TEST_TASK);
    EXPECT_EQ(START_CAPACITY + 1, tb.size());
    EXPECT_EQ(2 * START_CAPACITY, tb.capacity());
}

TEST_F(task_base_test, add_and_get)
{
    task_base tb(1024, 10);

    tb.add(TEST_TASK);
    EXPECT_EQ(TEST_TASK, tb.get(0));
}

TEST_F(task_base_test, add_with_overflow_size)
{
    task_base tb(1024, 10);
    const std::string overflow_text(tb.max_task_size() + 1, 'c');
    EXPECT_THROW(tb.add(overflow_text), std::out_of_range);
}

TEST_F(task_base_test, get_id_out_of_range)
{
    task_base tb(1024, 10);
    EXPECT_THROW(tb.get(1), std::out_of_range);
}

TEST_F(task_base_test, get_removed_task_return_empty_string)
{
    task_base tb(1024, 10);
    const auto id = tb.add(TEST_TASK);
    EXPECT_NO_THROW(tb.remove(id));
    EXPECT_EQ("", tb.get(id));
}

TEST_F(task_base_test, remove_out_of_range)
{
    task_base tb(1024, 10);
    EXPECT_THROW(tb.remove(0), std::out_of_range);
}

TEST_F(task_base_test, remove_do_not_change_size)
{
    task_base tb(1024, 10);
    const auto id = tb.add(TEST_TASK);
    tb.remove(id);
    EXPECT_EQ(1, tb.size());
}

TEST_F(task_base_test, update_out_of_range)
{
    task_base tb(1024, 10);
    EXPECT_THROW(tb.update(0, TEST_TASK), std::out_of_range);
}

TEST_F(task_base_test, update_with_oveflow_task_size)
{
    task_base tb(1024, 10);
    const auto id = tb.add(TEST_TASK);
    const std::string overflow_text(tb.max_task_size() + 1, 'c');
    EXPECT_THROW(tb.update(id, overflow_text), std::out_of_range);
}

TEST_F(task_base_test, update_change_task_text)
{
    task_base tb(1024, 10);
    const auto id = tb.add(TEST_TASK);
    tb.update(id, TEST_TASK_1);
    EXPECT_EQ(TEST_TASK_1, tb.get(id));
}

TEST_F(task_base_test, update_removed_task_throw)
{
    task_base tb(1024, 10);
    const auto id = tb.add(TEST_TASK);
    tb.remove(id);
    EXPECT_THROW(tb.update(id, TEST_TASK_1), std::invalid_argument);
}

TEST_F(task_base_test, serialize_create_file)
{
    task_base tb(1024, 10);
    tb.add(TEST_TASK);
    tb.add(TEST_TASK_1);

    tb.serialize(FILE_PATH);
    EXPECT_TRUE(std::filesystem::exists(FILE_PATH));
}

TEST_F(task_base_test, serialize_deserialize)
{
    {
        task_base tb(1024, 10);
        tb.add(TEST_TASK);
        tb.add(TEST_TASK_1);

        tb.serialize(FILE_PATH);
    }
    
    task_base tb;
    tb.deserialize(FILE_PATH);

    EXPECT_EQ(1024, tb.max_task_size());
    EXPECT_EQ(10, tb.capacity());
    EXPECT_EQ(2, tb.size());
    EXPECT_EQ(TEST_TASK, tb.get(0));
    EXPECT_EQ(TEST_TASK_1, tb.get(1));
}

TEST_F(task_base_test, deserialize_from_nonexistent_path)
{
    task_base tb;
    EXPECT_THROW(tb.deserialize("nonexistent_path.db"), std::invalid_argument);
}

} // namespace
} // namespace detail
} // namespace database
} // namespace doorlose