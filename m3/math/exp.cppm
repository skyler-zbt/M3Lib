
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

struct InverseSqrt {
    template <typename T>
    constexpr T operator()(T x) const noexcept {
        return static_cast<T>(1) / std::sqrt(x);
    }
};

}

export namespace m3 {

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

}
