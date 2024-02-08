#ifndef UTILS_H
#define UTILS_H

#include <functional> // for std::plus etc
#include <type_traits> // for std::type_identity, std::is_lvalue_reference_v, std::remove_reference_t, std::is_rvalue_reference_v<T>, std::conditional etc
#include <utility> // for std::forward, std::index_sequence_for

namespace symbolic
{
    struct unconstrained
    {
        template <class T>
        struct trait : std::true_type
        {
        };
    };

    struct real
    {
        template <class T>
        struct trait : std::is_floating_point<T>
        {
        };
    };

    // Type trait
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

#endif //UTILS_H
