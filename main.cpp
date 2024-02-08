#include <cmath>
#include <iostream>

namespace symbolic
{
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

    // Index constant type
    template <std::size_t I>
    struct index_constant : std::integral_constant<std::size_t, I>
    {
    };

    // Index constant variable template
    template <std::size_t I>
    inline constexpr index_constant<I> index = {};

    // An indexed substitution element
    template <std::size_t I, class  B>
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

    template <auto Value>
    struct constant_symbol
    {
        using type = decltype(Value);

        static constexpr type value = Value;

        template <class... Binders>
        constexpr auto operator()(const substitution<Binders...> &) const
        {
            return value;
        }
    };

    // Making it symbolic
    template <auto Value>
    struct is_symbolic<constant_symbol<Value>> : std::true_type
    {
    };

    // The class for symbolic expressions
    template <class Operator, symbolic... Terms>
    struct symbolic_expression
    {
        template <class... Binders>
        constexpr auto operator()(const substitution<Binders...> &s) const noexcept
        {
            return Operator{}(Terms{}(s)...); // <- Everything happens here
        }
    };

    // Making it symbolic
    template <class Operator, symbolic... Terms>
    struct is_symbolic<symbolic_expression<Operator, Terms...>> : std::true_type
    {
    };

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

    template <symbolic Lhs, symbolic Rhs>
    constexpr symbolic_expression<std::plus<>, Lhs, Rhs> operator+(Lhs, Rhs)
    {
        return {};
    }

    template <symbolic Lhs, symbolic Rhs>
    constexpr symbolic_expression<std::minus<>, Lhs, Rhs> operator-(Lhs, Rhs)
    {
        return {};
    }

    template <symbolic Lhs, symbolic Rhs>
    constexpr symbolic_expression<std::multiplies<>, Lhs, Rhs> operator*(Lhs, Rhs)
    {
        return {};
    }

    template <symbolic Lhs, symbolic Rhs>
    constexpr symbolic_expression<std::divides<>, Lhs, Rhs> operator/(Lhs, Rhs)
    {
        return {};
    }

    // Define function object
    struct sin_symbol
    {
        template <class Arg>
        constexpr auto operator()(Arg &&arg) const
        {
            return std::sin(std::forward<Arg>(arg));
        }
    };

    // Function builder
    template <symbolic Arg>
    constexpr symbolic_expression<sin_symbol, Arg> sin(Arg) noexcept
    {
        return {};
    }

    // Define function object
    struct exp_symbol
    {
        template <class Arg>
        constexpr auto operator()(Arg &&arg) const
        {
            return std::exp(std::forward<Arg>(arg));
        }
    };

    // Function builder
    template <symbolic Arg>
    constexpr symbolic_expression<exp_symbol, Arg> exp(Arg) noexcept
    {
        return {};
    }

    // Specialization for variable symbol
    template <class T, auto Id>
    struct is_symbolic<symbol<T, Id>> : std::true_type
    {
    };

} // namespace symbolic

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
