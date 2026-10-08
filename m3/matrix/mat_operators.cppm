
export module m3.matrix:operators;

import std;

import m3.detail;
import m3.matrix.base;
import m3.matrix.mat;
import m3.vector.vec;

export namespace m3 {

namespace detail {

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

}

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

template <int C, int R, detail::Arithmetic T, detail::Qualifier Q>
constexpr Mat<C, R, T, Q>& operator/=(Mat<C, R, T, Q>& a, const Mat<C, R, T, Q>& b) noexcept {
    for (std::size_t c = 0; c < static_cast<std::size_t>(C); ++c)
        for (std::size_t r = 0; r < static_cast<std::size_t>(R); ++r)
            a(c, r) /= b(c, r);
    return a;
}

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

namespace detail {

template <int C, int R, typename T, Qualifier Q>
[[nodiscard]]
constexpr Mat<C, R, T, Q> mat_matmul(const Mat<C, R, T, Q>& a, const Mat<C, R, T, Q>& b) {
    Mat<C, R, T, Q> result;

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

}

template <int C, int R, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard]]
constexpr Mat<C, R, T, Q> operator*(const Mat<C, R, T, Q>& a, const Mat<C, R, T, Q>& b) noexcept {
    return detail::mat_matmul(a, b);
}

template <int C, int R, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard]]
constexpr Vec<R, T, Q> operator*(const Mat<C, R, T, Q>& m, const Vec<C, T, Q>& v) noexcept {
    return detail::mat_vec_mul(m, v);
}

template <int C, int R, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard]]
constexpr Vec<R, T, Q> operator*(const Vec<R, T, Q>& v, const Mat<C, R, T, Q>& m) noexcept {
    return detail::vec_mat_mul(v, m);
}

template <int C, int R, detail::Arithmetic T, detail::Qualifier Q>
constexpr Mat<C, R, T, Q>& operator*=(Mat<C, R, T, Q>& a, const Mat<C, R, T, Q>& b) noexcept {
    Mat<C, R, T, Q> tmp = detail::mat_matmul(a, b);
    a = tmp;
    return a;
}

}
