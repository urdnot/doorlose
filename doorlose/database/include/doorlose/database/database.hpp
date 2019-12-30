#pragma once

#include <doorlose/database/detail/common.hpp>

#include <filesystem>

namespace doorlose {
namespace database {

class DOORLOSE_DATABASE_EXPORT database
{
public:
    struct get_request_t
    {
        std::uint64_t client_id;
        std::uint64_t group_id;
    };

    struct get_response_t
    {
        std::uint64_t client_id;
        std::uint64_t group_id;
        std::string_view task;
    };

public:
    explicit database();

    //explicit database();

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
};

} // namespace database
} // namespace doorlose