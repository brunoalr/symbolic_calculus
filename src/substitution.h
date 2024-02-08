#ifndef SUBSTITUTION_H
#define SUBSTITUTION_H

#include <type_traits> // for std::type_identity, std::is_lvalue_reference_v, std::remove_reference_t, std::is_rvalue_reference_v<T>, std::conditional etc
#include <utility> // for std::forward, std::index_sequence_for

namespace symbolic
{
    // Index constant type
    template <std::size_t I>
    struct index_constant : std::integral_constant<std::size_t, I>
    {
    };

    // Index constant variable template
    template <std::size_t I>
    inline constexpr index_constant<I> index = {};

    // An indexed substitution element
    template <std::size_t I, class B>
    struct substitution_element
    {
        // Types and constants
        using index = index_constant<I>;

        using id_type = decltype(B::symbol_type::id);

        // Constructor
        explicit constexpr substitution_element(const B &b) : _binder(b) {}

        // Access by index
        constexpr const B &operator[](index) const { return _binder; }

        // Acess by id: all the magic happens here
        constexpr const B &operator[](id_type) const { return _binder; }

    private:
        const B _binder;
    };

    // A helper class to build the substitution
    template <class Sequence, class... T>
    struct substitution_base;

    // The substitution wrapper itself
    template <class... Binders>
    struct substitution;

    // Substitution providing an index sequence to its base
    template <class... Binders>
    struct substitution : substitution_base<std::index_sequence_for<Binders...>, Binders...>
    {
        using base = substitution_base<std::index_sequence_for<Binders...>, Binders...>;

        using base::base;

        using base::operator[];
    };

    // Deduction guide
    template <class... Binders>
    substitution(const Binders &...) -> substitution<Binders...>;

    // Substitution base deriving from each binder
    template <std::size_t... Index, class... Binders>
    struct substitution_base<std::index_sequence<Index...>, Binders...> : substitution_element<Index, Binders>...
    {
        using index_sequence = std::index_sequence<Index...>;

        using substitution_element<Index, Binders>::operator[]...;

        constexpr explicit substitution_base(const Binders &...x) : substitution_element<Index, Binders>(x)... {}
    };
} // namespace symbolic

#endif // SUBSTITUTION_H
