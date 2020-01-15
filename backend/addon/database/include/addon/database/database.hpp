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

struct ADDON_DATABASE_EXPORT get_request_t
{
    std::uint64_t client_id;
    std::uint64_t group_id;
};

struct ADDON_DATABASE_EXPORT get_response_t
{
    std::uint64_t client_id;
    std::uint64_t group_id;
    std::string_view task;
};

class ADDON_DATABASE_EXPORT database
{
public:
    explicit database();

    explicit database(std::uint64_t groups_count);

    ///////////////////////////////////////////////////////////////////////////
    // Client
    ///////////////////////////////////////////////////////////////////////////

    /**
     * Get random task from specified group for specified client
     */
    get_response_t get_task(get_request_t req);


    ///////////////////////////////////////////////////////////////////////////
    // Administration
    ///////////////////////////////////////////////////////////////////////////

    /**
     *
     */
    std::string_view examine_task(std::uint64_t group_id, std::uint64_t task_id);

    /**
     *
     */
    void update_task(std::uint64_t group_id, std::uint64_t task_id, std::string_view task);

    /**
     *
     */
    void remove_task(std::uint64_t group_id, std::uint64_t task_id);

    /**
     * Add tasks from UTF-8 json file
     */
    void add_from(const std::filesystem::path &from);

    /**
     * Replace tasks from UTF-8 json file, wipe all clients choises
     */
    void replace_from(const std::filesystem::path &from);

    void serialize(const std::filesystem::path &to_folder) const;
    void deserialize(const std::filesystem::path &from_folder);

private:
    struct client_record_t
    {
        std::mutex mtx;
        std::time_t last_active;
    };

    struct group_t
    {
        std::unique_ptr<detail::choise_base> choises;
        std::unique_ptr<detail::task_base> tasks;
    };

private:
    const std::uint64_t START_MASK_SIZE = 512;       // bits
    const std::uint64_t MASK_GRANULARITY = 512;      // bits
    const std::uint64_t CLIENT_GRANULARITY = 1000;   // clients
    const std::uint64_t MAX_TASK_SIZE = 4096;        // bytes
    const std::uint64_t TASK_GRANULARITY = 1000;     // tasks

    std::shared_mutex base_mtx_;
    std::vector<client_record_t> clients_;
    std::vector<group_t> groups_;
};

} // namespace database
} // namespace addon