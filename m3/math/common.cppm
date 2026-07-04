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

// mix uses x*(1-a) + y*a (not x + (y-x)*a) for better numerical stability
// when a is close to 1 — (y-x) can suffer catastrophic cancellation, and
// multiplying by a near-1 amplifies the error.  GLSL 4.60 §8.3 also
// recommends this formulation.
//
// mix 使用 x*(1-a) + y*a（而非 x + (y-x)*a）以获得更好的数值稳定性——
// 当 a 接近 1 时，(y-x) 可能发生灾难性抵消，再乘以接近 1 的 a 会放大
// 误差。GLSL 4.60 §8.3 也推荐此公式。
template <detail::Arithmetic T>
[[nodiscard("pure function: discarding a blend result is likely a bug")]]
constexpr T mix(T x, T y, T a) noexcept {
    return x * (static_cast<T>(1) - a) + y * a;
}

template <int L, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard("pure function: discarding a blend result is likely a bug")]]
constexpr Vec<L, T, Q> mix(const Vec<L, T, Q>& x, const Vec<L, T, Q>& y, T a) noexcept {
    return x * (static_cast<T>(1) - a) + y * a;
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
    // Defense-in-depth: catch invalid range even when contracts are
    // not active.
    //
    // 纵深防御：即使在 contracts 未激活时也能捕获无效范围。
    if (minVal > maxVal) [[unlikely]] {
        std::abort();
    }
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
