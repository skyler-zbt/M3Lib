// apply_binary / apply_unary — generic element-wise operation helpers.
// Dispatch is unrolled for dimension ∈ {1,2,3,4}.  The generic-loop
// else-branch exists for future types with higher dimensions (e.g. matrices
// with >4 columns), but is currently unreachable for Vec since
// ValidDimension restricts to [1, 4].
// These functions are parameterised on a VectorLike type (not a concrete
// Vec) so that the same dispatch works for vectors, matrices, quaternions,
// and any future types that expose dimension / value_type / operator[].
//
// apply_scalar_binary_left / apply_scalar_binary_right — scalar × vector hybrids.
// Reduce duplicated scalar-operator unrolling down to two helper functions
// plus single-line call sites.
//
// Architectural note: this module does not import m3.vector.vec.
// The dependency inversion is realised through the VectorLike concept.
//
// apply_binary / apply_unary —— 泛型逐元素操作辅助函数。
// 对维度 ∈ {1,2,3,4} 展开分派。通用循环 else 分支为未来更高维度的类型
// （如 >4 列的矩阵）预留，但因 ValidDimension 限制为 [1, 4]，目前对 Vec
// 不可达。
// 这些函数以 VectorLike 类型（而非具体 Vec）为参数，使同一分派逻辑能
// 作用于向量、矩阵、四元数，以及任何暴露 dimension / value_type / operator[] 的未来类型。
//
// apply_scalar_binary_left / apply_scalar_binary_right —— 标量×向量混合操作。
// 将重复的标量-运算符展开代码缩减为两个辅助函数加单行调用点。
//
// 架构说明：此模块不导入 m3.vector.vec。
// 通过 VectorLike 概念实现依赖倒置。
export module m3.detail.operations.apply;

import std;

import m3.detail.concepts;

export namespace m3::detail {

// Applies a binary functor element-wise to two VectorLike values.
//
// 对两个 VectorLike 值逐元素应用二元函数对象。
template <typename Op, VectorLike V>
requires BinaryOp<Op, typename V::value_type>
[[nodiscard("element-wise operation: discarding the result is likely a bug")]]
constexpr V apply_binary(const V& a, const V& b) {
    V result;
    constexpr int L = V::dimension;
    if constexpr (L == 1) {
        result[0] = Op{}(a[0], b[0]);
    } else if constexpr (L == 2) {
        result[0] = Op{}(a[0], b[0]);
        result[1] = Op{}(a[1], b[1]);
    } else if constexpr (L == 3) {
        result[0] = Op{}(a[0], b[0]);
        result[1] = Op{}(a[1], b[1]);
        result[2] = Op{}(a[2], b[2]);
    } else if constexpr (L == 4) {
        result[0] = Op{}(a[0], b[0]);
        result[1] = Op{}(a[1], b[1]);
        result[2] = Op{}(a[2], b[2]);
        result[3] = Op{}(a[3], b[3]);
    } else {
        for (int i = 0; i < L; ++i) {
            result[i] = Op{}(a[i], b[i]);
        }
    }
    return result;
}

// Applies a unary functor element-wise to a VectorLike value.
//
// 对 VectorLike 值逐元素应用一元函数对象。
template <typename Op, VectorLike V>
requires UnaryOp<Op, typename V::value_type>
[[nodiscard("element-wise operation: discarding the result is likely a bug")]]
constexpr V apply_unary(const V& a) {
    V result;
    constexpr int L = V::dimension;
    if constexpr (L == 1) {
        result[0] = Op{}(a[0]);
    } else if constexpr (L == 2) {
        result[0] = Op{}(a[0]);
        result[1] = Op{}(a[1]);
    } else if constexpr (L == 3) {
        result[0] = Op{}(a[0]);
        result[1] = Op{}(a[1]);
        result[2] = Op{}(a[2]);
    } else if constexpr (L == 4) {
        result[0] = Op{}(a[0]);
        result[1] = Op{}(a[1]);
        result[2] = Op{}(a[2]);
        result[3] = Op{}(a[3]);
    } else {
        for (int i = 0; i < L; ++i) {
            result[i] = Op{}(a[i]);
        }
    }
    return result;
}

// Applies Op{}(scalar, v[i]) — scalar on the left, vector on the right.
// Used by operator+(T, Vec) / operator-(T, Vec) / operator*(T, Vec) in vector.cppm.
//
// 应用 Op{}(scalar, v[i]) —— 标量在左，向量在右。
// 由 vector.cppm 中的 operator+(T, Vec) / operator-(T, Vec) / operator*(T, Vec) 调用。
template <typename Op, VectorLike V>
requires BinaryOp<Op, typename V::value_type>
[[nodiscard("element-wise operation: discarding the result is likely a bug")]]
constexpr V apply_scalar_binary_left(typename V::value_type s, const V& v) {
    V result;
    constexpr int L = V::dimension;
    if constexpr (L == 1) {
        result[0] = Op{}(s, v[0]);
    } else if constexpr (L == 2) {
        result[0] = Op{}(s, v[0]);
        result[1] = Op{}(s, v[1]);
    } else if constexpr (L == 3) {
        result[0] = Op{}(s, v[0]);
        result[1] = Op{}(s, v[1]);
        result[2] = Op{}(s, v[2]);
    } else if constexpr (L == 4) {
        result[0] = Op{}(s, v[0]);
        result[1] = Op{}(s, v[1]);
        result[2] = Op{}(s, v[2]);
        result[3] = Op{}(s, v[3]);
    } else {
        for (int i = 0; i < L; ++i) {
            result[i] = Op{}(s, v[i]);
        }
    }
    return result;
}

// Applies Op{}(v[i], scalar) — vector on the left, scalar on the right.
// Used by operator+(Vec, T) / operator-(Vec, T) / operator*(Vec, T) / operator/(Vec, T)
// in vector.cppm.
//
// 应用 Op{}(v[i], scalar) —— 向量在左，标量在右。
// 由 vector.cppm 中的 operator+(Vec, T) / operator-(Vec, T) /
// operator*(Vec, T) / operator/(Vec, T) 调用。
template <typename Op, VectorLike V>
requires BinaryOp<Op, typename V::value_type>
[[nodiscard("element-wise operation: discarding the result is likely a bug")]]
constexpr V apply_scalar_binary_right(const V& v, typename V::value_type s) {
    V result;
    constexpr int L = V::dimension;
    if constexpr (L == 1) {
        result[0] = Op{}(v[0], s);
    } else if constexpr (L == 2) {
        result[0] = Op{}(v[0], s);
        result[1] = Op{}(v[1], s);
    } else if constexpr (L == 3) {
        result[0] = Op{}(v[0], s);
        result[1] = Op{}(v[1], s);
        result[2] = Op{}(v[2], s);
    } else if constexpr (L == 4) {
        result[0] = Op{}(v[0], s);
        result[1] = Op{}(v[1], s);
        result[2] = Op{}(v[2], s);
        result[3] = Op{}(v[3], s);
    } else {
        for (int i = 0; i < L; ++i) {
            result[i] = Op{}(v[i], s);
        }
    }
    return result;
}

}  // namespace m3::detail
