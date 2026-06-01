// Element-wise arithmetic functors.
// Each is a stateless callable used by apply_binary / apply_unary.
// Functors (rather than lambdas) are used because they can be passed as
// non-type template parameters — lambdas are not allowed in that context
// before C++20, and even in C++20+ each lambda has a unique type,
// complicating template argument deduction.
export module m3.detail.operations.ops;

export namespace m3::detail {

    struct add { template<typename T> constexpr T operator()(T a, T b) const { return a + b; } };
    struct sub { template<typename T> constexpr T operator()(T a, T b) const { return a - b; } };
    struct mul { template<typename T> constexpr T operator()(T a, T b) const { return a * b; } };
    struct div { template<typename T> constexpr T operator()(T a, T b) const { return a / b; } };

    struct neg { template<typename T> constexpr T operator()(T a) const { return -a; } };

}
