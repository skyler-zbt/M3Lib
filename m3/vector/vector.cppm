// Vector arithmetic and comparison operators.
// Component-wise operators delegate to detail::apply_binary / apply_unary.
// Scalar operators delegate to detail::apply_scalar_binary_left / _right,
// eliminating the ~140 lines of manual unrolling that previously existed.
// Compound assignment operators delegate to the corresponding binary operators.
//
// 向量算术和比较运算符。
// 逐分量运算符委托给 detail::apply_binary / apply_unary。
// 标量运算符委托给 detail::apply_scalar_binary_left / _right，
// 消除了原先存在的约 140 行手动展开代码。
// 复合赋值运算符委托给对应的二元运算符。
export module m3.vector;

import std;

export import m3.vector.base;
export import m3.vector.vec;

import m3.detail;
import m3.detail.operations.ops;
import m3.detail.operations.apply;

export namespace m3 {

    // ---- component-wise binary operators (delegate to detail::apply_binary) ----

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    [[nodiscard]] constexpr Vec<L, T, Q> operator+(const Vec<L, T, Q>& a, const Vec<L, T, Q>& b) {
        return detail::apply_binary<detail::add>(a, b);
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    [[nodiscard]] constexpr Vec<L, T, Q> operator-(const Vec<L, T, Q>& a, const Vec<L, T, Q>& b) {
        return detail::apply_binary<detail::sub>(a, b);
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    [[nodiscard]] constexpr Vec<L, T, Q> operator*(const Vec<L, T, Q>& a, const Vec<L, T, Q>& b) {
        return detail::apply_binary<detail::mul>(a, b);
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    [[nodiscard]] constexpr Vec<L, T, Q> operator/(const Vec<L, T, Q>& a, const Vec<L, T, Q>& b) {
        return detail::apply_binary<detail::div>(a, b);
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    [[nodiscard]] constexpr Vec<L, T, Q> operator-(const Vec<L, T, Q>& a) {
        return detail::apply_unary<detail::neg>(a);
    }

    // ---- scalar binary operators (delegate to apply_scalar_binary_*) ----

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    [[nodiscard]] constexpr Vec<L, T, Q> operator*(const Vec<L, T, Q>& a, T scalar) {
        return detail::apply_scalar_binary_right<detail::mul>(a, scalar);
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    [[nodiscard]] constexpr Vec<L, T, Q> operator*(T scalar, const Vec<L, T, Q>& a) {
        return detail::apply_scalar_binary_left<detail::mul>(scalar, a);
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    [[nodiscard]] constexpr Vec<L, T, Q> operator/(const Vec<L, T, Q>& a, T scalar) {
        return detail::apply_scalar_binary_right<detail::div>(a, scalar);
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    [[nodiscard]] constexpr Vec<L, T, Q> operator+(const Vec<L, T, Q>& a, T scalar) {
        return detail::apply_scalar_binary_right<detail::add>(a, scalar);
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    [[nodiscard]] constexpr Vec<L, T, Q> operator+(T scalar, const Vec<L, T, Q>& a) {
        return detail::apply_scalar_binary_left<detail::add>(scalar, a);
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    [[nodiscard]] constexpr Vec<L, T, Q> operator-(const Vec<L, T, Q>& a, T scalar) {
        return detail::apply_scalar_binary_right<detail::sub>(a, scalar);
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    [[nodiscard]] constexpr Vec<L, T, Q> operator-(T scalar, const Vec<L, T, Q>& a) {
        return detail::apply_scalar_binary_left<detail::sub>(scalar, a);
    }

    // ---- compound assignment operators (delegate to binary operators) ----

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    constexpr Vec<L, T, Q>& operator+=(Vec<L, T, Q>& a, const Vec<L, T, Q>& b) {
        return a = a + b;
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    constexpr Vec<L, T, Q>& operator-=(Vec<L, T, Q>& a, const Vec<L, T, Q>& b) {
        return a = a - b;
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    constexpr Vec<L, T, Q>& operator*=(Vec<L, T, Q>& a, const Vec<L, T, Q>& b) {
        return a = a * b;
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    constexpr Vec<L, T, Q>& operator/=(Vec<L, T, Q>& a, const Vec<L, T, Q>& b) {
        return a = a / b;
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    constexpr Vec<L, T, Q>& operator*=(Vec<L, T, Q>& a, T scalar) {
        return a = a * scalar;
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    constexpr Vec<L, T, Q>& operator/=(Vec<L, T, Q>& a, T scalar) {
        return a = a / scalar;
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    constexpr Vec<L, T, Q>& operator+=(Vec<L, T, Q>& a, T scalar) {
        return a = a + scalar;
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    constexpr Vec<L, T, Q>& operator-=(Vec<L, T, Q>& a, T scalar) {
        return a = a - scalar;
    }

    // ---- comparison operators ----

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    [[nodiscard]] constexpr bool operator==(const Vec<L, T, Q>& a, const Vec<L, T, Q>& b) {
        for (int i = 0; i < L; ++i) {
            if (a[i] != b[i]) return false;
        }
        return true;
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    [[nodiscard]] constexpr bool operator!=(const Vec<L, T, Q>& a, const Vec<L, T, Q>& b) {
        return !(a == b);
    }

} // namespace m3
