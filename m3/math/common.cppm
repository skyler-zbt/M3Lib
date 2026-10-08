
export module m3.math:common;

import std;

import m3.detail;
import m3.vector;

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

    template <typename T>
    constexpr T operator()(T x, T edge) const noexcept {
        return x < edge ? static_cast<T>(0) : static_cast<T>(1);
    }
};

}

export namespace m3 {

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

template <detail::Arithmetic T>
[[nodiscard("pure function: discarding a clamp result is likely a bug")]] constexpr T
clamp(T x, T minVal, T maxVal) noexcept pre(minVal <= maxVal) {
    return x < minVal ? minVal : (x > maxVal ? maxVal : x);
}

template <int L, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard("pure function: discarding a clamp result is likely a bug")]] constexpr Vec<L, T, Q>
clamp(const Vec<L, T, Q>& x, T minVal, T maxVal) noexcept pre(minVal <= maxVal) {

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

template <detail::Arithmetic T>
[[nodiscard]] constexpr T abs(T x) noexcept {
    return x < static_cast<T>(0) ? -x : x;
}

template <int L, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard]] constexpr Vec<L, T, Q> abs(const Vec<L, T, Q>& x) noexcept {
    return detail::apply_unary<detail::Abs>(x);
}

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

template <detail::FloatingPoint T>
[[nodiscard]] constexpr T fract(T x) noexcept {
    return x - std::floor(x);
}

template <int L, detail::FloatingPoint T, detail::Qualifier Q>
[[nodiscard]] constexpr Vec<L, T, Q> fract(const Vec<L, T, Q>& x) noexcept {
    return detail::apply_unary<detail::Fract>(x);
}

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

template <detail::FloatingPoint T>
[[nodiscard]] constexpr T step(T edge, T x) noexcept {
    return x < edge ? static_cast<T>(0) : static_cast<T>(1);
}

template <int L, detail::FloatingPoint T, detail::Qualifier Q>
[[nodiscard]] constexpr Vec<L, T, Q> step(T edge, const Vec<L, T, Q>& x) noexcept {
    return detail::apply_scalar_binary_right<detail::Step>(x, edge);
}

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

}
