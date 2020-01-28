#pragma once

#include <addon/database/database.hpp>

namespace addon {
namespace database {

enum status_t : std::uint8_t
{
    OK,
    ARGUMENT_ERROR,
    INTERNAL_ERROR,
    LOST_MESSAGE,
};

ADDON_DATABASE_EXPORT std::string make_hello_world(std::string hello);

ADDON_DATABASE_EXPORT std::string foo(std::uint64_t id);

} // namespace database
} // namespace addon