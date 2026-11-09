#include "support/glm_reference.hpp"

namespace {
using test::Q;
test::Runner runner;

template <int L, typename T, Q Qualifier>
void floating() {
    using G = test::GVec<L, T>;
    constexpr T values[] = {T{-3.5}, T{-2}, T{-1.25}, T{-0.0}, T{0}, T{0.25}, T{1.5}, T{2}, T{3.75}};
    constexpr T divisors[] = {T{-2}, T{-0.5}, T{0.5}, T{2}};
    for (int sample = 0; sample < 9; ++sample) {
        G gx{T{0}}, gy{T{0}}, gd{T{1}};
        for (int i = 0; i < L; ++i) {
            gx[i] = values[(sample + i) % 9];
            gy[i] = values[(sample + 2 * i + 3) % 9];
            gd[i] = divisors[(sample + i) % 4];
        }
        const auto x = test::as_m3<Qualifier>(gx);
        const auto y = test::as_m3<Qualifier>(gy);
        const auto d = test::as_m3<Qualifier>(gd);
        runner.vector(m3::abs(x), glm::abs(gx), "abs vector");
        runner.vector(m3::sign(x), glm::sign(gx), "sign vector");
        runner.vector(m3::floor(x), glm::floor(gx), "floor vector");
        runner.vector(m3::ceil(x), glm::ceil(gx), "ceil vector");
        runner.vector(m3::fract(x), glm::fract(gx), "fract vector");
        runner.vector(m3::clamp(x, T{-1}, T{2}), glm::clamp(gx, T{-1}, T{2}), "clamp vector");
        runner.vector(m3::min(x, y), glm::min(gx, gy), "min vector");
        runner.vector(m3::max(x, y), glm::max(gx, gy), "max vector");
        runner.vector(m3::min(x, T{1}), glm::min(gx, T{1}), "min scalar bound");
        runner.vector(m3::max(x, T{1}), glm::max(gx, T{1}), "max scalar bound");
        runner.vector(m3::mod(x, d), glm::mod(gx, gd), "mod vector");
        runner.vector(m3::mod(x, T{2}), glm::mod(gx, T{2}), "mod scalar divisor");
        runner.vector(m3::step(T{0}, x), glm::step(T{0}, gx), "step vector");
        runner.vector(m3::smoothstep(T{-2}, T{2}, x), glm::smoothstep(T{-2}, T{2}, gx), "smoothstep vector");
        for (int i = 0; i < L; ++i) {
            runner.check(m3::abs(x[i]), glm::abs(gx[i]), "abs scalar");
            runner.check(m3::sign(x[i]), glm::sign(gx[i]), "sign scalar");
            runner.check(m3::floor(x[i]), glm::floor(gx[i]), "floor scalar");
            runner.check(m3::ceil(x[i]), glm::ceil(gx[i]), "ceil scalar");
            runner.check(m3::fract(x[i]), glm::fract(gx[i]), "fract scalar");
            runner.check(m3::clamp(x[i], T{-1}, T{2}), glm::clamp(gx[i], T{-1}, T{2}), "clamp scalar");
            runner.check(m3::min(x[i], y[i]), glm::min(gx[i], gy[i]), "min scalar");
            runner.check(m3::max(x[i], y[i]), glm::max(gx[i], gy[i]), "max scalar");
            runner.check(m3::mod(x[i], d[i]), glm::mod(gx[i], gd[i]), "mod scalar");
            runner.check(m3::step(T{0}, x[i]), glm::step(T{0}, gx[i]), "step scalar");
            runner.check(m3::smoothstep(T{-2}, T{2}, x[i]),
                         glm::smoothstep(T{-2}, T{2}, gx[i]), "smoothstep scalar");
        }
        for (T factor : {T{-0.5}, T{0}, T{0.25}, T{0.5}, T{1}, T{1.5}}) {
            const auto expected = glm::mix(gx, gy, factor);
            runner.vector(m3::mix(x, y, factor), expected, "mix vector");
            runner.vector(m3::lerp(x, y, factor), expected, "lerp vector uses GLM mix");
            for (int i = 0; i < L; ++i) {
                runner.check(m3::mix(x[i], y[i], factor), glm::mix(gx[i], gy[i], factor), "mix scalar");
                runner.check(m3::lerp(x[i], y[i], factor), glm::mix(gx[i], gy[i], factor), "lerp scalar");
            }
        }
    }
    constexpr T constant_mod = m3::mod(T{1}, static_cast<T>(0.1));
    volatile T runtime_one = T{1}, runtime_tenth = static_cast<T>(0.1);
    runner.check(constant_mod, glm::mod(T{1}, static_cast<T>(0.1)), "constexpr mod GLM");
    runner.check(m3::mod(static_cast<T>(runtime_one), static_cast<T>(runtime_tenth)),
                 glm::mod(T{1}, static_cast<T>(0.1)), "runtime mod GLM");

    // Widen GLM's own functions, rather than duplicating their formulas.
    if constexpr (std::numeric_limits<long double>::max_exponent
                  > 2 * std::numeric_limits<T>::max_exponent) {
        const T largest = std::numeric_limits<T>::max();
        const T half = largest / T{2};
        const long double wide = largest;
        for (T input : {-largest, -half, T{0}, half, largest}) {
            const auto x = test::as_m3<Qualifier>(G{input});
            runner.vector(m3::smoothstep(-largest, largest, x),
                          glm::smoothstep(-wide, wide, test::GVec<L, long double>{input}),
                          "wide smoothstep GLM long double");
            runner.check(m3::smoothstep(-largest, largest, input),
                         glm::smoothstep(-wide, wide, static_cast<long double>(input)),
                         "wide smoothstep scalar GLM long double");
        }
        for (T input : {-largest, largest}) {
            for (T divisor : {T{-0.25}, T{0.25}}) {
                const auto x = test::as_m3<Qualifier>(G{input});
                const auto d = test::as_m3<Qualifier>(G{divisor});
                const auto expected = glm::mod(test::GVec<L, long double>{input},
                                               test::GVec<L, long double>{divisor});
                runner.vector(m3::mod(x, d), expected, "wide mod vector GLM long double");
                runner.vector(m3::mod(x, divisor), expected, "wide mod scalar divisor GLM long double");
                runner.check(m3::mod(input, divisor),
                             glm::mod(static_cast<long double>(input), static_cast<long double>(divisor)),
                             "wide mod scalar GLM long double");
            }
        }
        const T divisor = largest * T{0.75};
        runner.check(m3::mod(-largest, divisor),
                     glm::mod(-wide, static_cast<long double>(divisor)),
                     "mod product range GLM long double");
        const T ulp = std::ldexp(T{1}, std::numeric_limits<T>::max_exponent - std::numeric_limits<T>::digits);
        const T edge0 = T{-1.5} * ulp;
        const T edge1 = largest - ulp;
        const T input = edge1 / T{2};
        runner.check(m3::smoothstep(edge0, edge1, input),
                     glm::smoothstep(static_cast<long double>(edge0),
                                     static_cast<long double>(edge1), static_cast<long double>(input)),
                     "smoothstep threshold tie GLM long double");
        constexpr T constant_wide = m3::smoothstep(-std::numeric_limits<T>::max(),
                                                   std::numeric_limits<T>::max(), T{0});
        constexpr T constant_remainder = m3::mod(std::numeric_limits<T>::max(), T{0.25});
        runner.check(constant_wide, glm::smoothstep(-wide, wide, 0.0L), "constexpr wide smoothstep GLM");
        runner.check(constant_remainder, glm::mod(wide, 0.25L), "constexpr wide mod GLM");
    } else if constexpr (L == 1 && Qualifier == Q::aligned_none) {
        std::cout << "SKIP wide common functions: long double exponent range is insufficient\n";
    }
}

template <int L, typename T, Q Qualifier>
void integral() {
    test::GVec<L, T> gx{T{0}}, gy{T{0}};
    for (int i = 0; i < L; ++i) {
        gx[i] = static_cast<T>(std::is_signed_v<T> ? i - 2 : i);
        gy[i] = static_cast<T>(i + 3);
    }
    const auto x = test::as_m3<Qualifier>(gx);
    const auto y = test::as_m3<Qualifier>(gy);
    const auto wide_x = test::as_glm<long double>(x);
    const auto wide_y = test::as_glm<long double>(y);
    runner.vector(m3::abs(x), glm::abs(wide_x), "integer abs GLM widened");
    runner.vector(m3::sign(x), glm::sign(wide_x), "integer sign GLM widened");
    runner.vector(m3::min(x, y), glm::min(gx, gy), "integer min GLM");
    runner.vector(m3::max(x, y), glm::max(gx, gy), "integer max GLM");
    runner.vector(m3::clamp(x, T{0}, T{2}), glm::clamp(gx, T{0}, T{2}), "integer clamp GLM");
    runner.vector(m3::mix(x, y, T{0}), glm::mix(wide_x, wide_y, 0.0L), "integer mix extension GLM widened");
    runner.vector(m3::lerp(x, y, T{1}), glm::mix(wide_x, wide_y, 1.0L), "integer lerp extension GLM widened");
    for (int i = 0; i < L; ++i) {
        runner.check(m3::abs(x[i]), glm::abs(wide_x[i]), "integer scalar abs GLM");
        runner.check(m3::sign(x[i]), glm::sign(wide_x[i]), "integer scalar sign GLM");
        runner.check(m3::clamp(x[i], T{0}, T{2}), glm::clamp(gx[i], T{0}, T{2}), "integer scalar clamp GLM");
    }
}

template <int L, Q Qualifier>
void dimension() {
    floating<L, float, Qualifier>();
    floating<L, double, Qualifier>();
    integral<L, int, Qualifier>();
    integral<L, unsigned, Qualifier>();
}

template <Q Qualifier>
void qualifier() {
    dimension<1, Qualifier>(); dimension<2, Qualifier>();
    dimension<3, Qualifier>(); dimension<4, Qualifier>();
}
}

int main() {
    qualifier<Q::aligned_none>(); qualifier<Q::aligned_low>();
    qualifier<Q::aligned_medium>(); qualifier<Q::aligned_high>();
    return runner.finish("Common math GLM reference");
}