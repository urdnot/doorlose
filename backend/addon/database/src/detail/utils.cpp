#include <addon/database/detail/utils.hpp>

namespace addon {
namespace database {
namespace detail {

void check_non_zero(std::uint64_t value, std::string_view error_msg)
{
    if (!value)
    {
        throw invalid_argument(error_msg.data());
    }
}

void check_mult(std::uint64_t dividend, std::uint64_t divider, std::string_view error_msg)
{
    if (!divider || dividend % divider)
    {
        throw invalid_argument(error_msg.data());
    }
}

void check_limit(std::uint64_t value, std::uint64_t limit,
    std::string_view error_msg)
{
    if (value >= limit)
    {
        throw out_of_range(error_msg.data());
    }
}

void check_ostream(const std::ofstream &os, std::string_view error_msg)
{
    if (!os)
    {
        throw runtime_error(error_msg.data());
    }
}

void check_istream(const std::ifstream &is, std::string_view error_msg)
{
    if (!is)
    {
        throw runtime_error(error_msg.data());
    }
}

} // namespace detail
} // namespace database
} // namespace addon