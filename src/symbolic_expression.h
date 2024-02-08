#ifndef SYMBOLIC_EXPRESSION_H
#define SYMBOLIC_EXPRESSION_H

#include <cmath>
#include <utility> // for std::forward, std::index_sequence_for

#include "substitution.h"
#include "traits.h"

namespace symbolic
{
    // The class for symbolic expressions
    template <class Operator, symbolic... Terms>
    struct symbolic_expression
    {
        template <class... Binders>
        constexpr auto operator()(const substitution<Binders...> &s) const noexcept
        {
            return Operator{}(Terms{}(s)...); // <- Everything happens here
        }
    };

    // Making it symbolic
    template <class Operator, symbolic... Terms>
    struct is_symbolic<symbolic_expression<Operator, Terms...>> : std::true_type
    {
    };

    template <symbolic Lhs, symbolic Rhs>
    constexpr symbolic_expression<std::plus<void>, Lhs, Rhs> operator+(Lhs, Rhs)
    {
        return {};
    }

    template <symbolic Lhs, symbolic Rhs>
    constexpr symbolic_expression<std::minus<void>, Lhs, Rhs> operator-(Lhs, Rhs)
    {
        return {};
    }

    template <symbolic Lhs, symbolic Rhs>
    constexpr symbolic_expression<std::multiplies<void>, Lhs, Rhs> operator*(Lhs, Rhs)
    {
        return {};
    }

    template <symbolic Lhs, symbolic Rhs>
    constexpr symbolic_expression<std::divides<void>, Lhs, Rhs> operator/(Lhs, Rhs)
    {
        return {};
    }

    // Define function object
    struct sin_symbol
    {
        template <class Arg>
        constexpr auto operator()(Arg &&arg) const
        {
            return std::sin(std::forward<Arg>(arg));
        }
    };

    // Function builder
    template <symbolic Arg>
    constexpr symbolic_expression<sin_symbol, Arg> sin(Arg) noexcept
    {
        return {};
    }

    // Define function object
    struct exp_symbol
    {
        template <class Arg>
        constexpr auto operator()(Arg &&arg) const
        {
            return std::exp(std::forward<Arg>(arg));
        }
    };

    // Function builder
    template <symbolic Arg>
    constexpr symbolic_expression<exp_symbol, Arg> exp(Arg) noexcept
    {
        return {};
    }

    template <auto Value>
    struct constant_symbol
    {
        using type = decltype(Value);

        static constexpr type value = Value;

        template <class... Binders>
        constexpr auto operator()(const substitution<Binders...> &) const
        {
            return value;
        }
    };

    // Making it symbolic
    template <auto Value>
    struct is_symbolic<constant_symbol<Value>> : std::true_type
    {
    };
} // namespace symbolic

#endif // SYMBOLIC_EXPRESSION_H
