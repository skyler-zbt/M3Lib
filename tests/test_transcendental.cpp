#include "support/glm_reference.hpp"

namespace {
using test::Q;
test::Runner runner;

template <int L, typename T, Q Qualifier>
void functions() {
    using G = test::GVec<L, T>;
    for (int sample = 0; sample < 12; ++sample) {
        G ga{T{0}}, gu{T{0}}, gp{T{1}}, ge{T{0}}, gy{T{1}}, gx{T{1}};
        for (int i = 0; i < L; ++i) {
            const int index = (sample + i) % 8;
            ga[i] = static_cast<T>(index - 4) / T{4};
            gu[i] = static_cast<T>(index - 4) / T{4};
            gp[i] = static_cast<T>(index + 1) / T{4};
            ge[i] = static_cast<T>(index - 3) / T{2};
            gy[i] = index % 2 == 0 ? gp[i] : -gp[i];
            gx[i] = index % 3 == 0 ? -gp[i] : gp[i];
        }
        const auto a = test::as_m3<Qualifier>(ga), u = test::as_m3<Qualifier>(gu);
        const auto p = test::as_m3<Qualifier>(gp), e = test::as_m3<Qualifier>(ge);
        const auto y = test::as_m3<Qualifier>(gy), x = test::as_m3<Qualifier>(gx);
        runner.vector(m3::sin(a), glm::sin(ga), "sin vector");
        runner.vector(m3::cos(a), glm::cos(ga), "cos vector");
        runner.vector(m3::tan(a), glm::tan(ga), "tan vector");
        runner.vector(m3::asin(u), glm::asin(gu), "asin vector");
        runner.vector(m3::acos(u), glm::acos(gu), "acos vector");
        runner.vector(m3::atan(a), glm::atan(ga), "atan vector");
        runner.vector(m3::atan2(y, x), glm::atan(gy, gx), "atan2 vector uses GLM atan(y,x)");
        runner.vector(m3::pow(p, e), glm::pow(gp, ge), "pow vector");
        runner.vector(m3::exp(a), glm::exp(ga), "exp vector");
        runner.vector(m3::exp2(e), glm::exp2(ge), "exp2 vector");
        runner.vector(m3::log(p), glm::log(gp), "log vector");
        runner.vector(m3::log2(p), glm::log2(gp), "log2 vector");
        runner.vector(m3::sqrt(p), glm::sqrt(gp), "sqrt vector");
        runner.vector(m3::inversesqrt(p), glm::inversesqrt(gp), "inversesqrt vector");
        if constexpr (!std::is_same_v<T, long double>) {
            runner.vector(m3::radians(a), glm::radians(ga), "radians vector");
            runner.vector(m3::degrees(a), glm::degrees(ga), "degrees vector");
        }
        for (int i = 0; i < L; ++i) {
            runner.check(m3::sin(a[i]), glm::sin(ga[i]), "sin scalar");
            runner.check(m3::cos(a[i]), glm::cos(ga[i]), "cos scalar");
            runner.check(m3::tan(a[i]), glm::tan(ga[i]), "tan scalar");
            runner.check(m3::asin(u[i]), glm::asin(gu[i]), "asin scalar");
            runner.check(m3::acos(u[i]), glm::acos(gu[i]), "acos scalar");
            runner.check(m3::atan(a[i]), glm::atan(ga[i]), "atan scalar");
            runner.check(m3::atan2(y[i], x[i]), glm::atan(gy[i], gx[i]), "atan2 scalar");
            runner.check(m3::pow(p[i], e[i]), glm::pow(gp[i], ge[i]), "pow scalar");
            runner.check(m3::exp(a[i]), glm::exp(ga[i]), "exp scalar");
            runner.check(m3::exp2(e[i]), glm::exp2(ge[i]), "exp2 scalar");
            runner.check(m3::log(p[i]), glm::log(gp[i]), "log scalar");
            runner.check(m3::log2(p[i]), glm::log2(gp[i]), "log2 scalar");
            runner.check(m3::sqrt(p[i]), glm::sqrt(gp[i]), "sqrt scalar");
            runner.check(m3::inversesqrt(p[i]), glm::inversesqrt(gp[i]), "inversesqrt scalar");
            if constexpr (std::is_same_v<T, long double>) {
                // This GLM version's angle-conversion coefficients are double literals.
                runner.check(static_cast<double>(m3::radians(a[i])), glm::radians(static_cast<double>(ga[i])),
                             "long double radians at GLM coefficient precision");
                runner.check(static_cast<double>(m3::degrees(a[i])), glm::degrees(static_cast<double>(ga[i])),
                             "long double degrees at GLM coefficient precision");
            } else {
                runner.check(m3::radians(a[i]), glm::radians(ga[i]), "radians scalar");
                runner.check(m3::degrees(a[i]), glm::degrees(ga[i]), "degrees scalar");
            }
        }
    }
    const T zero = T{0}, negative_zero = -zero;
    runner.check(m3::atan2(zero, T{-1}), glm::atan(zero, T{-1}), "atan2 positive-zero quadrant", true);
    runner.check(m3::atan2(negative_zero, T{-1}), glm::atan(negative_zero, T{-1}), "atan2 negative-zero quadrant", true);
    runner.check(m3::atan2(negative_zero, T{1}), glm::atan(negative_zero, T{1}), "atan2 signed zero", true);
}

template <typename T, Q Qualifier>
void type() {
    functions<1, T, Qualifier>(); functions<2, T, Qualifier>();
    functions<3, T, Qualifier>(); functions<4, T, Qualifier>();
}

template <Q Qualifier>
void qualifier() {
    type<float, Qualifier>(); type<double, Qualifier>(); type<long double, Qualifier>();
}
}

int main() {
    qualifier<Q::aligned_none>(); qualifier<Q::aligned_low>();
    qualifier<Q::aligned_medium>(); qualifier<Q::aligned_high>();
    return runner.finish("Transcendental GLM reference");
}