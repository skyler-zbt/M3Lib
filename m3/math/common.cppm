// GLSL common functions: mix, clamp, lerp.
// Scalar and per-element vector overloads for each.
//
// GLSL 通用函数：mix、clamp、lerp。
// 每个函数均提供标量和逐元素向量重载。
export module m3.math:common;

import std;

import m3.detail;
import m3.vector;

// Local element-wise functors for the common functions defined below.
// Defined here (non-exported, module linkage) rather than in
// m3.detail.operations.ops because they are specific to GLSL common
// functions and are not reused by the vector arithmetic operators.
//
// 下方通用函数使用的局部逐元素函数对象。
// 定义于此（非导出、模块链接）而非 m3.detail.operations.ops，
// 因为它们专用于 GLSL 通用函数，不被向量算术运算符复用。
namespace m3::detail {

struct Abs {
    template <typename T>
    constexpr T operator()(T x) const noexcept {
        return x < static_cast<T>(0) ? -x : x;
    }
};

struct Sign {
    template <typename T>
    constexpr T operator()(T x) const noexcept {
        if (x > static_cast<T>(0))
            return static_cast<T>(1);
        if (x < static_cast<T>(0))
            return static_cast<T>(-1);
        return static_cast<T>(0);
    }
};

struct Floor {
    template <typename T>
    constexpr T operator()(T x) const noexcept {
        return std::floor(x);
    }
};

struct Ceil {
    template <typename T>
    constexpr T operator()(T x) const noexcept {
        return std::ceil(x);
    }
};

struct Fract {
    template <typename T>
    constexpr T operator()(T x) const noexcept {
        return x - std::floor(x);
    }
};

struct Mod {
    // GLSL mod: x - y * floor(x/y)  (NOT std::fmod which truncates toward zero)
    template <typename T>
    constexpr T operator()(T x, T y) const noexcept {
        return x - y * std::floor(x / y);
    }
};

struct Min {
    template <typename T>
    constexpr T operator()(T x, T y) const noexcept {
        return x < y ? x : y;
    }
};

struct Max {
    template <typename T>
    constexpr T operator()(T x, T y) const noexcept {
        return x > y ? x : y;
    }
};

struct Step {
    // Used with apply_scalar_binary_right: Op{}(v[i], edge)
    // => v[i] < edge ? 0 : 1, matching GLSL step(edge, x).
    template <typename T>
    constexpr T operator()(T x, T edge) const noexcept {
        return x < edge ? static_cast<T>(0) : static_cast<T>(1);
    }
};

}  // namespace m3::detail

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

// ---- abs / sign ----

// abs — absolute value, works for both integral and floating-point types.
//
// abs —— 绝对值，适用于整数和浮点类型。
template <detail::Arithmetic T>
[[nodiscard]] constexpr T abs(T x) noexcept {
    return x < static_cast<T>(0) ? -x : x;
}

template <int L, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard]] constexpr Vec<L, T, Q> abs(const Vec<L, T, Q>& x) noexcept {
    return detail::apply_unary<detail::Abs>(x);
}

// sign — returns -1, 0, or 1 for negative, zero, or positive input.
//
// sign —— 对负数、零、正数分别返回 -1、0、1。
template <detail::Arithmetic T>
[[nodiscard]] constexpr T sign(T x) noexcept {
    if (x > static_cast<T>(0))
        return static_cast<T>(1);
    if (x < static_cast<T>(0))
        return static_cast<T>(-1);
    return static_cast<T>(0);
}

template <int L, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard]] constexpr Vec<L, T, Q> sign(const Vec<L, T, Q>& x) noexcept {
    return detail::apply_unary<detail::Sign>(x);
}

// ---- floor / ceil / fract ----

template <detail::FloatingPoint T>
[[nodiscard]] constexpr T floor(T x) noexcept {
    return std::floor(x);
}

template <int L, detail::FloatingPoint T, detail::Qualifier Q>
[[nodiscard]] constexpr Vec<L, T, Q> floor(const Vec<L, T, Q>& x) noexcept {
    return detail::apply_unary<detail::Floor>(x);
}

template <detail::FloatingPoint T>
[[nodiscard]] constexpr T ceil(T x) noexcept {
    return std::ceil(x);
}

template <int L, detail::FloatingPoint T, detail::Qualifier Q>
[[nodiscard]] constexpr Vec<L, T, Q> ceil(const Vec<L, T, Q>& x) noexcept {
    return detail::apply_unary<detail::Ceil>(x);
}

// fract — GLSL: returns x - floor(x), result is always in [0, 1).
//
// fract —— GLSL：返回 x - floor(x)，结果恒在 [0, 1) 区间。
template <detail::FloatingPoint T>
[[nodiscard]] constexpr T fract(T x) noexcept {
    return x - std::floor(x);
}

template <int L, detail::FloatingPoint T, detail::Qualifier Q>
[[nodiscard]] constexpr Vec<L, T, Q> fract(const Vec<L, T, Q>& x) noexcept {
    return detail::apply_unary<detail::Fract>(x);
}

// ---- mod (GLSL semantics) ----

