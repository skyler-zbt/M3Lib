// GLSL 4.60 §8.1 trigonometric functions: sin, cos, tan, asin, acos, atan,
// atan2, radians, degrees.
// Each function provides a scalar overload (constrained to FloatingPoint)
// and a Vec<1..4> per-element overload via apply_unary / apply_binary.
//
// GLSL 4.60 §8.1 三角函数：sin、cos、tan、asin、acos、atan、atan2、radians、degrees。
// 每个函数均提供标量重载（约束为 FloatingPoint）和通过 apply_unary / apply_binary
// 实现的 Vec<1..4> 逐元素重载。
export module m3.math:trig;

import std;

import m3.detail;
import m3.vector;

namespace m3::detail {

// Element-wise functors for trigonometric operations.  Each wraps the
// corresponding std::math function so that apply_unary / apply_binary can
// dispatch generically across Vec<1..4>.
//
// 三角运算的逐元素函数对象。每个包装对应的 std::math 函数，
// 使 apply_unary / apply_binary 能泛型分派到 Vec<1..4>。
struct Sin {
    template <typename T>
    constexpr T operator()(T x) const noexcept {
        return std::sin(x);
    }
};

struct Cos {
    template <typename T>
    constexpr T operator()(T x) const noexcept {
        return std::cos(x);
    }
};

struct Tan {
    template <typename T>
    constexpr T operator()(T x) const noexcept {
        return std::tan(x);
    }
};

struct Asin {
    template <typename T>
    constexpr T operator()(T x) const noexcept {
        return std::asin(x);
    }
};

struct Acos {
    template <typename T>
    constexpr T operator()(T x) const noexcept {
        return std::acos(x);
    }
};

struct Atan {
    template <typename T>
    constexpr T operator()(T x) const noexcept {
        return std::atan(x);
    }
};

struct Atan2 {
    template <typename T>
    constexpr T operator()(T y, T x) const noexcept {
        return std::atan2(y, x);
    }
};

struct Radians {
    template <typename T>
    constexpr T operator()(T degrees) const noexcept {
        return degrees * (std::numbers::pi_v<T> / static_cast<T>(180));
    }
};

struct Degrees {
    template <typename T>
    constexpr T operator()(T radians) const noexcept {
        return radians * (static_cast<T>(180) / std::numbers::pi_v<T>);
    }
};

}  // namespace m3::detail

export namespace m3 {

// ---- sin ----

template <detail::FloatingPoint T>
[[nodiscard("pure function: discarding a trig result is likely a bug")]]
constexpr T sin(T x) noexcept {
    return std::sin(x);
}

template <int L, detail::FloatingPoint T, detail::Qualifier Q>
[[nodiscard("pure function: discarding a trig result is likely a bug")]]
constexpr Vec<L, T, Q> sin(const Vec<L, T, Q>& v) noexcept {
    return detail::apply_unary<detail::Sin, Vec<L, T, Q>>(v);
}

// ---- cos ----

template <detail::FloatingPoint T>
[[nodiscard("pure function: discarding a trig result is likely a bug")]]
constexpr T cos(T x) noexcept {
    return std::cos(x);
}

template <int L, detail::FloatingPoint T, detail::Qualifier Q>
[[nodiscard("pure function: discarding a trig result is likely a bug")]]
constexpr Vec<L, T, Q> cos(const Vec<L, T, Q>& v) noexcept {
    return detail::apply_unary<detail::Cos, Vec<L, T, Q>>(v);
}

// ---- tan ----

template <detail::FloatingPoint T>
[[nodiscard("pure function: discarding a trig result is likely a bug")]]
constexpr T tan(T x) noexcept {
    return std::tan(x);
}

template <int L, detail::FloatingPoint T, detail::Qualifier Q>
[[nodiscard("pure function: discarding a trig result is likely a bug")]]
constexpr Vec<L, T, Q> tan(const Vec<L, T, Q>& v) noexcept {
    return detail::apply_unary<detail::Tan, Vec<L, T, Q>>(v);
}

// ---- asin ----

template <detail::FloatingPoint T>
[[nodiscard("pure function: discarding a trig result is likely a bug")]]
constexpr T asin(T x) noexcept {
    return std::asin(x);
}

template <int L, detail::FloatingPoint T, detail::Qualifier Q>
[[nodiscard("pure function: discarding a trig result is likely a bug")]]
constexpr Vec<L, T, Q> asin(const Vec<L, T, Q>& v) noexcept {
    return detail::apply_unary<detail::Asin, Vec<L, T, Q>>(v);
}

// ---- acos ----

template <detail::FloatingPoint T>
[[nodiscard("pure function: discarding a trig result is likely a bug")]]
constexpr T acos(T x) noexcept {
    return std::acos(x);
}

template <int L, detail::FloatingPoint T, detail::Qualifier Q>
[[nodiscard("pure function: discarding a trig result is likely a bug")]]
constexpr Vec<L, T, Q> acos(const Vec<L, T, Q>& v) noexcept {
    return detail::apply_unary<detail::Acos, Vec<L, T, Q>>(v);
}

// ---- atan (single-argument) ----

template <detail::FloatingPoint T>
[[nodiscard("pure function: discarding a trig result is likely a bug")]]
constexpr T atan(T x) noexcept {
    return std::atan(x);
}

template <int L, detail::FloatingPoint T, detail::Qualifier Q>
[[nodiscard("pure function: discarding a trig result is likely a bug")]]
constexpr Vec<L, T, Q> atan(const Vec<L, T, Q>& v) noexcept {
    return detail::apply_unary<detail::Atan, Vec<L, T, Q>>(v);
}

// ---- atan2 (two-argument) ----

template <detail::FloatingPoint T>
[[nodiscard]]
constexpr T atan2(T y, T x) noexcept {
    return std::atan2(y, x);
}

template <int L, detail::FloatingPoint T, detail::Qualifier Q>
[[nodiscard]]
constexpr Vec<L, T, Q> atan2(const Vec<L, T, Q>& y, const Vec<L, T, Q>& x) noexcept {
    return detail::apply_binary<detail::Atan2, Vec<L, T, Q>>(y, x);
}

// ---- radians ----

template <detail::FloatingPoint T>
[[nodiscard]]
constexpr T radians(T degrees) noexcept {
    return degrees * (std::numbers::pi_v<T> / static_cast<T>(180));
}

template <int L, detail::FloatingPoint T, detail::Qualifier Q>
[[nodiscard]]
constexpr Vec<L, T, Q> radians(const Vec<L, T, Q>& v) noexcept {
    return detail::apply_unary<detail::Radians, Vec<L, T, Q>>(v);
}

// ---- degrees ----

template <detail::FloatingPoint T>
[[nodiscard]]
constexpr T degrees(T radians) noexcept {
    return radians * (static_cast<T>(180) / std::numbers::pi_v<T>);
}

template <int L, detail::FloatingPoint T, detail::Qualifier Q>
[[nodiscard]]
constexpr Vec<L, T, Q> degrees(const Vec<L, T, Q>& v) noexcept {
    return detail::apply_unary<detail::Degrees, Vec<L, T, Q>>(v);
}

}  // namespace m3
