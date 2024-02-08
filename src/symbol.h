#ifndef SYMBOL_H
#define SYMBOL_H

#include <type_traits> // for std::type_identity, std::is_lvalue_reference_v, std::remove_reference_t, std::is_rvalue_reference_v<T>, std::conditional etc
#include <utility> // for std::forward, std::index_sequence_for

#include "symbol_id.h"
#include "utils.h"

namespace symbolic
{
    template <class Trait = unconstrained, auto Id = symbol_id<decltype([] {})>{}>
    struct symbol
    {
        // Unique identifier
        static constexpr auto id = Id;

        // Binding mechanism
        // clang-format off
    template <class Arg>
    requires Trait::template trait<std::remove_cvref_t<Arg>>::value
    constexpr symbol_binder<symbol, Arg &&> operator=(Arg &&arg) const
    {
      return symbol_binder(*this, std::forward<Arg>(arg));
    }
        // clang-format on

        template <class... Binders>
        constexpr auto operator()(const substitution<Binders...> &s) const
        {
            return s[id](); // <- Everything happens here
        }
    };

    // Specialization for variable symbol
    template <class T, auto Id>
    struct is_symbolic<symbol<T, Id>> : std::true_type
    {
    };

} // namespace symbolic

#endif // SYMBOL_H
