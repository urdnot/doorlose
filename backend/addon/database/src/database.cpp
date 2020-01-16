#include <addon/database/database.hpp>

#include <addon/database/detail/utils.hpp>

#include <ctime>

namespace addon {
namespace database {
namespace {

} // namespace

using detail::check_non_zero;
using detail::check_mult;
using detail::check_limit;
using detail::check_ostream;
using detail::check_istream;

database::database()
{
}

database::database(std::uint64_t groups_count)
{
    //
    // Checks
    //
    check_non_zero(groups_count, "database::ctor(): `groups_count` cannot be zero");

    //
    // Inits
    //
    for (std::uint64_t i = 0; i < groups_count; ++i)
    {
        group_t group;
        group.choises =
            std::make_unique<detail::choise_base>(START_MASK_SIZE, MASK_GRANULARITY, CLIENT_GRANULARITY);
        group.tasks =
            std::make_unique<detail::task_base>(MAX_TASK_SIZE, TASK_GRANULARITY);
        groups_.push_back(group);
    }
}

///////////////////////////////////////////////////////////////////////////
// Client
///////////////////////////////////////////////////////////////////////////

/**
 * Get random task from specified group for specified client
 */
std::tuple<status_t, std::uint64_t, std::string_view> database::get_task(
    std::uint64_t client_id, std::uint64_t group_id) noexcept
{
    std::shared_lock lock(base_mtx_);

    if (client_id != UNDEFINED_CLIENT_ID && client_id >= clients_.size())
    {
        return std::make_tuple(INVALID_ARGUMENTS, client_id,
            "Client id out of range");
    }

    if (group_id >= groups_.size())
    {
        return std::make_tuple(INVALID_ARGUMENTS, client_id,
            "Group id out of range");
    }

    if (client_id == UNDEFINED_CLIENT_ID)
    {
        try
        {
            clients_.push_back(client_record_t());
        }
        catch (...)
        {
            return std::make_tuple(INTERNAL_ERROR, client_id,
                "Memory allocation error during client addition to clients vector");
        }

        for (std::uint64_t i = 0; i < groups_.size(); ++i)
        {
            try
            {
                groups_[i].choises->add();
            }
            catch (...)
            {
                for (std::uint64_t j = 0; j < i; ++j)
                {
                    groups_[j].choises->rollback_add();
                }
                clients_.pop_back();

                return std::make_tuple(INTERNAL_ERROR, client_id,
                    "Memory allocation error during client addion to choises base");
            }
        }

        client_id = clients_.size() - 1;
    }

    std::string_view task;

    {
        std::lock_guard lock_client(clients_[client_id].mtx);

        while (task.empty())
        {
            const std::uint64_t task_id = groups_[group_id].choises->choose(client_id);
            task = groups_[group_id].tasks->get(task_id);
        }

        const auto cur_time = std::time(nullptr);
        clients_[client_id].last_active =
            (cur_time != (std::time_t)(-1)) ? cur_time : 0;
    }

    return std::make_tuple(OK, client_id, task);
}


///////////////////////////////////////////////////////////////////////////
// Administration
///////////////////////////////////////////////////////////////////////////

/**
 *
 */
std::tuple<status_t, std::string_view> database::examine_task(
    std::uint64_t group_id, std::uint64_t task_id)
{
    
}

/**
 *
 */
std::tuple<status_t, std::string_view> database::update_task(
    std::uint64_t group_id, std::uint64_t task_id, std::string_view task)
{

}

/**
 *
 */
std::tuple<status_t, std::string_view> database::remove_task(
    std::uint64_t group_id, std::uint64_t task_id)
{
}

/**
 * Add tasks from UTF-8 json file
 */
std::tuple<status_t, std::string_view> database::add_from(
    const std::filesystem::path &from)
{
}

/**
 * Replace tasks from UTF-8 json file, wipe all clients choises
 */
std::tuple<status_t, std::string_view> database::replace_from(
    const std::filesystem::path &from)
{
}

std::tuple<status_t, std::string_view> database::serialize(
    const std::filesystem::path &to_folder) const
{
}

std::tuple<status_t, std::string_view> database::deserialize(
    const std::filesystem::path &from_folder)
{
}

} // namespace database
} // namespace addon