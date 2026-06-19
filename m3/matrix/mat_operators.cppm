// Mat operators — element-wise (+, -, /) and matrix products (Mat*Mat, Mat*Vec, Vec*Mat).
// Per GLSL convention (followed by glm and most graphics math libraries):
//   - operator+(Mat, Mat), operator-(Mat, Mat), operator/(Mat, Mat)  → Hadamard
//   - operator*(Mat, Mat)                                            → matrix product
//   - operator*(Mat, Vec)                                            → column vector
//   right-multiplication
//   - operator*(Vec, Mat)                                            → row vector
//   left-multiplication
//   - operator*(Mat, scalar), operator*(scalar, Mat)                 → scalar broadcast
//
// Per TD-005: both operator*(Mat, Vec) and operator*(Vec, Mat) are
// provided in this module.
//
// Per TD-007:
//   - Element-wise operators use dedicated per-column helpers (NOT
//     apply_binary, because Mat is MatrixLike, not VectorLike).
//   - Matrix products use dedicated per-size helpers (NOT apply_*, because
//     MM/MV/VM are structured products, not element-wise binary ops).
//
// The deviation from TD-007 (which suggested a MatrixLike apply_binary
// overload) is documented here.  Reason: apply_binary's VectorLike
// constraint requires operator[] returning value_type&.  Mat's operator[]
// returns Vec<R,T,Q> by value (flat storage), which violates this
// constraint.  Adding a MatrixLike overload to apply_binary would force
// every vector dispatch through an if-constexpr branch, slowing the hot
// path.
//
// Mat 运算符——逐元素（+、-、/）和矩阵乘法（Mat*Mat、Mat*Vec、Vec*Mat）。
// 按 GLSL 惯例（glm 和大多数图形数学库沿用）：
//   - operator+(Mat, Mat)、operator-(Mat, Mat)、operator/(Mat, Mat)  → Hadamard
//   - operator*(Mat, Mat)                                             → 矩阵乘法
//   - operator*(Mat, Vec)                                             → 列向量右乘
//   - operator*(Vec, Mat)                                             → 行向量左乘
//   - operator*(Mat, scalar)、operator*(scalar, Mat)                  → 标量广播
//
// 按 TD-005：本模块同时提供 operator*(Mat, Vec) 和 operator*(Vec, Mat)。
//
// 按 TD-007：
//   - 逐元素运算符使用专门的逐列辅助函数（不用 apply_binary，
//     因为 Mat 是 MatrixLike，不是 VectorLike）
//   - 矩阵乘法使用专门的按尺寸辅助函数（不用 apply_*，因为 MM/MV/VM
//     是结构化乘法，不是逐元素二元运算）
//
// 偏离 TD-007（其建议 MatrixLike apply_binary 重载）的原因在此说明：
// apply_binary 的 VectorLike 约束要求 operator[] 返回 value_type&。
// Mat 的 operator[] 按值返回 Vec<R,T,Q>（扁平存储），违反该约束。
// 给 apply_binary 加 MatrixLike 重载会强制每个向量分派走
// if-constexpr 分支，拖慢热路径。
export module m3.matrix:operators;

import std;

import m3.detail;
import m3.matrix.base;
import m3.matrix.mat;
import m3.vector.vec;