// GLSL mod: x - y * floor(x/y).
// This differs from std::fmod, which truncates toward zero and may return
// a negative result for negative x.  GLSL mod always returns a value with
// the same sign as y.
//
// GLSL mod：x - y * floor(x/y)。
// 与 std::fmod 不同——后者向零截断，对负 x 可能返回负值。
// GLSL mod 的结果始终与 y 同号。
template <detail::FloatingPoint T>
[[nodiscard]] constexpr T mod(T x, T y) noexcept {
    return x - y * std::floor(x / y);
}

template <int L, detail::FloatingPoint T, detail::Qualifier Q>
[[nodiscard]] constexpr Vec<L, T, Q> mod(const Vec<L, T, Q>& x, const Vec<L, T, Q>& y) noexcept {
    return detail::apply_binary<detail::Mod>(x, y);
}

template <int L, detail::FloatingPoint T, detail::Qualifier Q>
[[nodiscard]] constexpr Vec<L, T, Q> mod(const Vec<L, T, Q>& x, T y) noexcept {
    return detail::apply_scalar_binary_right<detail::Mod>(x, y);
}

// ---- min / max ----

template <detail::Arithmetic T>
[[nodiscard]] constexpr T min(T x, T y) noexcept {
    return x < y ? x : y;
}

template <int L, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard]] constexpr Vec<L, T, Q> min(const Vec<L, T, Q>& x, const Vec<L, T, Q>& y) noexcept {
    return detail::apply_binary<detail::Min>(x, y);
}

template <int L, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard]] constexpr Vec<L, T, Q> min(const Vec<L, T, Q>& x, T y) noexcept {
    return detail::apply_scalar_binary_right<detail::Min>(x, y);
}

template <detail::Arithmetic T>
[[nodiscard]] constexpr T max(T x, T y) noexcept {
    return x > y ? x : y;
}

template <int L, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard]] constexpr Vec<L, T, Q> max(const Vec<L, T, Q>& x, const Vec<L, T, Q>& y) noexcept {
    return detail::apply_binary<detail::Max>(x, y);
}

template <int L, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard]] constexpr Vec<L, T, Q> max(const Vec<L, T, Q>& x, T y) noexcept {
    return detail::apply_scalar_binary_right<detail::Max>(x, y);
}

// ---- step ----

// step — GLSL: returns 0.0 if x < edge, else 1.0.
//
// step —— GLSL：若 x < edge 返回 0.0，否则返回 1.0。
template <detail::FloatingPoint T>
[[nodiscard]] constexpr T step(T edge, T x) noexcept {
    return x < edge ? static_cast<T>(0) : static_cast<T>(1);
}

// Vector step: scalar edge applied component-wise to x.
// Uses apply_scalar_binary_right with the Step functor, which computes
// Op{}(x[i], edge) = x[i] < edge ? 0 : 1, matching GLSL step(edge, x[i]).
//
// 向量 step：标量 edge 逐分量应用于 x。
// 使用 apply_scalar_binary_right 配合 Step 函数对象，计算
// Op{}(x[i], edge) = x[i] < edge ? 0 : 1，与 GLSL step(edge, x[i]) 一致。
template <int L, detail::FloatingPoint T, detail::Qualifier Q>
[[nodiscard]] constexpr Vec<L, T, Q> step(T edge, const Vec<L, T, Q>& x) noexcept {
    return detail::apply_scalar_binary_right<detail::Step>(x, edge);
}

// ---- smoothstep ----

// smoothstep — GLSL: Hermite interpolation.
// t = clamp((x - edge0) / (edge1 - edge0), 0, 1)
// return t * t * (3 - 2 * t)
//
// smoothstep —— GLSL：厄米插值。
// t = clamp((x - edge0) / (edge1 - edge0), 0, 1)
// 返回 t * t * (3 - 2 * t)
template <detail::FloatingPoint T>
[[nodiscard]] constexpr T smoothstep(T edge0, T edge1, T x) noexcept pre(edge0 < edge1) {
    T t = clamp((x - edge0) / (edge1 - edge0), static_cast<T>(0), static_cast<T>(1));
    return t * t * (static_cast<T>(3) - static_cast<T>(2) * t);
}

template <int L, detail::FloatingPoint T, detail::Qualifier Q>
[[nodiscard]] constexpr Vec<L, T, Q> smoothstep(T edge0, T edge1, const Vec<L, T, Q>& x) noexcept
    pre(edge0 < edge1) {
    Vec<L, T, Q> result;
    if constexpr (L == 1) {
        result[0] = smoothstep(edge0, edge1, x[0]);
    } else if constexpr (L == 2) {
        result[0] = smoothstep(edge0, edge1, x[0]);
        result[1] = smoothstep(edge0, edge1, x[1]);
    } else if constexpr (L == 3) {
        result[0] = smoothstep(edge0, edge1, x[0]);
        result[1] = smoothstep(edge0, edge1, x[1]);
        result[2] = smoothstep(edge0, edge1, x[2]);
    } else if constexpr (L == 4) {
        result[0] = smoothstep(edge0, edge1, x[0]);
        result[1] = smoothstep(edge0, edge1, x[1]);
        result[2] = smoothstep(edge0, edge1, x[2]);
        result[3] = smoothstep(edge0, edge1, x[3]);
    }
    return result;
}

}  // namespace m3
