#ifndef SYMBOL_BINDER_H
#define SYMBOL_BINDER_H

#include <type_traits> // for std::type_identity, std::is_lvalue_reference_v, std::remove_reference_t, std::is_rvalue_reference_v<T>, std::conditional etc

#include "reference.h"
#include "utils.h"

namespace symbolic
{
    template <class Symbol, class T>
    struct symbol_binder
    {
        // Types and constants
        using symbol_type = Symbol;

        using value_type = std::remove_cvref_t<T>;

        static constexpr Symbol symbol = {};

        // Constructors
        template <class U>
            requires std::is_convertible_v<U &&, requalify_as_const_t<remove_rvalue_reference_t<T>>>
        constexpr symbol_binder(Symbol, U &&x) noexcept(
            std::is_nothrow_convertible_v<U &&, requalify_as_const_t<remove_rvalue_reference_t<T>>>) :
            value(std::forward<U>(x))
        {
        }

        // Accessors
        const value_type &operator()() const noexcept { return value; }

        // Implementation details: data members
    private:
        requalify_as_const_t<remove_rvalue_reference_t<T>> value;
    };

    // Deduction guide
    template <class Symbol, class T>
    symbol_binder(Symbol, T &&) -> symbol_binder<Symbol, T &&>;
} // namespace symbolic

#endif //SYMBOL_BINDER_H
