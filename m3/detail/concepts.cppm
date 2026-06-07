// Core concepts used across M3Lib for type constraints.
//   Arithmetic       — restricts to built-in arithmetic types (int, float, double, …)
//   FloatingPoint    — restricts to floating-point types (float, double, long double)
//   ValidDimension   — restricts vector/matrix dimensions to [1, 4]
//   VectorLike       — any type that behaves like a vector (has dimension, value_type, operator[])
//   element_ref_t<V> — reference type returned by v[i]; decouples apply_* from Vec-specific
//                       operator[] semantics, enabling future Matrix reuse
//   BinaryOp / UnaryOp — constrains functors used by apply_binary / apply_unary
//
// M3Lib 全局类型约束概念。
//   Arithmetic       —— 限定内置算术类型（int、float、double 等）
//   FloatingPoint    —— 限定浮点类型（float、double、long double）
//   ValidDimension   —— 限定向量/矩阵维度为 [1, 4]
//   VectorLike       —— 任何行为类似向量的类型（具有 dimension、value_type、operator[]）
//   element_ref_t<V> —— v[i] 返回的引用类型；将 apply_* 与 Vec 专属的 operator[]
//                       语义解耦，为未来 Matrix 复用做准备
//   BinaryOp / UnaryOp —— 约束 apply_binary / apply_unary 使用的函数对象
export module m3.detail.concepts;

import std;

export namespace m3::detail {
template <typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

template <typename T>
concept FloatingPoint = std::floating_point<T>;

template <int L>
concept ValidDimension = (L >= 1 && L <= 4);

// VectorLike: any type with dimension, value_type, qualifier_type,
// operator[], and default-constructibility.
// Enables apply_binary / apply_unary to work generically with Vec, Mat, Quat, etc.
//
// VectorLike：任何具有 dimension、value_type、qualifier_type、
// operator[] 和默认可构造性的类型。
// 使 apply_binary / apply_unary 能泛型作用于 Vec、Mat、Quat 等类型。
template <typename V>
concept VectorLike = requires {
    typename V::value_type;
    typename V::qualifier_type;
    { V::dimension } -> std::convertible_to<int>;
    requires std::default_initializable<V>;
    requires requires(V& v, const V& cv, std::size_t i) {
        { v[i] } -> std::same_as<typename V::value_type&>;
        { cv[i] } -> std::same_as<const typename V::value_type&>;
    };
};

// element_ref_t<V> — abstracts the reference type returned by v[i].
// For Vec<L,T>: this is T& (a scalar reference).
// For Mat<M,N,T>: this would be Vec<N,T>& (a column reference).
// This trait decouples apply_binary / apply_unary from the assumption that
// operator[] always returns a scalar reference, enabling Matrix to reuse
// the same generic dispatch infrastructure.
//
// element_ref_t<V> —— 抽象 v[i] 返回的引用类型。
// 对 Vec<L,T>：结果为 T&（标量引用）。
// 对 Mat<M,N,T>：结果为 Vec<N,T>&（列引用）。
// 此 trait 将 apply_binary / apply_unary 与 "operator[] 始终返回标量引用"
// 的假设解耦，使 Matrix 能复用相同的泛型分派基础设施。
template <typename V>
using element_ref_t = decltype(std::declval<V&>()[std::size_t{}]);

template <typename Op, typename T>
concept BinaryOp =
    std::regular_invocable<Op, T, T> && std::convertible_to<std::invoke_result_t<Op, T, T>, T>;

template <typename Op, typename T>
concept UnaryOp =
    std::regular_invocable<Op, T> && std::convertible_to<std::invoke_result_t<Op, T>, T>;
}  // namespace m3::detail
