
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
        const auto reduce = [x, y] {
            // fmod reduces the arguments without needing a representable quotient.
            T remainder = std::fmod(x, y);
            if (remainder != T{0} && ((remainder < T{0}) != (y < T{0}))) {
                remainder += y;
            }
            return remainder;
        };
        const T divisor_magnitude = std::abs(y);
        const T limit = std::numeric_limits<T>::max();
        // Preflight the same overflow cases at compile time and at run time.
        if (divisor_magnitude < T{1} && std::abs(x) >= limit * divisor_magnitude) {
            return reduce();
        }
        const T quotient = std::floor(x / y);
        const T quotient_magnitude = std::abs(quotient);
        if (quotient_magnitude > T{1} && divisor_magnitude >= limit / quotient_magnitude) {
            return reduce();
        }
        return x - y * quotient;
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
clamp(T x, T min_value, T max_value) noexcept /* pre(min_value <= max_value) */ {
    return x < min_value ? min_value : (x > max_value ? max_value : x);
}

template <int L, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard("pure function: discarding a clamp result is likely a bug")]] constexpr Vec<L, T, Q>
clamp(const Vec<L, T, Q>& x, T min_value, T max_value) noexcept /* pre(min_value <= max_value) */ {

    if (min_value > max_value) [[unlikely]] {
        std::abort();
    }
    Vec<L, T, Q> result;
    if constexpr (L == 1) {
        result[0] = clamp(x[0], min_value, max_value);
    } else if constexpr (L == 2) {
        result[0] = clamp(x[0], min_value, max_value);
        result[1] = clamp(x[1], min_value, max_value);
    } else if constexpr (L == 3) {
        result[0] = clamp(x[0], min_value, max_value);
        result[1] = clamp(x[1], min_value, max_value);
        result[2] = clamp(x[2], min_value, max_value);
    } else if constexpr (L == 4) {
        result[0] = clamp(x[0], min_value, max_value);
        result[1] = clamp(x[1], min_value, max_value);
        result[2] = clamp(x[2], min_value, max_value);
        result[3] = clamp(x[3], min_value, max_value);
    } else {
        for (int i = 0; i < L; ++i)
            result[i] = clamp(x[i], min_value, max_value);
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
    return detail::Mod{}(x, y);
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
[[nodiscard]] constexpr T smoothstep(T edge0, T edge1, T x) noexcept /* pre(edge0 < edge1) */ {
    if (x <= edge0)
        return T{0};
    if (x >= edge1)
        return T{1};
    T t;
    if (edge0 < T{0} && edge1 >= std::numeric_limits<T>::max() + edge0) {
        // Halve before subtracting when the finite interval is wider than T can hold.
        t = (x / T{2} - edge0 / T{2}) / (edge1 / T{2} - edge0 / T{2});
    } else {
        t = (x - edge0) / (edge1 - edge0);
    }
    t = clamp(t, T{0}, T{1});
    return t * t * (static_cast<T>(3) - static_cast<T>(2) * t);
}

template <int L, detail::FloatingPoint T, detail::Qualifier Q>
[[nodiscard]] constexpr Vec<L, T, Q> smoothstep(T edge0, T edge1, const Vec<L, T, Q>& x) noexcept
    /* pre(edge0 < edge1) */ {
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
