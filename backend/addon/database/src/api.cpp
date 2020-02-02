#include <addon/database/api.hpp>

#include <filesystem>
#include <memory>
#include <new>
#include <system_error>

namespace addon {
namespace database {
namespace {

std::unique_ptr<database> g_database;

} // namespace

std::tuple<status_t, std::string_view> initialize() noexcept
{
    try
    {
        try
        {
            g_database.reset(new database);
            return std::make_tuple(status_t::OK, "");
        }
        catch (const invalid_argument &e)
        {
            return std::make_tuple(status_t::ARGUMENT_ERROR, e.what());
        }
        catch (const std::bad_alloc &)
        {
            return std::make_tuple(status_t::INTERNAL_ERROR,
                "Memory allocation error during database creation");
        }
    }
    catch (...)
    {
        return std::make_tuple(status_t::LOST_MESSAGE, "initialize() unknown exception");
    }
}

std::tuple<status_t, std::string_view, uint_t> get_task(uint_t client_id, uint_t group_id) noexcept
{
    if (g_database == nullptr)
    {
        return std::make_tuple(status_t::NOT_INITIALIZED, "get_task(): database not initialized", 0);
    }

    try
    {
        try
        {
            const auto result = g_database->get_task(client_id, group_id);
            return std::make_tuple(status_t::OK, result.first, result.second);
        }
        catch (const invalid_argument &e)
        {
            return std::make_tuple(status_t::ARGUMENT_ERROR, e.what(), 0);
        }
        catch (const runtime_error &e)
        {
            return std::make_tuple(status_t::INTERNAL_ERROR, e.what(), 0);
        }
        catch (const std::system_error &e)
        {
            return std::make_tuple(status_t::INTERNAL_ERROR, e.what(), 0);
        }
    }
    catch (...)
    {
        return std::make_tuple(status_t::LOST_MESSAGE, "get_task() unknown exception", 0);
    }
}

std::tuple<status_t, std::string_view, uint_t> task_count(uint_t group_id) noexcept
{
    if (g_database == nullptr)
    {
        return std::make_tuple(status_t::NOT_INITIALIZED, "task_count(): database not initialized", 0);
    }

    try
    {
        try
        {
            const auto result = g_database->task_count(group_id);
            return std::make_tuple(status_t::OK, "", result);
        }
        catch (const invalid_argument &e)
        {
            return std::make_tuple(status_t::ARGUMENT_ERROR, e.what(), 0);
        }
        catch (const std::system_error &e)
        {
            return std::make_tuple(status_t::INTERNAL_ERROR, e.what(), 0);
        }
    }
    catch (...)
    {
        return std::make_tuple(status_t::LOST_MESSAGE, "task_count() unknown exception", 0);
    }
}

std::tuple<status_t, std::string_view, bool> examine_task(uint_t group_id, uint_t task_id) noexcept
{
    if (g_database == nullptr)
    {
        return std::make_tuple(status_t::NOT_INITIALIZED, "examine_count(): database not initialized", false);
    }

    try
    {
        try
        {
            const auto result = g_database->examine_task(group_id, task_id);
            return std::make_tuple(status_t::OK, result.first, result.second);
        }
        catch (const invalid_argument &e)
        {
            return std::make_tuple(status_t::ARGUMENT_ERROR, e.what(), false);
        }
        catch (const std::system_error &e)
        {
            return std::make_tuple(status_t::INTERNAL_ERROR, e.what(), false);
        }
    }
    catch (...)
    {
        return std::make_tuple(status_t::LOST_MESSAGE, "examine_task() unknown exception", false);
    }
}

std::tuple<status_t, std::string_view> set_removed_flag(uint_t group_id, uint_t task_id,
    bool removed) noexcept
{
    if (g_database == nullptr)
    {
        return std::make_tuple(status_t::NOT_INITIALIZED, "set_removed_flag(): database not initialized");
    }

    try
    {
        try
        {
            g_database->update_task(group_id, task_id, removed);
            return std::make_tuple(status_t::OK, "");
        }
        catch (const invalid_argument &e)
        {
            return std::make_tuple(status_t::ARGUMENT_ERROR, e.what());
        }
        catch (const std::system_error &e)
        {
            return std::make_tuple(status_t::INTERNAL_ERROR, e.what());
        }
    }
    catch (...)
    {
        return std::make_tuple(status_t::LOST_MESSAGE, "set_removed_flag() unknown exception");
    }
}

std::tuple<status_t, std::string_view> update_task(uint_t group_id, uint_t task_id,
    std::string_view task) noexcept
{
    if (g_database == nullptr)
    {
        return std::make_tuple(status_t::NOT_INITIALIZED, "update_task(): database not initialized");
    }

    try
    {
        try
        {
            g_database->update_task(group_id, task_id, task);
            return std::make_tuple(status_t::OK, "");
        }
        catch (const invalid_argument &e)
        {
            return std::make_tuple(status_t::ARGUMENT_ERROR, e.what());
        }
        catch (const std::system_error &e)
        {
            return std::make_tuple(status_t::INTERNAL_ERROR, e.what());
        }
    }
    catch (...)
    {
        return std::make_tuple(status_t::LOST_MESSAGE, "update_task() unknown exception");
    }
}

std::tuple<status_t, std::string_view> add_task(uint_t group_id, std::string_view task) noexcept
{
    if (g_database == nullptr)
    {
        return std::make_tuple(status_t::NOT_INITIALIZED, "add_task(): database not initialized");
    }

    try
    {
        try
        {
            g_database->add_task(group_id, task);
            return std::make_tuple(status_t::OK, "");
        }
        catch (const invalid_argument &e)
        {
            return std::make_tuple(status_t::ARGUMENT_ERROR, e.what());
        }
        catch (const runtime_error &e)
        {
            return std::make_tuple(status_t::INTERNAL_ERROR, e.what());
        }
        catch (const std::system_error &e)
        {
            return std::make_tuple(status_t::INTERNAL_ERROR, e.what());
        }
    }
    catch (...)
    {
        return std::make_tuple(status_t::LOST_MESSAGE, "add_task() unknown exception");
    }
}

std::tuple<status_t, std::string_view> serialize(const std::filesystem::path &to_folder) noexcept
{
    if (g_database == nullptr)
    {
        return std::make_tuple(status_t::NOT_INITIALIZED, "serialize(): database not initialized");
    }

    try
    {
        try
        {
            g_database->serialize(to_folder);
            return std::make_tuple(status_t::OK, "");
        }
        catch (const invalid_argument &e)
        {
            return std::make_tuple(status_t::ARGUMENT_ERROR, e.what());
        }
        catch (const runtime_error &e)
        {
            return std::make_tuple(status_t::INTERNAL_ERROR, e.what());
        }
        catch (const std::system_error &e)
        {
            return std::make_tuple(status_t::INTERNAL_ERROR, e.what());
        }
        catch (const std::bad_alloc &)
        {
            return std::make_tuple(status_t::INTERNAL_ERROR,
                "Memory allocation error during database serialization");
        }
    }
    catch (...)
    {
        return std::make_tuple(status_t::LOST_MESSAGE, "serialize() unknown exception");
    }
}

std::tuple<status_t, std::string_view> deserialize(const std::filesystem::path &from_folder) noexcept
{
    if (g_database == nullptr)
    {
        return std::make_tuple(status_t::NOT_INITIALIZED, "deserialize(): database not initialized");
    }

    try
    {
        try
        {
            g_database->deserialize(from_folder);
            return std::make_tuple(status_t::OK, "");
        }
        catch (const invalid_argument &e)
        {
            return std::make_tuple(status_t::ARGUMENT_ERROR, e.what());
        }
        catch (const runtime_error &e)
        {
            return std::make_tuple(status_t::INTERNAL_ERROR, e.what());
        }
        catch (const std::system_error &e)
        {
            return std::make_tuple(status_t::INTERNAL_ERROR, e.what());
        }
        catch (const std::bad_alloc &)
        {
            return std::make_tuple(status_t::INTERNAL_ERROR,
                "Memory allocation error during database deserialization");
        }
    }
    catch (...)
    {
        return std::make_tuple(status_t::LOST_MESSAGE, "deserialize() unknown exception");
    }
}

} // namespace database
} // namespace addon