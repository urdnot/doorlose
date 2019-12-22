#include <doorlose/database.hpp>

#include <iostream>

int main()
{
    doorlose::database::task_base tb(0, 0);
    tb.serialize("path.db");

    std::cout << "Finish" << std::endl;
    return 0;
}