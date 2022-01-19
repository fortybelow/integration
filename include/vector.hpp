#ifndef MATH_VECTOR_HPP
#define MATH_VECTOR_HPP

#include <cmath>
#include <array>

namespace math
{
    template <size_t Store>
    struct store
    {
        static constexpr size_t value = Store;
    };

    template <class Value, size_t Dimension>
    struct vec
    {
        using value_type = Value;
        using query_dimension_type = store<Dimension>;
        using array_type = std::array<value_type, query_dimension_type::value>;

        array_type coordinates;

        value_type& operator[](size_t index) { return coordinates[index]; }
        const value_type& operator[](size_t index) const { return coordinates[index]; }


    };

    // TODO: Specialize for vec2,3,4 int,float,double

    template <class Value, size_t Dimension>
    vec<Value, Dimension>& operator+=(vec<Value, Dimension>& lhs, const vec<Value, Dimension>& rhs) noexcept
    {
        for(size_t i = 0; i < Dimension; ++i)
            lhs[i] += rhs[i];

        return lhs;
    }

    template <class Value, size_t Dimension>
    vec<Value, Dimension> operator+(const vec<Value, Dimension>& lhs, const vec<Value, Dimension>& rhs) noexcept
    {
        vec<Value, Dimension> sum{ lhs };
        return sum += rhs;
    }

    using vec2f = vec<float, 2ull>;
    using vec2i = vec<int, 2ull>;
    using vec2u = vec<unsigned, 2ull>;
}

#endif
