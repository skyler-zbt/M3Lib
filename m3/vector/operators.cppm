
export module m3.vector:operators;

import m3.detail;
import m3.vector.vec;

export namespace m3 {

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

}
