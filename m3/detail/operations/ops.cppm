
export module m3.detail.operations.ops;

import std;

export namespace m3::detail {

struct Add {
    template <typename T>
    constexpr T operator()(T a, T b) const noexcept {
        return a + b;
    }
};
struct Sub {
    template <typename T>
    constexpr T operator()(T a, T b) const noexcept {
        return a - b;
    }
};
struct Mul {
    template <typename T>
    constexpr T operator()(T a, T b) const noexcept {
        return a * b;
    }
};
struct Div {
    template <typename T>
    constexpr T operator()(T a, T b) const noexcept pre(b != T{0}) {

        if (b == T{0}) [[unlikely]] {
            std::abort();
        }
        return a / b;
    }
};

struct Neg {
    template <typename T>
    constexpr T operator()(T a) const noexcept {

        if constexpr (std::is_integral_v<T> && std::is_signed_v<T>) {
            if (a == std::numeric_limits<T>::lowest()) [[unlikely]] {
                std::abort();
            }
        }
        return -a;
    }
};

}
