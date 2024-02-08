#include <iostream>

#include "formula.h"
#include "symbol.h"
#include "symbolic_expression.h"

using namespace symbolic;

int main()
{
    constexpr symbol<real> a;
    constexpr symbol<real> w;
    constexpr symbol<real> t;
    constexpr symbol<real> phi;

    constexpr formula f = a * sin(w * t + phi);
    const auto y = f(a = 5.0, w = 2.5, t = 1.6, phi = 0.0);

    constexpr formula f2 = exp(a);
    const double y2 = f2(a = y);

    std::cout << y << std::endl;
    std::cout << y2 << std::endl;

    return 0;
}
