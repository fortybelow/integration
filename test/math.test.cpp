#include "math.hpp"

#include <iostream>
#include <sstream>
#include <type_traits>

void test_math_impl()
{
    auto _string_buf = std::stringstream{};
    auto _prev_buf = std::cout.rdbuf(_string_buf.rdbuf());

    char name[] = "test_math_impl";

    shifting(1, (char**)(&name));

    std::cout << "Captured-------\n";
    std::cout << _string_buf.str() << "\n";
    std::cout << "---------------\n";
}
