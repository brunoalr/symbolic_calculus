#ifndef CONSTRAINTS_H
#define CONSTRAINTS_H

#include <type_traits> // for std::type_identity, std::is_lvalue_reference_v, std::remove_reference_t, std::is_rvalue_reference_v<T>, std::conditional etc

namespace symbolic {
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
} // namespace symbolic

#endif //CONSTRAINTS_H
