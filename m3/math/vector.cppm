// GLSL vector functions: dot, cross, normalize, length, distance, reflect, refract.
// All functions are constexpr and accept Vec<L, T, Q> for any valid dimension.
// cross() is restricted to Vec<3> per GLSL convention.
export module m3.math:vector;

import std;

import m3.detail;
import m3.detail.operations.ops;
import m3.vector;

export namespace m3 {

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    [[nodiscard("pure function: discarding a dot product is likely a bug")]] constexpr T dot(const Vec<L, T, Q>& a, const Vec<L, T, Q>& b) noexcept
    {
        T result{};
        for (int i = 0; i < L; ++i) {
            result += a[i] * b[i];
        }
        return result;
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    [[nodiscard("pure function: discarding a cross product is likely a bug")]] constexpr Vec<3, T, Q> cross(
        const Vec<3, T, Q>& a, const Vec<3, T, Q>& b) noexcept
    {
        return Vec<3, T, Q>{
            a.y() * b.z() - a.z() * b.y(),
            a.z() * b.x() - a.x() * b.z(),
            a.x() * b.y() - a.y() * b.x()
        };
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    [[nodiscard("pure function: discarding a length computation is likely a bug")]] constexpr auto length(const Vec<L, T, Q>& v) noexcept
        -> decltype(std::sqrt(dot(v, v)))
    {
        return std::sqrt(dot(v, v));
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    [[nodiscard("pure function: discarding a distance computation is likely a bug")]] constexpr auto distance(
        const Vec<L, T, Q>& a, const Vec<L, T, Q>& b) noexcept
        -> decltype(length(a - b))
    {
        return length(a - b);
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    [[nodiscard("pure function: discarding a normalized vector is likely a bug; use normalize() to produce a unit vector")]] constexpr Vec<L, T, Q> normalize(
        const Vec<L, T, Q>& v) noexcept
    {
        auto len = length(v);
        if (len == static_cast<decltype(len)>(0)) {
            return Vec<L, T, Q>{static_cast<T>(0)};
        }
        return v / static_cast<T>(len);
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    [[nodiscard("pure function: discarding a reflected vector is likely a bug")]] constexpr Vec<L, T, Q> reflect(
        const Vec<L, T, Q>& I, const Vec<L, T, Q>& N) noexcept
    {
        return I - N * static_cast<T>(2) * dot(N, I);
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    [[nodiscard("pure function: discarding a refracted vector is likely a bug")]] constexpr Vec<L, T, Q> refract(
        const Vec<L, T, Q>& I, const Vec<L, T, Q>& N, T eta) noexcept
    {
        auto d = dot(N, I);
        auto k = static_cast<T>(1) - eta * eta * (static_cast<T>(1) - d * d);
        if (k < static_cast<T>(0)) {
            return Vec<L, T, Q>{static_cast<T>(0)};
        }
        return I * eta - N * (eta * d + std::sqrt(k));
    }

} // namespace m3
