#include <iostream>
#include "math.hpp"

void shifting([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    math::vec2f origin { 0.f, 0.f };


    for(const auto shift : { math::vec2f{ 1.f, 0.f },
                             math::vec2f{ 1.f, 1.f },
                             math::vec2f{ -1.f, -1.f },
                             math::vec2f{ -1.f, 0.f } })
    {
        auto origin_shifted = origin + shift;

        std::cout << "New Origin: " << origin_shifted << std::endl;
    }
}
