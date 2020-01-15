#include <addon/database/detail/choise_base.hpp>

#include <gtest/gtest.h>

#include <algorithm>

namespace addon {
namespace database {
namespace detail {
namespace {

const std::filesystem::path FILE_PATH = "choise_test.db";

class choise_base_test : public ::testing::Test
{
public:
    choise_base_test()
    {
        std::filesystem::remove(FILE_PATH);
    }
};

TEST_F(choise_base_test, empty_ctor)
{
    choise_base cb;
    EXPECT_EQ(0, cb.size());
    EXPECT_EQ(0, cb.capacity());
    EXPECT_EQ(0, cb.mask_size());
    EXPECT_EQ(0, cb.mask_capacity());
}

TEST_F(choise_base_test, ctor_base_initialize_check)
{
    choise_base cb(13, 64, 10);

    EXPECT_EQ(0, cb.size());
    EXPECT_EQ(10, cb.capacity());
    EXPECT_EQ(13, cb.mask_size());
    EXPECT_EQ(64, cb.mask_capacity());
}

TEST_F(choise_base_test, invalid_mask_granularity)
{
    EXPECT_THROW(choise_base cb(13, 65, 10), std::invalid_argument);
}

TEST_F(choise_base_test, zero_mask_granularity)
{
    EXPECT_THROW(choise_base cb(13, 0, 10), std::invalid_argument);
}

TEST_F(choise_base_test, zero_granularity)
{
    EXPECT_THROW(choise_base cb(13, 64, 0), std::invalid_argument);
}

TEST_F(choise_base_test, add_client_adds_to_the_end)
{
    choise_base cb(13, 64, 10);
    EXPECT_EQ(0, cb.add());
    EXPECT_EQ(1, cb.add());
}

TEST_F(choise_base_test, add_client_change_size)
{
    choise_base cb(13, 64, 10);

    EXPECT_EQ(0, cb.size());
    EXPECT_EQ(10, cb.capacity());
    cb.add();
    EXPECT_EQ(1, cb.size());
    EXPECT_EQ(10, cb.capacity());
}

TEST_F(choise_base_test, add_client_more_capacity_increase_capacity)
{
    choise_base cb(13, 64, 10);

    EXPECT_EQ(0, cb.size());
    EXPECT_EQ(10, cb.capacity());
    for (std::uint64_t i = 0; i < 10; ++i)
    {
        EXPECT_EQ(i, cb.add());
        EXPECT_EQ(10, cb.capacity());
    }
    EXPECT_EQ(10, cb.add());
    EXPECT_EQ(20, cb.capacity());
}

TEST_F(choise_base_test, choose_task_index)
{
    const std::uint64_t TASK_COUNT = 13;
    choise_base cb(TASK_COUNT, 64, 10);
    const auto client_id = cb.add();
    std::vector<std::uint64_t> generated;
    for (std::uint64_t i = 0; i < TASK_COUNT; ++i)
    {
        const std::uint64_t num = cb.choose(client_id);

        EXPECT_TRUE(0 <= num && num <TASK_COUNT);

        const auto begin = generated.begin();
        const auto end = generated.end();
        if (std::find(begin, end, num) != end)
        {
            FAIL();
            return;
        }
        else
        {
            generated.push_back(num);
        }
    }
    SUCCEED();
}

TEST_F(choise_base_test, choose_task_cyclic)
{
    const std::uint64_t TASK_COUNT = 13;
    choise_base cb(TASK_COUNT, 64, 10);
    const auto client_id = cb.add();
    for (std::uint64_t i = 0; i < TASK_COUNT; ++i)
    {
        cb.choose(client_id);
    }
    
    std::vector<std::uint64_t> generated;
    for (std::uint64_t i = 0; i < TASK_COUNT; ++i)
    {
        const std::uint64_t num = cb.choose(client_id);

        EXPECT_TRUE(0 <= num && num <TASK_COUNT);

        const auto begin = generated.begin();
        const auto end = generated.end();
        if (std::find(begin, end, num) != end)
        {
            FAIL();
            return;
        }
        else
        {
            generated.push_back(num);
        }
    }
    SUCCEED();
}

TEST_F(choise_base_test, choose_task_client_id_out_of_range)
{
    choise_base cb(13, 64, 10);
    EXPECT_EQ(0, cb.size());
    EXPECT_THROW(cb.choose(1), std::out_of_range);
}

TEST_F(choise_base_test, increase_mask_change_mask_size)
{
    choise_base cb(13, 64, 10);
    EXPECT_EQ(13, cb.mask_size());
    cb.increase_mask(2);
    EXPECT_EQ(15, cb.mask_size());
}

TEST_F(choise_base_test, increase_mask_more_capacity_change_mask_capacity)
{
    choise_base cb(63, 64, 10);
    EXPECT_EQ(63, cb.mask_size());
    EXPECT_EQ(64, cb.mask_capacity());
    cb.increase_mask(1);
    EXPECT_EQ(64, cb.mask_size());
    EXPECT_EQ(64, cb.mask_capacity());
    cb.increase_mask(1);
    EXPECT_EQ(65, cb.mask_size());
    EXPECT_EQ(128, cb.mask_capacity());
}

TEST_F(choise_base_test, serialize_create_file)
{
    choise_base cb(13, 64, 10);
    const auto id1 = cb.add();
    const auto id2 = cb.add();
    cb.choose(id1);
    cb.choose(id2);

    cb.serialize(FILE_PATH);
    EXPECT_TRUE(std::filesystem::exists(FILE_PATH));
}

TEST_F(choise_base_test, serialize_deserialize)
{
    std::vector<std::uint64_t> expected;
    std::uint64_t client_id;
    {
        choise_base cb(32, 64, 10);

        client_id = cb.add();
        for (std::uint64_t i = 0; i < 16; ++i)
        {
            cb.choose(client_id);
        }

        cb.serialize(FILE_PATH);

        for (std::uint64_t i = 0; i < 16; ++i)
        {
            expected.push_back(cb.choose(client_id));
        }
        std::sort(expected.begin(), expected.end());
    }

    choise_base cb;
    cb.deserialize(FILE_PATH);

    std::vector<std::uint64_t> result;
    for (std::uint64_t i = 0; i < 16; ++i)
    {
        result.push_back(cb.choose(client_id));
    }
    std::sort(result.begin(), result.end());

    EXPECT_EQ(expected, result);
}

TEST_F(choise_base_test, deserialize_from_nonexistent_path)
{
    choise_base cb;
    EXPECT_THROW(cb.deserialize("nonexistent_path.db"), std::invalid_argument);
}

} // namespace
} // namespace detail
} // namespace database
} // namespace addon