#include <doorlose/database/detail/task_base.hpp>

#include <iostream>

int main()
{
    doorlose::database::detail::task_base tb(0, 0);
    tb.serialize("path.db");

    std::cout << "Finish" << std::endl;
    return 0;
}