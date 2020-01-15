#include <addon/database/detail/choise_base.hpp>

#include <addon/database/detail/bitset.hpp>
#include <addon/database/detail/utils.hpp>

#include <cassert>

#include <fstream>
#include <stdexcept>

namespace addon {
namespace database {
namespace detail {
namespace {

struct serialization_header_t
{
    std::uint64_t mask_size;
    std::uint64_t mask_granularity;
    std::uint64_t mask_capacity;
    std::uint64_t record_size;
    std::uint64_t record_granularity;
    std::uint64_t record_capacity;
};

} // namespace

choise_base::choise_base()
{
}

choise_base::choise_base(const std::uint64_t start_mask_size,
    std::uint64_t mask_granularity, const std::uint64_t record_granularity)
{
    //
    // Checks
    //
    check_non_zero(mask_granularity, "`mask_granularity` should be positive number");
    check_non_zero(record_granularity, "`record_granularity` should be positive number");
    check_mult(mask_granularity, 64, "`mask_granularity` should be multiple by 64");

    //
    // Inits
    //
    mask_size_ = start_mask_size;
    mask_granularity_ = mask_granularity;
    mask_capacity_ = (start_mask_size / mask_granularity +
        (start_mask_size % mask_granularity) ? 1 : 0) * mask_granularity;
    record_size_ = 0;
    record_granularity_ = record_granularity;
    record_capacity_ = record_granularity;

    base_.resize((mask_capacity_ / 8) * record_capacity_);
}

std::uint64_t choise_base::size() const noexcept
{
    return record_size_;
}

std::uint64_t choise_base::capacity() const noexcept
{
    return record_capacity_;
}

std::uint64_t choise_base::mask_size() const noexcept
{
    return mask_size_;
}

std::uint64_t choise_base::mask_capacity() const noexcept
{
    return mask_capacity_;
}

std::uint8_t *choise_base::get_entry(std::uint64_t id)
{
    return base_.data() + (mask_capacity_ / 8) * id;
}

const std::uint8_t *choise_base::get_entry(std::uint64_t id) const
{
    return base_.data() + (mask_capacity_ / 8) * id;
}

std::uint64_t choise_base::add()
{
    if (record_size_ == record_capacity_)
    {
        expand(1);
    }

    return record_size_++;
}

std::uint64_t choise_base::choose(std::uint64_t id)
{
    check_limit(id, record_size_, "choise_base::choose(): Client index out of range");
    return bitset::get_random(get_entry(id), mask_size_);
}

void choise_base::increase_mask(std::uint64_t delta)
{
    if (mask_size_ + delta > mask_capacity_)
    {
        const std::uint64_t new_size = mask_size_ + delta;
        const std::uint64_t dlt = (new_size / mask_granularity_ +
            ((new_size % mask_granularity_) ? 1 : 0)) - (mask_capacity_ / mask_granularity_);
        expand_mask(dlt);
    }

    mask_size_ += delta;
}

void choise_base::serialize(const std::filesystem::path &to) const
{
    std::ofstream output(to, std::ios_base::out | std::ios_base::trunc);

    check_ostream(output, "choise_base::serialize(): cannot open output file for serialize");

    serialization_header_t header;
    header.mask_size = mask_size_;
    header.mask_granularity = mask_granularity_;
    header.mask_capacity = mask_capacity_;
    header.record_size = record_size_;
    header.record_granularity = record_granularity_;
    header.record_capacity = record_capacity_;

    output.write(reinterpret_cast<const char *>(&header), sizeof(serialization_header_t));
    check_ostream(output, "choise_base::serialize(): cannot serialize header");

    for (std::uint64_t i = 0; i < record_size_; ++i)
    {
        output.write(reinterpret_cast<const char *>(get_entry(i)), mask_capacity_);
        check_ostream(output, "choise_base::serialize(): cannot serialize client choise mask");
    }
}

void choise_base::deserialize(const std::filesystem::path &from)
{
    if (!std::filesystem::exists(from))
    {
        throw std::invalid_argument("choise_base::deserialize(): file for deserialize do not exist");
    }

    std::ifstream input(from);

    check_istream(input, "choise_base::deserialize(): cannot open input file for deserialize");

    serialization_header_t header;
    input.read(reinterpret_cast<char *>(&header), sizeof(serialization_header_t));
    check_istream(input, "choise_base::deserialize(): cannot deserialize header");

    //
    // Checks
    //
    
    check_non_zero(header.mask_granularity, "`mask_granularity` should be positive number");
    check_non_zero(header.record_granularity, "`record_granularity` should be positive number");
    check_mult(header.mask_granularity, 64, "`mask_granularity` should be multiple by 64");
    check_mult(header.mask_capacity, header.mask_granularity,
        "`mask_capacity` should be multiple by `mask_granularity`");
    check_mult(header.record_capacity, header.record_granularity,
        "`record_capacity` should be multiple by `record_granularity`");
    check_limit(header.mask_size, header.mask_capacity,
        "`mask_size` should be less then `mask_capacity`");
    check_limit(header.record_size, header.record_capacity,
        "`record_size` should be less then `record_capacity`");

    std::vector<std::uint8_t> base;
    base.resize(header.record_capacity * (header.mask_capacity / 8));

    for (std::uint64_t i = 0; i < header.record_size; ++i)
    {
        const auto entry_size = header.mask_capacity / 8;
        const auto entry = reinterpret_cast<char *>(base.data()) + i * entry_size;
        input.read(entry, entry_size);
        check_istream(input, "choise_base::deserialize(): cannot deserialize choise mask");
    }

    mask_size_ = header.mask_size;
    mask_granularity_ = header.mask_granularity;
    mask_capacity_ = header.mask_capacity;
    record_size_ = header.record_size;
    record_granularity_ = header.record_granularity;
    record_capacity_ = header.record_capacity;

    base_.swap(base);
}

void choise_base::expand(std::uint64_t delta)
{
    base_.resize(base_.size() +
        (mask_capacity_ / 8) * (record_capacity_ + delta * record_granularity_));
    record_capacity_ += delta * record_granularity_;
}

void choise_base::expand_mask(std::uint64_t delta)
{
    base_.resize(base_.size() +
        ((mask_capacity_ + delta * mask_granularity_) / 8) * record_capacity_);
    mask_capacity_ += delta * mask_granularity_;
}

} // namespace detail
} // namespace database
} // namespace addon