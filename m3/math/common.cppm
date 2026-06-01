// GLSL common functions: mix, clamp, lerp.
// Scalar and per-element vector overloads for each.
export module m3.math:common;

import std;

import m3.detail;
import m3.vector;

export namespace m3 {

    // ---- mix / lerp ----

    template<detail::Arithmetic T>
    [[nodiscard("pure function: discarding a blend result is likely a bug")]]
    constexpr T mix(const T& x, const T& y, const T& a) noexcept
    {
        return x + (y - x) * a;
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    [[nodiscard("pure function: discarding a blend result is likely a bug")]]
    constexpr Vec<L, T, Q> mix(
        const Vec<L, T, Q>& x, const Vec<L, T, Q>& y, const T& a) noexcept
    {
        return x + (y - x) * a;
    }

    // ---- clamp ----

    template<detail::Arithmetic T>
    [[nodiscard("pure function: discarding a clamp result is likely a bug")]] constexpr T clamp(
        const T& x, const T& minVal, const T& maxVal) noexcept
    {
        return x < minVal ? minVal : (x > maxVal ? maxVal : x);
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    [[nodiscard("pure function: discarding a clamp result is likely a bug")]] constexpr Vec<L, T, Q> clamp(
        const Vec<L, T, Q>& x, const T& minVal, const T& maxVal) noexcept
    {
        Vec<L, T, Q> result;
        for (int i = 0; i < L; ++i) {
            result[i] = clamp(x[i], minVal, maxVal);
        }
        return result;
    }

    // ---- lerp (alias for mix) ----

    template<detail::Arithmetic T>
    [[nodiscard("pure function: discarding a lerp result is likely a bug")]] constexpr T lerp(const T& x, const T& y, const T& a) noexcept
    {
        return mix(x, y, a);
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    [[nodiscard("pure function: discarding a lerp result is likely a bug")]] constexpr Vec<L, T, Q> lerp(
        const Vec<L, T, Q>& x, const Vec<L, T, Q>& y, const T& a) noexcept
    {
        return mix(x, y, a);
    }

} // namespace m3
