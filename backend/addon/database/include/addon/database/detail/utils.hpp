#pragma once

#include <addon/database/detail/common.hpp>

#include <cstdint>

#include <exception>
#include <fstream>
#include <string_view>

namespace addon {
namespace database {
namespace detail {

class ADDON_DATABASE_EXPORT exception : public std::exception
{
public:
    explicit exception(const char *msg)
        : msg_(msg)
    {
    }

    const char *what() const noexcept override
    {
        return msg_;
    }

private:
    const char *const msg_;
};

class ADDON_DATABASE_EXPORT invalid_argument : public exception
{
public:
    explicit invalid_argument(const char *msg)
        : exception(msg)
    {
    }
};

class ADDON_DATABASE_EXPORT out_of_range : public exception
{
public:
    explicit out_of_range(const char *msg)
        : exception(msg)
    {
    }
};

class ADDON_DATABASE_EXPORT runtime_error : public exception
{
public:
    explicit runtime_error(const char *msg)
        : exception(msg)
    {
    }
};

void check_non_zero(std::uint64_t value, std::string_view error_msg);

void check_mult(std::uint64_t dividend, std::uint64_t divider, std::string_view error_msg);

void check_limit(std::uint64_t value, std::uint64_t limit,
    std::string_view error_msg);

void check_ostream(const std::ofstream &os, std::string_view error_msg);

void check_istream(const std::ifstream &is, std::string_view error_msg);

} // namespace detail
} // namespace database
} // namespace addon