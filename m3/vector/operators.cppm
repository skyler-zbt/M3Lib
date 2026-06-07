// Vector component-wise, scalar, compound-assignment, and comparison operators.
// Most operators delegate to the generic dispatch helpers in m3.detail
// (apply_binary, apply_unary, apply_scalar_binary_left,
// apply_scalar_binary_right).  operator== / operator!= are implemented
// directly with short-circuiting if-constexpr unrolling for better
// branch-prediction on comparison.
//
// 向量逐分量、标量、复合赋值和比较运算符。
// 多数运算符委托给 m3.detail 中的泛型分发辅助函数（apply_binary、
// apply_unary、apply_scalar_binary_left、apply_scalar_binary_right）。
// operator== / operator!= 直接以短路 if-constexpr 展开实现，
// 为比较操作提供更好的分支预测。
export module m3.vector:operators;

import m3.detail;
import m3.vector.vec;

export namespace m3 {

// ---- component-wise binary operators (delegate to detail::apply_binary) ----

template <int L, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard]] constexpr Vec<L, T, Q> operator+(const Vec<L, T, Q>& a,
                                               const Vec<L, T, Q>& b) noexcept {
    return detail::apply_binary<detail::Add>(a, b);
}

template <int L, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard]] constexpr Vec<L, T, Q> operator-(const Vec<L, T, Q>& a,
                                               const Vec<L, T, Q>& b) noexcept {
    return detail::apply_binary<detail::Sub>(a, b);
}

template <int L, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard]] constexpr Vec<L, T, Q> operator*(const Vec<L, T, Q>& a,
                                               const Vec<L, T, Q>& b) noexcept {
    return detail::apply_binary<detail::Mul>(a, b);
}

template <int L, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard]] constexpr Vec<L, T, Q> operator/(const Vec<L, T, Q>& a,
                                               const Vec<L, T, Q>& b) noexcept {
    return detail::apply_binary<detail::Div>(a, b);
}

template <int L, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard]] constexpr Vec<L, T, Q> operator-(const Vec<L, T, Q>& a) noexcept {
    return detail::apply_unary<detail::Neg>(a);
}

// ---- scalar binary operators (delegate to apply_scalar_binary_*) ----

template <int L, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard]] constexpr Vec<L, T, Q> operator*(const Vec<L, T, Q>& a, T scalar) noexcept {
    return detail::apply_scalar_binary_right<detail::Mul>(a, scalar);
}

template <int L, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard]] constexpr Vec<L, T, Q> operator*(T scalar, const Vec<L, T, Q>& a) noexcept {
    return detail::apply_scalar_binary_left<detail::Mul>(scalar, a);
}

template <int L, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard]] constexpr Vec<L, T, Q> operator/(const Vec<L, T, Q>& a, T scalar) noexcept {
    return detail::apply_scalar_binary_right<detail::Div>(a, scalar);
}

template <int L, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard]] constexpr Vec<L, T, Q> operator+(const Vec<L, T, Q>& a, T scalar) noexcept {
    return detail::apply_scalar_binary_right<detail::Add>(a, scalar);
}

template <int L, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard]] constexpr Vec<L, T, Q> operator+(T scalar, const Vec<L, T, Q>& a) noexcept {
    return detail::apply_scalar_binary_left<detail::Add>(scalar, a);
}

template <int L, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard]] constexpr Vec<L, T, Q> operator-(const Vec<L, T, Q>& a, T scalar) noexcept {
    return detail::apply_scalar_binary_right<detail::Sub>(a, scalar);
}

template <int L, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard]] constexpr Vec<L, T, Q> operator-(T scalar, const Vec<L, T, Q>& a) noexcept {
    return detail::apply_scalar_binary_left<detail::Sub>(scalar, a);
}

// ---- compound assignment operators (delegate to binary operators) ----

template <int L, detail::Arithmetic T, detail::Qualifier Q>
constexpr Vec<L, T, Q>& operator+=(Vec<L, T, Q>& a, const Vec<L, T, Q>& b) noexcept {
    return a = a + b;
}

template <int L, detail::Arithmetic T, detail::Qualifier Q>
constexpr Vec<L, T, Q>& operator-=(Vec<L, T, Q>& a, const Vec<L, T, Q>& b) noexcept {
    return a = a - b;
}

template <int L, detail::Arithmetic T, detail::Qualifier Q>
constexpr Vec<L, T, Q>& operator*=(Vec<L, T, Q>& a, const Vec<L, T, Q>& b) noexcept {
    return a = a * b;
}

template <int L, detail::Arithmetic T, detail::Qualifier Q>
constexpr Vec<L, T, Q>& operator/=(Vec<L, T, Q>& a, const Vec<L, T, Q>& b) noexcept {
    return a = a / b;
}

template <int L, detail::Arithmetic T, detail::Qualifier Q>
constexpr Vec<L, T, Q>& operator*=(Vec<L, T, Q>& a, T scalar) noexcept {
    return a = a * scalar;
}

template <int L, detail::Arithmetic T, detail::Qualifier Q>
constexpr Vec<L, T, Q>& operator/=(Vec<L, T, Q>& a, T scalar) noexcept {
    return a = a / scalar;
}

template <int L, detail::Arithmetic T, detail::Qualifier Q>
constexpr Vec<L, T, Q>& operator+=(Vec<L, T, Q>& a, T scalar) noexcept {
    return a = a + scalar;
}

template <int L, detail::Arithmetic T, detail::Qualifier Q>
constexpr Vec<L, T, Q>& operator-=(Vec<L, T, Q>& a, T scalar) noexcept {
    return a = a - scalar;
}

// ---- comparison operators ----

template <int L, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard]] constexpr bool operator==(const Vec<L, T, Q>& a, const Vec<L, T, Q>& b) noexcept {
    if constexpr (L == 1) {
        return a[0] == b[0];
    } else if constexpr (L == 2) {
        return a[0] == b[0] && a[1] == b[1];
    } else if constexpr (L == 3) {
        return a[0] == b[0] && a[1] == b[1] && a[2] == b[2];
    } else if constexpr (L == 4) {
        return a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3];
    } else {
        for (int i = 0; i < L; ++i) {
            if (a[i] != b[i])
                return false;
        }
        return true;
    }
}

template <int L, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard]] constexpr bool operator!=(const Vec<L, T, Q>& a, const Vec<L, T, Q>& b) noexcept {
    return !(a == b);
}

}  // namespace m3
