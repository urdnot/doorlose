#include <doorlose/database/detail/choise_base.hpp>

namespace doorlose {
namespace database {
namespace detail {

choise_base::choise_base()
{
}

choise_base::choise_base(const std::uint64_t start_mask_size,
    std::uint64_t mask_granularity, const std::uint64_t record_granularity)
{
}

std::uint64_t choise_base::size() const noexcept
{
    return 0;
}

std::uint64_t choise_base::capacity() const noexcept
{
    return 0;
}

std::uint64_t choise_base::mask_size() const noexcept
{
    return 0;
}

std::uint64_t choise_base::mask_capacity() const noexcept
{
    return 0;
}

std::uint64_t choise_base::add()
{
    return 0;
}

std::uint64_t choise_base::choose(std::uint64_t id)
{
    return 0;
}

void choise_base::increase_mask(std::uint64_t delta)
{
}

void choise_base::serialize(const std::filesystem::path &to) const
{
}

void choise_base::deserialize(const std::filesystem::path &from)
{
}

void choise_base::expand(std::uint64_t delta)
{
}

void choise_base::expand_mask(std::uint64_t delta)
{
}

} // namespace detail
} // namespace database
} // namespace doorlose