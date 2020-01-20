#include <addon/database/database.hpp>

#include <addon/database/detail/utils.hpp>

#include <ctime>

#include <filesystem>
#include <fstream>
#include <utility>

namespace addon {
namespace database {
namespace {

struct serialization_header_t
{
    std::uint64_t clients_count;
};

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
        throw detail::invalid_argument("database::get_task(): Client id out of range");
    }

    if (group_id >= groups_.size())
    {
        throw detail::invalid_argument("database::get_task(): Group id out of range");
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
                "database::get_task(): Memory allocation error during client addition to clients vector");
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
                    "database::get_task(): Memory allocation error during client addion to choises base");
            }
        }

        client_id = clients_.size() - 1;
    }

    std::pair<std::string_view, bool> task_record;

    {
        std::lock_guard lock_client(clients_[client_id].mtx);

        while (!task_record.second)
        {
            const std::uint64_t task_id = groups_[group_id].choises->choose(client_id);
            task_record = groups_[group_id].tasks->get(task_id);
        }

        const auto cur_time = std::time(nullptr);
        clients_[client_id].last_active =
            (cur_time != (std::time_t)(-1)) ? cur_time : 0;
    }

    return std::make_pair(client_id, task_record.first);
}

///////////////////////////////////////////////////////////////////////////
// Administration
///////////////////////////////////////////////////////////////////////////

/**
 *
 */
std::pair<std::string_view, bool> database::examine_task(std::uint64_t group_id, std::uint64_t task_id) const
{
    std::shared_lock lock(base_mtx_);

    if (group_id >= groups_.size())
    {
        throw detail::invalid_argument("database::examine_task(): Group id out of range");
    }

    return groups_[group_id].tasks->get(task_id);
}

/**
 *
 */
void database::update_task(std::uint64_t group_id, std::uint64_t task_id,
    bool removed)
{
    std::unique_lock lock(base_mtx_);

    if (group_id >= groups_.size())
    {
        throw detail::invalid_argument("database::update_task(): Group id out of range");
    }

    groups_[group_id].tasks->update(task_id, removed);
}

/**
 *
 */
void database::update_task(
    std::uint64_t group_id, std::uint64_t task_id, std::string_view task)
{
    std::unique_lock lock(base_mtx_);

    if (group_id >= groups_.size())
    {
        throw detail::invalid_argument("database::update_task(): Group id out of range");
    }

    groups_[group_id].tasks->update(task_id, task);
}

/**
 * Add task
 */
void database::add_task(std::uint64_t group_id, std::string_view task)
{
    std::unique_lock lock(base_mtx_);

    try
    {
        groups_[group_id].choises->increase_mask(1);
    }
    catch (...)
    {
        throw detail::runtime_error(
            "database::add_task(): Memory allocation error during increase choises mask");
    }

    try
    {
        groups_[group_id].tasks->add(task);
    }
    catch (...)
    {
        groups_[group_id].choises->rollback_increase_mask();
        throw detail::runtime_error(
            "database::add_task(): Memory allocation error during task addition");
    }
}

void database::serialize(const std::filesystem::path &to_folder) const
{
    std::unique_lock lock(base_mtx_);

    std::filesystem::create_directory(to_folder);

    std::ofstream output(to_folder / CLIENTS_FILE,
        std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);

    check_ostream(output, "database::serialize():  cannot open clients output file for serialize");

    serialization_header_t header;
    header.clients_count = clients_.size();

    output.write(reinterpret_cast<const char *>(&header), sizeof(serialization_header_t));
    check_ostream(output, "database::serialize(): cannot serialize header");

    for (std::uint64_t i = 0; i < clients_.size(); ++i)
    {
        output.write(reinterpret_cast<const char *>(&clients_[i].last_active),
            sizeof(std::time_t));
        check_ostream(output, "database::serialize(): cannot serialize client data");
    }

    for (std::uint64_t i = 0; i < groups_.size(); ++i)
    {
        const std::string full_choise_name = std::to_string(i) + "-" + CHOISES_FILE;
        const std::string full_task_name = std::to_string(i) + "-" + TASKS_FILE;
        groups_[i].choises->serialize(to_folder / full_choise_name);
        groups_[i].tasks->serialize(to_folder / full_task_name);
    }
}

void database::deserialize(const std::filesystem::path &from_folder)
{
    std::unique_lock lock(base_mtx_);

    if (!std::filesystem::exists(from_folder / CLIENTS_FILE))
    {
        throw detail::invalid_argument("database::deserialize(): client file for deserialize do not exist");
    }

    std::ifstream input(from_folder / CLIENTS_FILE, std::ios_base::binary);
    check_istream(input, "database::deserialize(): cannot open clients file for deserialize");

    serialization_header_t header;
    input.read(reinterpret_cast<char *>(&header), sizeof(serialization_header_t));
    check_istream(input, "database::deserialize(): cannot deserialize header");

    std::vector<client_record_t> clients_swap;
    std::vector<detail::task_base> tasks_swap;
    std::vector<detail::choise_base> choises_swap;

    for (std::uint64_t i = 0; i < header.clients_count; ++i)
    {
        input.read(reinterpret_cast<char *>(&clients_swap[i].last_active), sizeof(std::time_t));
        check_istream(input, "database::deserialize(): cannot deserialize client data");
    }

    for (std::uint64_t i = 0; i < GROUPS_COUNT; ++i)
    {
        tasks_swap.push_back(detail::task_base());
        tasks_swap.back().deserialize(from_folder / TASKS_FILE);
        choises_swap.push_back(detail::choise_base());
        choises_swap.back().deserialize(from_folder / CHOISES_FILE);
    }

    clients_.swap(clients_swap);
    for (std::uint64_t i = 0; i < GROUPS_COUNT; ++i)
    {
        groups_[i].tasks->swap(tasks_swap[i]);
        groups_[i].choises->swap(choises_swap[i]);
    }
}

} // namespace database
} // namespace addon