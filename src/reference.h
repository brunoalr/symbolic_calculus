#ifndef REFERENCE_H
#define REFERENCE_H

#include <type_traits> // for std::type_identity, std::is_lvalue_reference_v, std::remove_reference_t, std::is_rvalue_reference_v<T>, std::conditional etc

namespace symbolic {
    template <class T>
    struct remove_lvalue_reference : std::type_identity<T>
    {
    };

    template <class T>
        requires std::is_lvalue_reference_v<T>
    struct remove_lvalue_reference<T> : std::type_identity<std::remove_reference_t<T>>
    {
    };

    template <class T>
    using remove_lvalue_reference_t = typename remove_lvalue_reference<T>::type;

    template <class T>
    struct remove_rvalue_reference : std::type_identity<T>
    {
    };

    template <class T>
        requires std::is_rvalue_reference_v<T>
    struct remove_rvalue_reference<T> : std::type_identity<std::remove_reference_t<T>>
    {
    };

    template <class T>
    using remove_rvalue_reference_t = typename remove_rvalue_reference<T>::type;

    template <class T>
    struct requalify_as_const
        : std::conditional<std::is_lvalue_reference_v<T>,
                           std::add_lvalue_reference_t<std::add_const_t<std::remove_reference_t<T>>>,
                           std::conditional_t<std::is_rvalue_reference_v<T>,
                                              std::add_rvalue_reference_t<std::add_const_t<std::remove_reference_t<T>>>,
                                              std::add_const_t<T>>>
    {
    };

    template <class T>
    using requalify_as_const_t = typename requalify_as_const<T>::type;

    template <class T>
    struct requalify_as_volatile;

    template <class T>
    using requalify_as_volatile_t = typename requalify_as_volatile<T>::type;

    template <class T>
    struct requalify_as_cv;

    template <class T>
    using requalify_as_cv_t = typename requalify_as_cv<T>::type;
} // namespace symbolic

#endif //REFERENCE_H
