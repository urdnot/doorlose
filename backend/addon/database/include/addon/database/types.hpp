#pragma once

#include <addon/database/detail/common.hpp>

#include <exception>

namespace addon {
namespace database {

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

class ADDON_DATABASE_EXPORT runtime_error : public exception
{
public:
    explicit runtime_error(const char *msg)
        : exception(msg)
    {
    }
};

} // namespace database
} // namespace addon