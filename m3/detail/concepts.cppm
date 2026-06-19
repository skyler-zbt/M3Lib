// Core concepts used across M3Lib for type constraints.
//   Arithmetic         — restricts to built-in arithmetic types (int, float, double, …)
//   FloatingPoint      — restricts to floating-point types (float, double, long double)
//   ValidVecDimension  — restricts vector dimensions to [1, 4]
//   ValidMatrixSize    — restricts matrix rows and columns to [1, 4]
//   VectorLike         — any type that behaves like a vector (has dimension, value_type,
//   operator[]) MatrixLike         — any type that behaves like a matrix (has columns, rows,
//   column_type, operator[]) element_ref_t<V>   — reference type returned by v[i]; decouples
//   apply_* from Vec-specific
//                         operator[] semantics, enabling future Matrix reuse
//   BinaryOp / UnaryOp — constrains functors used by apply_binary / apply_unary
//
// M3Lib 全局类型约束概念。
//   Arithmetic         —— 限定内置算术类型（int、float、double 等）
//   FloatingPoint      —— 限定浮点类型（float、double、long double）
//   ValidVecDimension  —— 限定向量维度为 [1, 4]
//   ValidMatrixSize    —— 限定矩阵行和列维度为 [1, 4]
//   VectorLike         —— 任何行为类似向量的类型（具有 dimension、value_type、operator[]）
//   MatrixLike         —— 任何行为类似矩阵的类型（具有 columns、rows、column_type、operator[]）
//   element_ref_t<V>   —— v[i] 返回的引用类型；将 apply_* 与 Vec 专属的 operator[]
//                         语义解耦，为未来 Matrix 复用做准备
//   BinaryOp / UnaryOp —— 约束 apply_binary / apply_unary 使用的函数对象
export module m3.detail.concepts;

import std;

export namespace m3::detail {
template <typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

template <typename T>
concept FloatingPoint = std::floating_point<T>;

template <int L>
concept ValidVecDimension = (L >= 1 && L <= 4);

// VectorLike: any type with dimension, value_type, qualifier_type,
// operator[], and default-constructibility.
// Enables apply_binary / apply_unary to work generically with vector types.
//
// VectorLike：任何具有 dimension、value_type、qualifier_type、
// operator[] 和默认可构造性的类型。
// 使 apply_binary / apply_unary 能泛型作用于向量类型。
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

// MatrixLike: any type with columns, rows, column_type, value_type,
// qualifier_type, operator[] returning a column reference, and default-
// constructibility.
//
// MatrixLike：任何具有 columns、rows、column_type、value_type、
// qualifier_type、operator[] 返回列引用和默认可构造性的类型。
template <typename M>
concept MatrixLike = requires {
    typename M::value_type;
    typename M::column_type;
    requires std::same_as<typename M::column_type::value_type, typename M::value_type>;
    typename M::qualifier_type;
    requires std::same_as<decltype(M::columns), const int>;
    requires std::same_as<decltype(M::rows), const int>;
    requires std::default_initializable<M>;
    requires Arithmetic<typename M::value_type>;
    requires requires(M& m, const M& cm, std::size_t i) {
        { m[i] } -> std::same_as<typename M::column_type&>;
        { cm[i] } -> std::same_as<const typename M::column_type&>;
    };
};

// ValidMatrixSize — restricts matrix rows and columns to [2, 4], excluding
// degenerate 1×N / N×1 matrices (TD-012).  Square constraint is enforced by
// individual Mat specialisations, not by this concept.
//
// ValidMatrixSize —— 将矩阵行和列维度限制为 [2, 4]，排除退化的
// 1×N / N×1 矩阵（TD-012）。方阵约束由各 Mat 特化强制执行，
// 不由此 concept 负责。
template <int C, int R>
concept ValidMatrixSize = (C >= 2 && C <= 4) && (R >= 2 && R <= 4);

// element_ref_t<V> — abstracts the reference type returned by v[i].
// Akin conceptually to std::iterator_traits::iter_reference_t but for
// vector-like types.
// For Vec<L,T>: this is T (a scalar, not a reference — remove_reference_t strips the &).
// For const Vec<L,T>: this is const T& (const scalar reference).
// For Mat<M,N,T>: this would be Vec<N,T>& (a column reference).
// remove_reference_t is used so the trait also works when V is const.
// This trait decouples apply_binary / apply_unary from the assumption that
// operator[] always returns a scalar reference, enabling Matrix to reuse
// the same generic dispatch infrastructure.
//
// element_ref_t<V> —— 抽象 v[i] 返回的引用类型。
// 概念上类似于 std::iterator_traits::iter_reference_t，但用于向量式类型。
// 对 Vec<L,T>：结果为 T&（标量引用）。
// 对 const Vec<L,T>：结果为 const T&（const 标量引用）。
// 对 Mat<M,N,T>：结果为 Vec<N,T>&（列引用）。
// 使用 remove_reference_t 使 trait 在 V 为 const 时也能工作。
// 此 trait 将 apply_binary / apply_unary 与 "operator[] 始终返回标量引用"
// 的假设解耦，使 Matrix 能复用相同的泛型分派基础设施。
template <typename V>
using element_ref_t = std::remove_reference_t<decltype(std::declval<V&>()[std::size_t{}])>;

template <typename Op, typename T>
concept BinaryOp =
    std::regular_invocable<Op, T, T> && std::convertible_to<std::invoke_result_t<Op, T, T>, T>;

template <typename Op, typename T>
concept UnaryOp =
    std::regular_invocable<Op, T> && std::convertible_to<std::invoke_result_t<Op, T>, T>;
}  // namespace m3::detail
