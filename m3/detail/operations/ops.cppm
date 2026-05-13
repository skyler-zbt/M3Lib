export module m3.detail.operations.ops;

export namespace m3::detail {

    struct add { template<typename T> constexpr T operator()(T a, T b) const { return a + b; } };
    struct sub { template<typename T> constexpr T operator()(T a, T b) const { return a - b; } };
    struct mul { template<typename T> constexpr T operator()(T a, T b) const { return a * b; } };
    struct div { template<typename T> constexpr T operator()(T a, T b) const { return a / b; } };

    struct neg { template<typename T> constexpr T operator()(T a) const { return -a; } };

}
