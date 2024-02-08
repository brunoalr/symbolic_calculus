#ifndef TRAITS_H
#define TRAITS_H

#include <type_traits> // for std::type_identity, std::is_lvalue_reference_v, std::remove_reference_t, std::is_rvalue_reference_v<T>, std::conditional etc

namespace symbolic
{   // Type trait
    template <class>
    struct is_symbolic : std::false_type
    {
    };

    // Variable template
    template <class T>
    inline constexpr bool is_symbolic_v = is_symbolic<T>::value;

    // Concept
    template <class T>
    concept symbolic = is_symbolic_v<T>;
} // namespace symbolic

#endif // TRAITS_H
