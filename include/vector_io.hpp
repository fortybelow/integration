#ifndef MATH_VECTOR_IO_HPP
#define MATH_VECTOR_IO_HPP

#include <ostream>

#include "vector.hpp"
#include "vector_io_details.hpp"

namespace math
{
    template <class Value, size_t Dimension>
    std::ostream& operator<<(std::ostream& os, const vec<Value, Dimension>& arg)
    {
        os << math::detail::Left << " ";
        for(const auto& value : arg.coordinates)
            os << value << ' ';
        return os << math::detail::Right;
    }
}

#endif
