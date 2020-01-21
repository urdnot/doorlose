#pragma once

#include <addon/database/detail/common.hpp>
#include <addon/database/types.hpp>

#include <cstdint>

#include <fstream>
#include <string_view>

namespace addon {
namespace database {
namespace detail {

void check_non_zero(std::uint64_t value, std::string_view error_msg);

void check_mult(std::uint64_t dividend, std::uint64_t divider, std::string_view error_msg);

void check_limit(std::uint64_t value, std::uint64_t limit,
    std::string_view error_msg);

void check_ostream(const std::ofstream &os, std::string_view error_msg);

void check_istream(const std::ifstream &is, std::string_view error_msg);

} // namespace detail
} // namespace database
} // namespace addon