#ifndef FORMULA_H
#define FORMULA_H

#include "substitution.h"
#include "utils.h"

namespace symbolic
{
    template <symbolic Expression>
    struct formula
    {
        // Types and constants
        using expression = Expression;

        // Constructors
        constexpr formula(Expression) noexcept {}

        // Call operator where substitution happens
        template <class... Args>
        constexpr auto operator()(Args... args) const noexcept
        {
            return expression{}(substitution(args...));
        }
    };
} // namespace symbolic

#endif // FORMULA_H
