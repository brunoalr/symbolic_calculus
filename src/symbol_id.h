#ifndef SYMBOL_ID_H
#define SYMBOL_ID_H

#include <functional> // for std::compare_three_way etc
#include <memory> // for std::addressof

#include "utils.h"

namespace symbolic {
    template <class>
    struct symbol_id
    {
        static constexpr auto singleton = [] {};

        static constexpr const void *address = std::addressof(singleton);
    };

    template <class Lhs, class Rhs>
    constexpr std::strong_ordering operator<=>(symbol_id<Lhs>, symbol_id<Rhs>)
    {
        return std::compare_three_way{}(symbol_id<Lhs>::address, symbol_id<Rhs>::address);
    }
} // namespace symbolic

#endif //SYMBOL_ID_H
