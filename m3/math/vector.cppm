// GLSL vector functions: dot, cross, normalize, length, distance, reflect, refract.
// All functions are constexpr and accept Vec<L, T, Q> for any valid dimension.
// cross() is restricted to Vec<3> per GLSL convention.
//
// GLSL 向量函数：dot、cross、normalize、length、distance、reflect、refract。
// 所有函数均为 constexpr，接受任意合法维度的 Vec<L, T, Q>。
// 按 GLSL 惯例，cross() 仅限于 Vec<3>。
export module m3.math:vector;

import std;

import m3.detail;
import m3.vector;

export namespace m3 {

template <int L, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard("pure function: discarding a dot product is likely a bug")]] constexpr T
dot(const Vec<L, T, Q>& a, const Vec<L, T, Q>& b) noexcept {
    if constexpr (L == 1) {
        return a[0] * b[0];
    } else if constexpr (L == 2) {
        return a[0] * b[0] + a[1] * b[1];
    } else if constexpr (L == 3) {
        return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
    } else if constexpr (L == 4) {
        return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
    } else {
        T result{};
        for (int i = 0; i < L; ++i)
            result += a[i] * b[i];
        return result;
    }
}

template <detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard("pure function: discarding a cross product is likely a bug")]] constexpr Vec<3, T, Q>
cross(const Vec<3, T, Q>& a, const Vec<3, T, Q>& b) noexcept {
    return Vec<3, T, Q>{a.y() * b.z() - a.z() * b.y(), a.z() * b.x() - a.x() * b.z(),
                        a.x() * b.y() - a.y() * b.x()};
}

template <int L, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard("pure function: discarding a length computation is likely a bug")]] constexpr auto
length(const Vec<L, T, Q>& v) noexcept -> decltype(std::sqrt(dot(v, v))) {
    return std::sqrt(dot(v, v));
}

template <int L, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard("pure function: discarding a distance computation is likely a bug")]] constexpr auto
distance(const Vec<L, T, Q>& a, const Vec<L, T, Q>& b) noexcept -> decltype(length(a - b)) {
    return length(a - b);
}

template <int L, detail::FloatingPoint T, detail::Qualifier Q>
[[nodiscard("pure function: discarding a normalized vector is likely a bug; use normalize() to "
            "produce a unit vector")]] constexpr Vec<L, T, Q>
normalize(const Vec<L, T, Q>& v) noexcept {
    auto len = length(v);
    if (len == static_cast<decltype(len)>(0)) {
        return Vec<L, T, Q>{static_cast<T>(0)};
    }
    return v / static_cast<T>(len);
}

template <int L, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard("pure function: discarding a reflected vector is likely a bug")]] constexpr Vec<L, T, Q>
reflect(const Vec<L, T, Q>& I, const Vec<L, T, Q>& N) noexcept {
    return I - N * static_cast<T>(2) * dot(N, I);
}

template <int L, detail::FloatingPoint T, detail::Qualifier Q>
[[nodiscard("pure function: discarding a refracted vector is likely a bug")]] constexpr Vec<L, T, Q>
refract(const Vec<L, T, Q>& I, const Vec<L, T, Q>& N, T eta) noexcept {
    auto d = dot(N, I);
    auto k = static_cast<T>(1) - eta * eta * (static_cast<T>(1) - d * d);
    if (k < static_cast<T>(0)) {
        return Vec<L, T, Q>{static_cast<T>(0)};
    }
    return I * eta - N * (eta * d + std::sqrt(k));
}

}  // namespace m3
