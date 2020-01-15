#include <addon/database/database.hpp>

#include <addon/database/detail/utils.hpp>

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
get_response_t database::get_task(get_request_t req)
{
    std::shared_lock lock(base_mtx_);

    req.client_id
}


///////////////////////////////////////////////////////////////////////////
// Administration
///////////////////////////////////////////////////////////////////////////

/**
 *
 */
std::string_view database::examine_task(std::uint64_t group_id, std::uint64_t task_id)
{
    return std::string_view();
}

/**
 *
 */
void database::update_task(std::uint64_t group_id, std::uint64_t task_id, std::string_view task)
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
void add_from(const std::filesystem::path &from)
{
}

/**
 * Replace tasks from UTF-8 json file, wipe all clients choises
 */
void replace_from(const std::filesystem::path &from)
{
}

void serialize(const std::filesystem::path &to_folder) const
{
}

void deserialize(const std::filesystem::path &from_folder)
{
}

} // namespace database
} // namespace addon