#pragma once

#include <doorlose/database/detail/common.hpp>

#include <filesystem>
#include <vector>

namespace doorlose {
namespace database {
namespace detail {

/**
 *
 */
class DOORLOSE_DATABASE_EXPORT choise_base
{
public:
    /**
     *
     */
    explicit choise_base();

    /**
     *
     */
    explicit choise_base(std::uint64_t granularity);

    /**
     * Add client
     */
    std::uint64_t add();

    /**
     * Choose random task
     */
    std::uint64_t choose(std::uint64_t id);

    /**
     * Expand choise's masks
     */
    void expand(std::uint64_t delta);

    /**
     *
     */
    void serialize(const std::filesystem::path &to) const;

    /**
     *
     */
    void deserialize(const std::filesystem::path &from);

private:

};

} // namespace detail
} // namespace database
} // namespace doorlose