export namespace m3 {

// ============================================================
//  Element-wise (Hadamard) operators — reuse Add/Sub/Div functors
//  Per GLSL: + - / are element-wise on matrices (no * for Hadamard;
//  * is reserved for matrix product)
// ============================================================

namespace detail {

// Element-wise: result[c][r] = Op{}(a[c][r], b[c][r])
template <typename Op, int C, int R, typename T, Qualifier Q>
[[nodiscard]]
constexpr Mat<C, R, T, Q> mat_hadamard(const Mat<C, R, T, Q>& a, const Mat<C, R, T, Q>& b) {
    Mat<C, R, T, Q> result;
    for (std::size_t c = 0; c < static_cast<std::size_t>(C); ++c) {
        for (std::size_t r = 0; r < static_cast<std::size_t>(R); ++r) {
            result(c, r) = Op{}(a(c, r), b(c, r));
        }
    }
    return result;
}

// Scalar broadcast: result[c][r] = Op{}(s, a[c][r]) or Op{}(a[c][r], s)
template <typename Op, int C, int R, typename T, Qualifier Q>
[[nodiscard]]
constexpr Mat<C, R, T, Q> mat_scalar_left(T s, const Mat<C, R, T, Q>& a) {
    Mat<C, R, T, Q> result;
    for (std::size_t c = 0; c < static_cast<std::size_t>(C); ++c) {
        for (std::size_t r = 0; r < static_cast<std::size_t>(R); ++r) {
            result(c, r) = Op{}(s, a(c, r));
        }
    }
    return result;
}

template <typename Op, int C, int R, typename T, Qualifier Q>
[[nodiscard]]
constexpr Mat<C, R, T, Q> mat_scalar_right(const Mat<C, R, T, Q>& a, T s) {
    Mat<C, R, T, Q> result;
    for (std::size_t c = 0; c < static_cast<std::size_t>(C); ++c) {
        for (std::size_t r = 0; r < static_cast<std::size_t>(R); ++r) {
            result(c, r) = Op{}(a(c, r), s);
        }
    }
    return result;
}

// Unary negation
template <int C, int R, typename T, Qualifier Q>
[[nodiscard]]
constexpr Mat<C, R, T, Q> mat_neg(const Mat<C, R, T, Q>& a) {
    Mat<C, R, T, Q> result;
    for (std::size_t c = 0; c < static_cast<std::size_t>(C); ++c) {
        for (std::size_t r = 0; r < static_cast<std::size_t>(R); ++r) {
            result(c, r) = -a(c, r);
        }
    }
    return result;
}

}  // namespace detail

// ---- Element-wise + - / (Mat, Mat) — NO Hadamard * (GLSL convention) ----

template <int C, int R, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard]]
constexpr Mat<C, R, T, Q> operator+(const Mat<C, R, T, Q>& a, const Mat<C, R, T, Q>& b) noexcept {
    return detail::mat_hadamard<detail::Add>(a, b);
}

template <int C, int R, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard]]
constexpr Mat<C, R, T, Q> operator-(const Mat<C, R, T, Q>& a, const Mat<C, R, T, Q>& b) noexcept {
    return detail::mat_hadamard<detail::Sub>(a, b);
}

template <int C, int R, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard]]
constexpr Mat<C, R, T, Q> operator/(const Mat<C, R, T, Q>& a, const Mat<C, R, T, Q>& b) noexcept {
    return detail::mat_hadamard<detail::Div>(a, b);
}

template <int C, int R, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard]]
constexpr Mat<C, R, T, Q> operator-(const Mat<C, R, T, Q>& a) noexcept {
    return detail::mat_neg(a);
}

// ---- Scalar broadcast operators ----
// Per GLSL: scalar * matrix and matrix * scalar both broadcast.
// matrix / scalar also broadcasts.

template <int C, int R, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard]]
constexpr Mat<C, R, T, Q> operator*(const Mat<C, R, T, Q>& a, T s) noexcept {
    return detail::mat_scalar_right<detail::Mul>(a, s);
}

template <int C, int R, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard]]
constexpr Mat<C, R, T, Q> operator*(T s, const Mat<C, R, T, Q>& a) noexcept {
    return detail::mat_scalar_left<detail::Mul>(s, a);
}

template <int C, int R, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard]]
constexpr Mat<C, R, T, Q> operator/(const Mat<C, R, T, Q>& a, T s) noexcept {
    return detail::mat_scalar_right<detail::Div>(a, s);
}

template <int C, int R, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard]]
constexpr Mat<C, R, T, Q> operator+(const Mat<C, R, T, Q>& a, T s) noexcept {
    return detail::mat_scalar_right<detail::Add>(a, s);
}

template <int C, int R, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard]]
constexpr Mat<C, R, T, Q> operator+(T s, const Mat<C, R, T, Q>& a) noexcept {
    return detail::mat_scalar_left<detail::Add>(s, a);
}

template <int C, int R, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard]]
constexpr Mat<C, R, T, Q> operator-(const Mat<C, R, T, Q>& a, T s) noexcept {
    return detail::mat_scalar_right<detail::Sub>(a, s);
}

