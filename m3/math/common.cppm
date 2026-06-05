// GLSL common functions: mix, clamp, lerp.
// Scalar and per-element vector overloads for each.
//
// GLSL 通用函数：mix、clamp、lerp。
// 每个函数均提供标量和逐元素向量重载。
export module m3.math:common;

import std;

import m3.detail;
import m3.vector;

export namespace m3 {

// ---- mix / lerp ----

template <detail::Arithmetic T>
[[nodiscard("pure function: discarding a blend result is likely a bug")]]
constexpr T mix(T x, T y, T a) noexcept {
    return x + (y - x) * a;
}

template <int L, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard("pure function: discarding a blend result is likely a bug")]]
constexpr Vec<L, T, Q> mix(const Vec<L, T, Q>& x, const Vec<L, T, Q>& y, T a) noexcept {
    return x + (y - x) * a;
}

// ---- clamp ----

template <detail::Arithmetic T>
[[nodiscard("pure function: discarding a clamp result is likely a bug")]] constexpr T
clamp(T x, T minVal, T maxVal) noexcept pre(minVal <= maxVal) {
    return x < minVal ? minVal : (x > maxVal ? maxVal : x);
}

template <int L, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard("pure function: discarding a clamp result is likely a bug")]] constexpr Vec<L, T, Q>
clamp(const Vec<L, T, Q>& x, T minVal, T maxVal) noexcept pre(minVal <= maxVal) {
    Vec<L, T, Q> result;
    if constexpr (L == 1) {
        result[0] = clamp(x[0], minVal, maxVal);
    } else if constexpr (L == 2) {
        result[0] = clamp(x[0], minVal, maxVal);
        result[1] = clamp(x[1], minVal, maxVal);
    } else if constexpr (L == 3) {
        result[0] = clamp(x[0], minVal, maxVal);
        result[1] = clamp(x[1], minVal, maxVal);
        result[2] = clamp(x[2], minVal, maxVal);
    } else if constexpr (L == 4) {
        result[0] = clamp(x[0], minVal, maxVal);
        result[1] = clamp(x[1], minVal, maxVal);
        result[2] = clamp(x[2], minVal, maxVal);
        result[3] = clamp(x[3], minVal, maxVal);
    } else {
        for (int i = 0; i < L; ++i)
            result[i] = clamp(x[i], minVal, maxVal);
    }
    return result;
}

// ---- lerp (alias for mix) ----

// lerp — GLSL compatibility alias for mix.
//
// lerp —— GLSL 兼容别名，等同于 mix。
template <detail::Arithmetic T>
[[nodiscard("pure function: discarding a lerp result is likely a bug")]] constexpr T
lerp(T x, T y, T a) noexcept {
    return mix(x, y, a);
}

template <int L, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard("pure function: discarding a lerp result is likely a bug")]] constexpr Vec<L, T, Q>
lerp(const Vec<L, T, Q>& x, const Vec<L, T, Q>& y, T a) noexcept {
    return mix(x, y, a);
}

}  // namespace m3
