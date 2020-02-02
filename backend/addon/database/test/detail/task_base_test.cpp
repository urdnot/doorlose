#include <addon/database/detail/task_base.hpp>
#include <addon/database/detail/utils.hpp>

#include <gtest/gtest.h>

namespace addon {
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
    EXPECT_THROW(task_base tb(123, 10), invalid_argument);
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
    const uint_t START_CAPACITY = 10;
    task_base tb(1024, START_CAPACITY);


    EXPECT_EQ(0, tb.size());
    EXPECT_EQ(START_CAPACITY, tb.capacity());

    for (uint_t i = 0; i < START_CAPACITY; ++i)
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
    const auto res = tb.get(0);
    EXPECT_EQ(TEST_TASK, res.first);
    EXPECT_FALSE(res.second);
}

TEST_F(task_base_test, add_with_overflow_size)
{
    task_base tb(1024, 10);
    const std::string overflow_text(tb.max_task_size() + 1, 'c');
    EXPECT_THROW(tb.add(overflow_text), invalid_argument);
}

TEST_F(task_base_test, get_task_id_out_of_range)
{
    task_base tb(1024, 10);
    EXPECT_THROW(tb.get(1), invalid_argument);
}

TEST_F(task_base_test, get_removed_task)
{
    task_base tb(1024, 10);
    const auto id = tb.add(TEST_TASK);
    EXPECT_NO_THROW(tb.update(id, true));
    const auto res = tb.get(id);
    EXPECT_EQ(TEST_TASK, res.first);
    EXPECT_TRUE(res.second);
}

TEST_F(task_base_test, update_remove_overload_out_of_range)
{
    task_base tb(1024, 10);
    EXPECT_THROW(tb.update(0, true), invalid_argument);
}

TEST_F(task_base_test, update_remove_overload_do_not_change_size)
{
    task_base tb(1024, 10);
    const auto id = tb.add(TEST_TASK);
    tb.update(id, true);
    EXPECT_EQ(1, tb.size());
}

TEST_F(task_base_test, update_text_overload_out_of_range)
{
    task_base tb(1024, 10);
    EXPECT_THROW(tb.update(0, TEST_TASK), invalid_argument);
}

TEST_F(task_base_test, update_with_oveflow_task_size)
{
    task_base tb(1024, 10);
    const auto id = tb.add(TEST_TASK);
    const std::string overflow_text(tb.max_task_size() + 1, 'c');
    EXPECT_THROW(tb.update(id, overflow_text), invalid_argument);
}

TEST_F(task_base_test, update_change_task_text)
{
    task_base tb(1024, 10);
    const auto id = tb.add(TEST_TASK);
    tb.update(id, TEST_TASK_1);
    const auto res = tb.get(id);
    EXPECT_EQ(TEST_TASK_1, res.first);
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
    EXPECT_EQ(TEST_TASK, tb.get(0).first);
    EXPECT_EQ(TEST_TASK_1, tb.get(1).first);
}

TEST_F(task_base_test, deserialize_from_nonexistent_path)
{
    task_base tb;
    EXPECT_THROW(tb.deserialize("nonexistent_path.db"), invalid_argument);
}

TEST_F(task_base_test, swap_check)
{
    task_base a(100, 10);
    a.add(TEST_TASK);

    task_base b(200, 20);
    b.add(TEST_TASK_1);
    b.add(TEST_TASK_1);
    b.update(0, true);

    a.swap(b);

    EXPECT_EQ(200, a.max_task_size());
    EXPECT_EQ(20, a.capacity());
    EXPECT_EQ(2, a.size());
    EXPECT_EQ(TEST_TASK_1, a.get(0).first);
    EXPECT_TRUE(a.get(0).second);
    EXPECT_EQ(TEST_TASK_1, a.get(1).first);

    EXPECT_EQ(100, b.max_task_size());
    EXPECT_EQ(10, b.capacity());
    EXPECT_EQ(1, b.size());
    EXPECT_EQ(TEST_TASK, b.get(0).first);
}

} // namespace
} // namespace detail
} // namespace database
} // namespace addon