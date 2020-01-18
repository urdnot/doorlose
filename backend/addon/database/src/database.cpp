#include <addon/database/database.hpp>

#include <addon/database/detail/utils.hpp>

#include <ctime>

#include <utility>

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
    //
    // Inits
    //
    for (std::uint64_t i = 0; i < GROUPS_COUNT; ++i)
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
std::pair<std::uint64_t, std::string_view> database::get_task(std::uint64_t client_id,
    std::uint64_t group_id)
{
    std::shared_lock lock(base_mtx_);

    if (client_id != UNDEFINED_CLIENT_ID && client_id >= clients_.size())
    {
        throw detail::invalid_argument("Client id out of range");
    }

    if (group_id >= groups_.size())
    {
        throw detail::invalid_argument("Group id out of range");
    }

    if (client_id == UNDEFINED_CLIENT_ID)
    {
        try
        {
            clients_.push_back(client_record_t());
        }
        catch (...)
        {
            throw detail::runtime_error(
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

                throw detail::runtime_error(
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

    return std::make_pair(client_id, task);
}

///////////////////////////////////////////////////////////////////////////
// Administration
///////////////////////////////////////////////////////////////////////////

/**
 *
 */
std::string_view database::examine_task(std::uint64_t group_id, std::uint64_t task_id) const
{
    std::shared_lock lock(base_mtx_);

    if (group_id >= groups_.size())
    {
        throw detail::invalid_argument("Group id out of range");
    }

    return groups_[group_id].tasks->get(task_id);
}

/**
 *
 */
void database::update_task(
    std::uint64_t group_id, std::uint64_t task_id, std::string_view task)
{

}

/**
 *
 */
void database::remove_task(std::uint64_t group_id, std::uint64_t task_id)
{
}

/**
 * Add tasks from UTF-8 json file
 */
void database::add_from(const std::filesystem::path &from)
{
}

/**
 * Replace tasks from UTF-8 json file, wipe all clients choises
 */
void database::replace_from(const std::filesystem::path &from)
{
}

void database::serialize(const std::filesystem::path &to_folder) const
{
}

void database::deserialize(const std::filesystem::path &from_folder)
{
}

} // namespace database
} // namespace addon