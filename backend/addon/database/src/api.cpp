#include <addon/database/api.hpp>

namespace addon {
namespace database {

std::string make_hello_world(std::string hello)
{
    return hello + " world";
}

std::string foo(std::uint64_t id)
{
    return std::to_string(id) + " number";
}

} // namespace database
} // namespace addon