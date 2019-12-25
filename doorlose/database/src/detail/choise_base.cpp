#include <doorlose/database/detail/choise_base.hpp>

namespace doorlose {
namespace database {
namespace detail {

choise_base::choise_base()
{
}

choise_base::choise_base(std::uint64_t mask_granularity)
{
}

std::uint64_t choise_base::add()
{
    return 0;
}

std::uint64_t choise_base::choose(std::uint64_t id)
{
    return 0;
}

void choise_base::expand(std::uint64_t delta)
{
}

void choise_base::serialize(const std::filesystem::path &to) const
{
}

void choise_base::deserialize(const std::filesystem::path &from)
{
}

} // namespace detail
} // namespace database
} // namespace doorlose