// ---- Compound assignment ----

template <int C, int R, detail::Arithmetic T, detail::Qualifier Q>
constexpr Mat<C, R, T, Q>& operator+=(Mat<C, R, T, Q>& a, const Mat<C, R, T, Q>& b) noexcept {
    for (std::size_t c = 0; c < static_cast<std::size_t>(C); ++c)
        for (std::size_t r = 0; r < static_cast<std::size_t>(R); ++r)
            a(c, r) += b(c, r);
    return a;
}

template <int C, int R, detail::Arithmetic T, detail::Qualifier Q>
constexpr Mat<C, R, T, Q>& operator-=(Mat<C, R, T, Q>& a, const Mat<C, R, T, Q>& b) noexcept {
    for (std::size_t c = 0; c < static_cast<std::size_t>(C); ++c)
        for (std::size_t r = 0; r < static_cast<std::size_t>(R); ++r)
            a(c, r) -= b(c, r);
    return a;
}

// Hadamard /=
template <int C, int R, detail::Arithmetic T, detail::Qualifier Q>
constexpr Mat<C, R, T, Q>& operator/=(Mat<C, R, T, Q>& a, const Mat<C, R, T, Q>& b) noexcept {
    for (std::size_t c = 0; c < static_cast<std::size_t>(C); ++c)
        for (std::size_t r = 0; r < static_cast<std::size_t>(R); ++r)
            a(c, r) /= b(c, r);
    return a;
}

// Scalar compound assignment
template <int C, int R, detail::Arithmetic T, detail::Qualifier Q>
constexpr Mat<C, R, T, Q>& operator*=(Mat<C, R, T, Q>& a, T s) noexcept {
    for (std::size_t c = 0; c < static_cast<std::size_t>(C); ++c)
        for (std::size_t r = 0; r < static_cast<std::size_t>(R); ++r)
            a(c, r) *= s;
    return a;
}

template <int C, int R, detail::Arithmetic T, detail::Qualifier Q>
constexpr Mat<C, R, T, Q>& operator+=(Mat<C, R, T, Q>& a, T s) noexcept {
    for (std::size_t c = 0; c < static_cast<std::size_t>(C); ++c)
        for (std::size_t r = 0; r < static_cast<std::size_t>(R); ++r)
            a(c, r) += s;
    return a;
}

template <int C, int R, detail::Arithmetic T, detail::Qualifier Q>
constexpr Mat<C, R, T, Q>& operator-=(Mat<C, R, T, Q>& a, T s) noexcept {
    for (std::size_t c = 0; c < static_cast<std::size_t>(C); ++c)
        for (std::size_t r = 0; r < static_cast<std::size_t>(R); ++r)
            a(c, r) -= s;
    return a;
}

template <int C, int R, detail::Arithmetic T, detail::Qualifier Q>
constexpr Mat<C, R, T, Q>& operator/=(Mat<C, R, T, Q>& a, T s) noexcept {
    for (std::size_t c = 0; c < static_cast<std::size_t>(C); ++c)
        for (std::size_t r = 0; r < static_cast<std::size_t>(R); ++r)
            a(c, r) /= s;
    return a;
}

// ---- Equality / inequality ----

template <int C, int R, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard]]
constexpr bool operator==(const Mat<C, R, T, Q>& a, const Mat<C, R, T, Q>& b) noexcept {
    for (std::size_t c = 0; c < static_cast<std::size_t>(C); ++c)
        for (std::size_t r = 0; r < static_cast<std::size_t>(R); ++r)
            if (a(c, r) != b(c, r))
                return false;
    return true;
}

template <int C, int R, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard]]
constexpr bool operator!=(const Mat<C, R, T, Q>& a, const Mat<C, R, T, Q>& b) noexcept {
    return !(a == b);
}

// ============================================================
//  Matrix product (Mat * Mat) — per GLSL, * on matrices IS the product.
//  Result[c][r] = sum over k of a(k, r) * b(c, k)
//  (column-major: result's column c = a * b's column c)
// ============================================================

