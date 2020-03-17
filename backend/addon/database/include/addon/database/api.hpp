#pragma once

#include <addon/database/database.hpp>

#include <string_view>
#include <tuple>

namespace addon {
namespace database {

enum status_t : std::uint8_t
{
    OK,
    ARGUMENT_ERROR,
    INTERNAL_ERROR,
    LOST_MESSAGE,
    NOT_INITIALIZED,
    JS_SIDE_ERROR,
};

/**
 *
 */
ADDON_DATABASE_EXPORT std::tuple<status_t, std::string_view> initialize() noexcept;

/**
 *
 */
ADDON_DATABASE_EXPORT std::tuple<status_t, std::string_view, uint_t> get_task(
    uint_t client_id, uint_t group_id) noexcept;

/**
 *
 */
ADDON_DATABASE_EXPORT std::tuple<status_t, std::string_view, uint_t> task_count(
    uint_t group_id) noexcept;

/**
 *
 */
ADDON_DATABASE_EXPORT std::tuple<status_t, std::string_view, bool> examine_task(
    uint_t group_id, uint_t task_id) noexcept;

/**
 *
 */
ADDON_DATABASE_EXPORT std::tuple<status_t, std::string_view> set_removed_flag(
    uint_t group_id, uint_t task_id, bool removed) noexcept;

/**
 *
 */
ADDON_DATABASE_EXPORT std::tuple<status_t, std::string_view> update_task(
    uint_t group_id, uint_t task_id, std::string_view task) noexcept;

/**
 *
 */
ADDON_DATABASE_EXPORT std::tuple<status_t, std::string_view> add_task(
    uint_t group_id, std::string_view task) noexcept;

/**
 *
 */
ADDON_DATABASE_EXPORT std::tuple<status_t, std::string_view> serialize(
    const std::filesystem::path &to_folder) noexcept;

/**
 *
 */
ADDON_DATABASE_EXPORT std::tuple<status_t, std::string_view> deserialize(
    const std::filesystem::path &from_folder) noexcept;

} // namespace database
} // namespace addon