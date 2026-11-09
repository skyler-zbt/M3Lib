#pragma once

#include <glm/glm.hpp>
#include <glm/ext/matrix_integer.hpp>
#include <glm/gtc/type_ptr.hpp>

import std;
import m3;

namespace test {

using Q = m3::detail::Qualifier;
template <int L, typename T> using GVec = glm::vec<L, T, glm::defaultp>;
template <int N, typename T> using GMat = glm::mat<N, N, T, glm::defaultp>;

template <typename R = void, int L, typename T, Q Qualifier>
constexpr auto as_glm(const m3::Vec<L, T, Qualifier>& value) {
    using Result = std::conditional_t<std::is_void_v<R>, T, R>;
    GVec<L, Result> result{Result{0}};
    for (int i = 0; i < L; ++i)
        result[i] = static_cast<Result>(value[i]);
    return result;
}

template <typename R = void, int C, int Rows, typename T, Q Qualifier>
constexpr auto as_glm(const m3::Mat<C, Rows, T, Qualifier>& value) {
    using Result = std::conditional_t<std::is_void_v<R>, T, R>;
    glm::mat<C, Rows, Result, glm::defaultp> result{Result{0}};
    for (int c = 0; c < C; ++c)
        for (int r = 0; r < Rows; ++r)
            result[c][r] = static_cast<Result>(value(c, r));
    return result;
}

template <Q Qualifier = Q::aligned_none, int L, typename T, glm::qualifier GQualifier>
constexpr auto as_m3(const glm::vec<L, T, GQualifier>& value) {
    m3::Vec<L, T, Qualifier> result;
    for (int i = 0; i < L; ++i)
        result[i] = value[i];
    return result;
}

template <Q Qualifier = Q::aligned_none, int C, int Rows, typename T, glm::qualifier GQualifier>
constexpr auto as_m3(const glm::mat<C, Rows, T, GQualifier>& value) {
    m3::Mat<C, Rows, T, Qualifier> result;
    for (int c = 0; c < C; ++c)
        for (int r = 0; r < Rows; ++r)
            result(c, r) = value[c][r];
    return result;
}

// GLM matrix/matrix division multiplies by an inverse. For M3's named
// component_divide function, ask GLM to divide corresponding column vectors.
template <int C, int Rows, typename T, glm::qualifier GQualifier>
constexpr auto component_divide(const glm::mat<C, Rows, T, GQualifier>& a,
                                const glm::mat<C, Rows, T, GQualifier>& b) {
    glm::mat<C, Rows, T, GQualifier> result{T{0}};
    for (int c = 0; c < C; ++c)
        result[c] = a[c] / b[c];
    return result;
}

struct Runner {
    std::uint64_t checks = 0;
    std::uint64_t failures = 0;

    template <typename Actual, typename Expected>
    void check(Actual actual, Expected expected, std::string_view name, bool signed_zero = false,
               bool relative_only = false) {
        ++checks;
        const long double a = static_cast<long double>(actual);
        const long double e = static_cast<long double>(expected);
        bool equal;
        if (std::isnan(e)) {
            equal = std::isnan(a);
        } else if (std::isinf(e)) {
            equal = a == e;
        } else if (!std::isfinite(a)) {
            equal = false;
        } else if constexpr (std::is_integral_v<Actual>) {
            equal = a == e;
        } else {
            const long double scale = relative_only ? std::abs(e)
                                                   : std::max(1.0L, std::abs(e));
            const long double tolerance = 64.0L * std::numeric_limits<Actual>::epsilon() * scale;
            equal = std::abs(a - e) <= tolerance;
            if (signed_zero && a == 0 && e == 0)
                equal = equal && std::signbit(a) == std::signbit(e);
        }
        if (equal)
            return;
        ++failures;
        if (failures <= 20)
            std::cerr << name << ": GLM=" << e << ", M3=" << a << '\n';
    }

    template <typename Actual, typename Expected>
    void relative(Actual actual, Expected expected, std::string_view name) {
        check(actual, expected, name, false, true);
    }

    template <int L, typename Actual, Q Qualifier, typename Expected, glm::qualifier GQualifier>
    void vector(const m3::Vec<L, Actual, Qualifier>& actual,
                const glm::vec<L, Expected, GQualifier>& expected,
                std::string_view name, bool signed_zero = false) {
        for (int i = 0; i < L; ++i)
            check(actual[i], expected[i], std::string{name} + "[" + std::to_string(i) + "]", signed_zero);
    }

    template <int C, int Rows, typename Actual, Q Qualifier, typename Expected, glm::qualifier GQualifier>
    void matrix(const m3::Mat<C, Rows, Actual, Qualifier>& actual,
                const glm::mat<C, Rows, Expected, GQualifier>& expected, std::string_view name) {
        for (int c = 0; c < C; ++c)
            for (int r = 0; r < Rows; ++r)
                check(actual(c, r), expected[c][r], std::string{name} + "["
                      + std::to_string(c) + "][" + std::to_string(r) + "]");
    }

    void truth(bool condition, std::string_view name) {
        check(condition, true, name);
    }

    int finish(std::string_view name) const {
        std::cout << name << " (GLM " << GLM_VERSION_MAJOR << '.' << GLM_VERSION_MINOR
                  << '.' << GLM_VERSION_PATCH << "): " << checks << " checks, "
                  << failures << " failures\n";
        return failures == 0 ? 0 : 1;
    }
};

}
