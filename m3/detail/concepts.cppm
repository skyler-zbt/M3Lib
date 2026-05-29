// Core concepts used across M3Lib for type constraints.
//   Arithmetic      — restricts to built-in arithmetic types (int, float, double, …)
//   ValidDimension  — restricts vector/matrix dimensions to [1, 4]
//   BinaryOp / UnaryOp — constrains functors used by apply_binary / apply_unary
export module m3.detail.concepts;

import std;

export namespace m3::detail {
    template <typename T>
    concept Arithmetic = std::is_arithmetic_v<T>;

    template <int L>
    concept ValidDimension = (L >= 1 && L<=4);

    template<typename Op, typename T>
    concept BinaryOp = std::regular_invocable<Op, T, T> && std::convertible_to<std::invoke_result_t<Op, T, T>, T>;

    template<typename Op, typename T>
    concept UnaryOp = std::regular_invocable<Op, T> && std::convertible_to<std::invoke_result_t<Op, T>, T>;
}