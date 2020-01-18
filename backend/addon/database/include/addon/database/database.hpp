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

enum ADDON_DATABASE_EXPORT status_t : std::uint8_t
{
    OK = 0,
    INVALID_ARGUMENTS = 1,
    INTERNAL_ERROR = 2,
    LOST_ERROR_MESSAGE = 3,
};

class ADDON_DATABASE_EXPORT database
{
public:
    static const std::uint64_t UNDEFINED_CLIENT_ID = 0xFFFFFFFFFFFFFFFF;

public:
    explicit database();

    ///////////////////////////////////////////////////////////////////////////
    // Client
    ///////////////////////////////////////////////////////////////////////////

    /**
     * Get random task from specified group for specified client
     */
    std::pair<std::uint64_t, std::string_view> get_task(std::uint64_t client_id,
        std::uint64_t group_id);


    ///////////////////////////////////////////////////////////////////////////
    // Administration
    ///////////////////////////////////////////////////////////////////////////

    /**
     *
     */
    std::string_view examine_task(std::uint64_t group_id,
        std::uint64_t task_id) const;

    /**
     *
     */
    void update_task(std::uint64_t group_id,
        std::uint64_t task_id, std::string_view task);

    /**
     *
     */
    void remove_task(std::uint64_t group_id,
        std::uint64_t task_id);

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
        client_record_t()
            : last_active(0)
        {
        }

        std::mutex mtx;
        std::time_t last_active;
    };

    struct group_t
    {
        std::unique_ptr<detail::choise_base> choises;
        std::unique_ptr<detail::task_base> tasks;
    };

private:
    const std::uint64_t GROUPS_COUNT = 6;            // groups
    const std::uint64_t START_MASK_SIZE = 512;       // bits
    const std::uint64_t MASK_GRANULARITY = 512;      // bits
    const std::uint64_t CLIENT_GRANULARITY = 1000;   // clients
    const std::uint64_t MAX_TASK_SIZE = 4096;        // bytes
    const std::uint64_t TASK_GRANULARITY = 1000;     // tasks

    mutable std::shared_mutex base_mtx_;
    std::vector<client_record_t> clients_;
    std::vector<group_t> groups_;
};

} // namespace database
} // namespace addon