#pragma once

#include <addon/database/detail/common.hpp>
#include <addon/database/detail/choise_base.hpp>
#include <addon/database/detail/task_base.hpp>

#include <filesystem>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <vector>

namespace addon {
namespace database {

class ADDON_DATABASE_EXPORT database
{
public:
    static const uint_t UNDEFINED_CLIENT_ID = 0xFFFFFFFF;

public:
    explicit database();

    ///////////////////////////////////////////////////////////////////////////
    // Client
    ///////////////////////////////////////////////////////////////////////////

    /**
     * Get random task from specified group for specified client
     */
    std::pair<std::string_view, uint_t> get_task(uint_t client_id, uint_t group_id);


    ///////////////////////////////////////////////////////////////////////////
    // Administration
    ///////////////////////////////////////////////////////////////////////////

    /**
     *
     */
    uint_t task_count(uint_t group_id) const;

    /**
     *
     */
    std::pair<std::string_view, bool> examine_task(uint_t group_id,
        uint_t task_id) const;

    /**
     *
     */
    void update_task(uint_t group_id, uint_t task_id, bool removed);

    /**
     *
     */
    void update_task(uint_t group_id, uint_t task_id, std::string_view task);

    /**
     * Add task
     */
    void add_task(uint_t group_id, std::string_view task);

    void serialize(const std::filesystem::path &to_folder) const;
    void deserialize(const std::filesystem::path &from_folder);

private:
    struct client_record_t
    {
        client_record_t()
            : last_active(0)
        {
        }

        client_record_t(client_record_t &&rec)
        {
            last_active = rec.last_active;
        }

        std::mutex mtx;
        std::time_t last_active;
    };

    struct group_t
    {
        group_t(std::unique_ptr<detail::choise_base> &&ch, std::unique_ptr<detail::task_base> &&tsk)
            : choises(std::move(ch))
            , tasks(std::move(tsk))
        {
        }

        std::unique_ptr<detail::choise_base> choises;
        std::unique_ptr<detail::task_base> tasks;
    };

private:
    const uint_t GROUPS_COUNT = 6;            // groups
    const uint_t MASK_GRANULARITY = 512;      // bits
    const uint_t CLIENT_GRANULARITY = 1000;   // clients
    const uint_t MAX_TASK_SIZE = 4096;        // bytes
    const uint_t TASK_GRANULARITY = 1000;     // tasks

    const std::string CLIENTS_FILE = "clients.db";
    const std::string CHOISES_FILE = "choises.db";
    const std::string TASKS_FILE = "tasks.db"; 

    mutable std::shared_mutex base_mtx_;
    std::vector<client_record_t> clients_;
    std::vector<group_t> groups_;
};

} // namespace database
} // namespace addon