namespace detail {

template <int C, int R, typename T, Qualifier Q>
[[nodiscard]]
constexpr Mat<C, R, T, Q> mat_matmul(const Mat<C, R, T, Q>& a, const Mat<C, R, T, Q>& b) {
    Mat<C, R, T, Q> result;
    // Column-major formula: result(c, r) = sum over k of a(c, k) * b(k, r)
    //
    // 列主序公式：result(c, r) = sum over k of a(c, k) * b(k, r)
    for (std::size_t c = 0; c < static_cast<std::size_t>(C); ++c) {
        for (std::size_t r = 0; r < static_cast<std::size_t>(R); ++r) {
            T sum = T{0};
            for (std::size_t k = 0; k < static_cast<std::size_t>(C); ++k) {
                sum += a(c, k) * b(k, r);
            }
            result(c, r) = sum;
        }
    }
    return result;
}

// Mat * Vec (right multiplication, column vector)
// result[r] = sum over c of m(c, r) * v[c]
//   (column-major: each column c of M contributes m(c, r) at row r,
//    scaled by v[c].)
//
// Mat * Vec（右乘，列向量）
// result[r] = sum over c of m(c, r) * v[c]
//   （列主序：M 的每一列 c 在行 r 处的元素 m(c, r) 由 v[c] 加权）
template <int C, int R, typename T, Qualifier Q>
[[nodiscard]]
constexpr Vec<R, T, Q> mat_vec_mul(const Mat<C, R, T, Q>& m, const Vec<C, T, Q>& v) {
    static_assert(C == R, "Mat * Vec requires square Mat");
    Vec<R, T, Q> result;
    for (std::size_t r = 0; r < static_cast<std::size_t>(R); ++r) {
        T sum = T{0};
        for (std::size_t c = 0; c < static_cast<std::size_t>(C); ++c) {
            sum += m(c, r) * v[c];
        }
        result[r] = sum;
    }
    return result;
}

// Vec * Mat (left multiplication, row vector)
// result[c] = sum over r of v[r] * m(c, r)
//   (column-major: the r-th element of v scales the r-th element of each
//    column c, contributing to result[c].)
//
// Vec * Mat（左乘，行向量）
// result[c] = sum over r of v[r] * m(c, r)
//   （列主序：v 的第 r 个元素对 M 每列 c 的第 r 个元素加权，贡献到 result[c]）
template <int C, int R, typename T, Qualifier Q>
[[nodiscard]]
constexpr Vec<C, T, Q> vec_mat_mul(const Vec<R, T, Q>& v, const Mat<C, R, T, Q>& m) {
    static_assert(C == R, "Vec * Mat requires square Mat");
    Vec<C, T, Q> result;
    for (std::size_t c = 0; c < static_cast<std::size_t>(C); ++c) {
        T sum = T{0};
        for (std::size_t r = 0; r < static_cast<std::size_t>(R); ++r) {
            sum += v[r] * m(c, r);
        }
        result[c] = sum;
    }
    return result;
}

}  // namespace detail

// Mat * Mat (matrix product) — NOT element-wise (no Hadamard * for Mat)
template <int C, int R, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard]]
constexpr Mat<C, R, T, Q> operator*(const Mat<C, R, T, Q>& a, const Mat<C, R, T, Q>& b) noexcept {
    return detail::mat_matmul(a, b);
}

// Mat * Vec (column vector right multiplication)
template <int C, int R, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard]]
constexpr Vec<C, T, Q> operator*(const Mat<C, R, T, Q>& m, const Vec<C, T, Q>& v) noexcept {
    return detail::mat_vec_mul(m, v);
}

// Vec * Mat (row vector left multiplication)
template <int C, int R, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard]]
constexpr Vec<R, T, Q> operator*(const Vec<R, T, Q>& v, const Mat<C, R, T, Q>& m) noexcept {
    return detail::vec_mat_mul(v, m);
}

// Mat * Mat assignment (matrix product, not Hadamard)
template <int C, int R, detail::Arithmetic T, detail::Qualifier Q>
constexpr Mat<C, R, T, Q>& operator*=(Mat<C, R, T, Q>& a, const Mat<C, R, T, Q>& b) noexcept {
    Mat<C, R, T, Q> tmp = detail::mat_matmul(a, b);
    a = tmp;
    return a;
}

}  // namespace m3
