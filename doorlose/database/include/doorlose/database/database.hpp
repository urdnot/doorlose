#pragma once

#include <doorlose/database/detail/common.hpp>

#include <filesystem>

namespace doorlose {
namespace database {

class DOORLOSE_DATABASE_EXPORT database
{
public:
    explicit database();


    void serialize(const std::filesystem::path &to_folder) const;
    void deserialize(const std::filesystem::path &from_folder);

private:
};

}
}