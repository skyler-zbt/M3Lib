// GLSL 4.60 §8.2 exponential functions: pow, exp, log, exp2, log2, sqrt,
// inversesqrt.
// Each function provides a scalar overload and Vec{1,2,3,4} vector overloads;
// vector cases delegate to detail::apply_unary (one-arg) or
// detail::apply_binary (two-arg).  All overloads are constexpr + noexcept.
//
// GLSL 4.60 §8.2 指数函数：pow、exp、log、exp2、log2、sqrt、inversesqrt。
// 每个函数均提供标量重载与 Vec{1,2,3,4} 向量重载；
// 向量情况委托给 detail::apply_unary（单参）或 detail::apply_binary（双参）。
// 所有重载均为 constexpr + noexcept。
export module m3.math:exp;

import std;

import m3.detail;
import m3.vector;

namespace m3::detail {

struct Pow {
    template <typename T>
    constexpr T operator()(T x, T y) const noexcept {
        return std::pow(x, y);
    }
};

struct Exp {
    template <typename T>
    constexpr T operator()(T x) const noexcept {
        return std::exp(x);
    }
};

struct Log {
    template <typename T>
    constexpr T operator()(T x) const noexcept {
        return std::log(x);
    }
};

struct Exp2 {
    template <typename T>
    constexpr T operator()(T x) const noexcept {
        return std::exp2(x);
    }
};

struct Log2 {
    template <typename T>
    constexpr T operator()(T x) const noexcept {
        return std::log2(x);
    }
};

struct Sqrt {
    template <typename T>
    constexpr T operator()(T x) const noexcept {
        return std::sqrt(x);
    }
};

// inversesqrt — GLSL-specific: 1 / sqrt(x).  Kept as a functor so the vector
// overload reuses the same apply_unary dispatch as the other unary functions.
//
// inversesqrt —— GLSL 特有函数：1 / sqrt(x)。作为函数对象定义，
// 使向量重载能与其他一元函数复用相同的 apply_unary 分派。
struct InverseSqrt {
    template <typename T>
    constexpr T operator()(T x) const noexcept {
        return static_cast<T>(1) / std::sqrt(x);
    }
};

}  // namespace m3::detail

export namespace m3 {

// ---- pow ----

template <detail::FloatingPoint T>
[[nodiscard("pure function: discarding a pow result is likely a bug")]]
constexpr T pow(T x, T y) noexcept {
    return std::pow(x, y);
}

template <int L, detail::FloatingPoint T, detail::Qualifier Q>
[[nodiscard("pure function: discarding a pow result is likely a bug")]]
constexpr Vec<L, T, Q> pow(const Vec<L, T, Q>& x, const Vec<L, T, Q>& y) noexcept {
    return detail::apply_binary<detail::Pow, Vec<L, T, Q>>(x, y);
}

// ---- exp ----

template <detail::FloatingPoint T>
[[nodiscard("pure function: discarding an exp result is likely a bug")]]
constexpr T exp(T x) noexcept {
    return std::exp(x);
}

template <int L, detail::FloatingPoint T, detail::Qualifier Q>
[[nodiscard("pure function: discarding an exp result is likely a bug")]]
constexpr Vec<L, T, Q> exp(const Vec<L, T, Q>& v) noexcept {
    return detail::apply_unary<detail::Exp, Vec<L, T, Q>>(v);
}

// ---- log ----

template <detail::FloatingPoint T>
[[nodiscard("pure function: discarding a log result is likely a bug")]]
constexpr T log(T x) noexcept {
    return std::log(x);
}

template <int L, detail::FloatingPoint T, detail::Qualifier Q>
[[nodiscard("pure function: discarding a log result is likely a bug")]]
constexpr Vec<L, T, Q> log(const Vec<L, T, Q>& v) noexcept {
    return detail::apply_unary<detail::Log, Vec<L, T, Q>>(v);
}

// ---- exp2 ----

template <detail::FloatingPoint T>
[[nodiscard("pure function: discarding an exp2 result is likely a bug")]]
constexpr T exp2(T x) noexcept {
    return std::exp2(x);
}

template <int L, detail::FloatingPoint T, detail::Qualifier Q>
[[nodiscard("pure function: discarding an exp2 result is likely a bug")]]
constexpr Vec<L, T, Q> exp2(const Vec<L, T, Q>& v) noexcept {
    return detail::apply_unary<detail::Exp2, Vec<L, T, Q>>(v);
}

// ---- log2 ----

template <detail::FloatingPoint T>
[[nodiscard("pure function: discarding a log2 result is likely a bug")]]
constexpr T log2(T x) noexcept {
    return std::log2(x);
}

template <int L, detail::FloatingPoint T, detail::Qualifier Q>
[[nodiscard("pure function: discarding a log2 result is likely a bug")]]
constexpr Vec<L, T, Q> log2(const Vec<L, T, Q>& v) noexcept {
    return detail::apply_unary<detail::Log2, Vec<L, T, Q>>(v);
}

// ---- sqrt ----

template <detail::FloatingPoint T>
[[nodiscard("pure function: discarding a sqrt result is likely a bug")]]
constexpr T sqrt(T x) noexcept {
    return std::sqrt(x);
}

template <int L, detail::FloatingPoint T, detail::Qualifier Q>
[[nodiscard("pure function: discarding a sqrt result is likely a bug")]]
constexpr Vec<L, T, Q> sqrt(const Vec<L, T, Q>& v) noexcept {
    return detail::apply_unary<detail::Sqrt, Vec<L, T, Q>>(v);
}

// ---- inversesqrt ----

template <detail::FloatingPoint T>
[[nodiscard("pure function: discarding an inversesqrt result is likely a bug")]]
constexpr T inversesqrt(T x) noexcept {
    return static_cast<T>(1) / std::sqrt(x);
}

template <int L, detail::FloatingPoint T, detail::Qualifier Q>
[[nodiscard("pure function: discarding an inversesqrt result is likely a bug")]]
constexpr Vec<L, T, Q> inversesqrt(const Vec<L, T, Q>& v) noexcept {
    return detail::apply_unary<detail::InverseSqrt, Vec<L, T, Q>>(v);
}

}  // namespace m3
