
export module m3.math:vector;

import std;

import m3.detail;
import m3.vector;

export namespace m3 {

template <int L, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard("pure function: discarding a dot product is likely a bug")]] constexpr T
dot(const Vec<L, T, Q>& a, const Vec<L, T, Q>& b) noexcept {
    if constexpr (L == 1) {
        return a[0] * b[0];
    } else if constexpr (L == 2) {
        return a[0] * b[0] + a[1] * b[1];
    } else if constexpr (L == 3) {
        return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
    } else if constexpr (L == 4) {
        return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
    } else {
        T result{};
        for (int i = 0; i < L; ++i)
            result += a[i] * b[i];
        return result;
    }
}

template <detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard("pure function: discarding a cross product is likely a bug")]] constexpr Vec<3, T, Q>
cross(const Vec<3, T, Q>& a, const Vec<3, T, Q>& b) noexcept {
    return Vec<3, T, Q>{a.y() * b.z() - a.z() * b.y(), a.z() * b.x() - a.x() * b.z(),
                        a.x() * b.y() - a.y() * b.x()};
}

template <int L, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard("pure function: discarding a length computation is likely a bug")]] constexpr auto
length(const Vec<L, T, Q>& v) noexcept -> decltype(std::sqrt(dot(v, v))) {
    if constexpr (std::is_integral_v<T>) {
        // The result is floating point: widen before multiplying integral components.
        using result_type = decltype(std::sqrt(T{}));
        result_type squared_length{};
        for (int i = 0; i < L; ++i) {
            const auto component = static_cast<result_type>(v[i]);
            squared_length += component * component;
        }
        return std::sqrt(squared_length);
    } else {
        if (!std::is_constant_evaluated()) {
            const T squared_length = dot(v, v);
            if (std::isnormal(squared_length)) {
                return std::sqrt(squared_length);
            }
        }
        // Avoid losing representable norms to overflow or underflow of the squares.
        T result{};
        for (int i = 0; i < L; ++i) {
            result = std::hypot(result, v[i]);
        }
        return result;
    }
}

template <int L, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard("pure function: discarding a distance computation is likely a bug")]] constexpr auto
distance(const Vec<L, T, Q>& a, const Vec<L, T, Q>& b) noexcept -> decltype(length(a - b)) {
    if constexpr (std::is_integral_v<T>) {
        using result_type = decltype(length(a));
        Vec<L, result_type, Q> difference;
        for (int i = 0; i < L; ++i) {
            if constexpr (std::is_same_v<T, bool>) {
                difference[i] = a[i] != b[i] ? result_type{1} : result_type{0};
            } else {
                // Unsigned magnitudes hold even the full signed min-to-max span.
                // Subtract before floating conversion so adjacent 64-bit values stay distinct.
                using unsigned_type = std::make_unsigned_t<T>;
                const auto first = static_cast<unsigned_type>(a[i]);
                const auto second = static_cast<unsigned_type>(b[i]);
                const auto magnitude = a[i] >= b[i]
                    ? static_cast<unsigned_type>(first - second)
                    : static_cast<unsigned_type>(second - first);
                difference[i] = static_cast<result_type>(magnitude);
            }
        }
        return length(difference);
    } else {
        return length(a - b);
    }
}

template <int L, detail::FloatingPoint T, detail::Qualifier Q>
[[nodiscard("pure function: discarding a normalized vector is likely a bug; use normalize() to "
            "produce a unit vector")]] constexpr Vec<L, T, Q>
normalize(const Vec<L, T, Q>& v) noexcept {
    if (!std::is_constant_evaluated()) {
        const T squared_length = dot(v, v);
        if (std::isnormal(squared_length)) {
            return v / std::sqrt(squared_length);
        }
    }
    T scale{};
    for (int i = 0; i < L; ++i) {
        if (!std::isfinite(v[i])) {
            return v / length(v);
        }
        scale = std::max(scale, std::abs(v[i]));
    }
    if (scale == T{0}) {
        return Vec<L, T, Q>{T{0}};
    }
    // Scaling first also handles finite vectors whose unscaled norm cannot fit in T.
    const auto scaled = v / scale;
    return scaled / std::sqrt(dot(scaled, scaled));
}

template <int L, detail::Arithmetic T, detail::Qualifier Q>
[[nodiscard("pure function: discarding a reflected vector is likely a bug")]] constexpr Vec<L, T, Q>
reflect(const Vec<L, T, Q>& I, const Vec<L, T, Q>& N) noexcept {
    return I - N * static_cast<T>(2) * dot(N, I);
}

template <int L, detail::FloatingPoint T, detail::Qualifier Q>
[[nodiscard("pure function: discarding a refracted vector is likely a bug")]] constexpr Vec<L, T, Q>
refract(const Vec<L, T, Q>& I, const Vec<L, T, Q>& N, T eta) noexcept {
    auto d = dot(N, I);
    auto k = static_cast<T>(1) - eta * eta * (static_cast<T>(1) - d * d);
    if (k < static_cast<T>(0)) {
        return Vec<L, T, Q>{static_cast<T>(0)};
    }
    return I * eta - N * (eta * d + std::sqrt(k));
}

}
