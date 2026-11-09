#include "support/glm_reference.hpp"

namespace {
using test::Q;
test::Runner runner;

template <int L, typename T, Q Qualifier>
void vectors() {
    test::GVec<L, T> ga{T{0}}, gb{T{0}}, gn{T{0}};
    constexpr int a_values[] = {2, -3, 4, -5};
    constexpr int b_values[] = {-4, 5, 2, -3};
    for (int i = 0; i < L; ++i) {
        ga[i] = static_cast<T>(a_values[i]);
        gb[i] = static_cast<T>(b_values[i]);
    }
    gn[0] = T{1};
    const auto a = test::as_m3<Qualifier>(ga);
    const auto b = test::as_m3<Qualifier>(gb);
    const auto n = test::as_m3<Qualifier>(gn);
    if constexpr (std::is_integral_v<T>) {
        const auto wa = test::as_glm<long double>(a);
        const auto wb = test::as_glm<long double>(b);
        const auto wn = test::as_glm<long double>(n);
        runner.check(m3::dot(a, b), glm::dot(wa, wb), "integer dot GLM widened");
        runner.check(m3::length(a), glm::length(wa), "integer length GLM widened");
        runner.check(m3::distance(a, b), glm::distance(wa, wb), "integer distance GLM widened");
        runner.vector(m3::reflect(a, n), glm::reflect(wa, wn), "integer reflect GLM widened");
    } else {
        runner.check(m3::dot(a, b), glm::dot(ga, gb), "dot GLM");
        runner.check(m3::length(a), glm::length(ga), "length GLM");
        runner.check(m3::distance(a, b), glm::distance(ga, gb), "distance GLM");
        runner.check(m3::distance(b, a), glm::distance(gb, ga), "reverse distance GLM");
        runner.vector(m3::reflect(a, n), glm::reflect(ga, gn), "reflect GLM");
        runner.vector(m3::normalize(a), glm::normalize(ga), "normalize GLM");
        runner.check(m3::length(m3::normalize(a)), glm::length(glm::normalize(ga)), "normalized length GLM");
        // M3 defines this result; GLM's normalize(zero) is not the reference.
        runner.vector(m3::normalize(m3::Vec<L, T, Qualifier>{}),
                      test::GVec<L, T>{T{0}}, "M3 policy: normalize zero");
        test::GVec<L, T> gi{T{0}};
        gi[0] = T{-1};
        runner.vector(m3::refract(test::as_m3<Qualifier>(gi), n, T{0.5}),
                      glm::refract(gi, gn, T{0.5}), "normal incidence refract GLM");
        if constexpr (L >= 2) {
            gi[0] = static_cast<T>(0.6); gi[1] = static_cast<T>(-0.8);
            gn[0] = T{0}; gn[1] = T{1};
            runner.vector(m3::refract(test::as_m3<Qualifier>(gi), test::as_m3<Qualifier>(gn), T{0.5}),
                          glm::refract(gi, gn, T{0.5}), "oblique refract GLM");
            runner.vector(m3::refract(test::as_m3<Qualifier>(gi), test::as_m3<Qualifier>(gn), T{2}),
                          glm::refract(gi, gn, T{2}), "total internal reflection GLM");
        }
    }
}

template <typename T, Q Qualifier>
void cross() {
    const test::GVec<3, T> ga{T{2}, T{-3}, T{4}}, gb{T{-4}, T{5}, T{2}};
    const auto a = test::as_m3<Qualifier>(ga), b = test::as_m3<Qualifier>(gb);
    if constexpr (std::is_integral_v<T>) {
        runner.vector(m3::cross(a, b), glm::cross(test::as_glm<long double>(a), test::as_glm<long double>(b)),
                      "integer cross GLM widened");
        runner.vector(m3::cross(b, a), glm::cross(test::as_glm<long double>(b), test::as_glm<long double>(a)),
                      "reverse integer cross GLM widened");
    } else {
        const auto expected = glm::cross(ga, gb);
        runner.vector(m3::cross(a, b), expected, "cross GLM");
        runner.vector(m3::cross(b, a), glm::cross(gb, ga), "reverse cross GLM");
        runner.check(m3::dot(m3::cross(a, b), a), glm::dot(expected, ga), "cross orthogonality GLM");
    }
}

template <int L, typename T, Q Qualifier>
void extremes() {
    if constexpr (std::numeric_limits<long double>::max_exponent
                  > 2 * std::numeric_limits<T>::max_exponent) {
        using V = m3::Vec<L, T, Qualifier>;
        const V large{std::numeric_limits<T>::max() / T{4}};
        const V small{std::numeric_limits<T>::min() * T{4}};
        const V overflowing{std::numeric_limits<T>::max()};
        const auto wide_large = test::as_glm<long double>(large);
        const auto wide_small = test::as_glm<long double>(small);
        const auto wide_overflowing = test::as_glm<long double>(overflowing);
        const test::GVec<L, long double> wide_zero{0.0L};
        runner.relative(m3::length(large), glm::length(wide_large), "large length GLM widened");
        runner.relative(m3::length(small), glm::length(wide_small), "small length GLM widened");
        runner.relative(m3::distance(large, V{}), glm::distance(wide_large, wide_zero), "large distance GLM widened");
        runner.relative(m3::distance(small, V{}), glm::distance(wide_small, wide_zero), "small distance GLM widened");
        runner.vector(m3::normalize(large), glm::normalize(wide_large), "large normalize GLM widened");
        runner.vector(m3::normalize(small), glm::normalize(wide_small), "small normalize GLM widened");
        runner.vector(m3::normalize(overflowing), glm::normalize(wide_overflowing), "overflowing raw norm GLM widened");
        V subnormal;
        subnormal[0] = std::numeric_limits<T>::denorm_min();
        const auto wide_subnormal = test::as_glm<long double>(subnormal);
        runner.relative(m3::length(subnormal), glm::length(wide_subnormal), "subnormal length GLM widened");
        runner.vector(m3::normalize(subnormal), glm::normalize(wide_subnormal), "subnormal normalize GLM widened");
    } else if constexpr (L == 1 && Qualifier == Q::aligned_none) {
        std::cout << "SKIP wide norms: long double exponent range is insufficient\n";
    }
}

template <typename T, Q Qualifier>
void span(const m3::Vec<1, T, Qualifier>& a, const m3::Vec<1, T, Qualifier>& b, std::string_view name) {
    const auto wa = test::as_glm<long double>(a), wb = test::as_glm<long double>(b);
    runner.check(m3::distance(a, b), glm::distance(wa, wb), name);
    runner.check(m3::distance(b, a), glm::distance(wb, wa), name);
}

template <Q Qualifier>
void integral_extremes() {
    const m3::Vec<2, int, Qualifier> large{50000, 0};
    runner.check(m3::length(large), glm::length(test::as_glm<long double>(large)), "int length range GLM");
    span(m3::Vec<1, int, Qualifier>{std::numeric_limits<int>::lowest()},
         m3::Vec<1, int, Qualifier>{std::numeric_limits<int>::max()}, "signed span GLM");
    span(m3::Vec<1, unsigned, Qualifier>{0u}, m3::Vec<1, unsigned, Qualifier>{65536u}, "unsigned span GLM");
    span(m3::Vec<1, signed char, Qualifier>{std::numeric_limits<signed char>::lowest()},
         m3::Vec<1, signed char, Qualifier>{std::numeric_limits<signed char>::max()}, "signed narrow span GLM");
    span(m3::Vec<1, unsigned char, Qualifier>{static_cast<unsigned char>(0)},
         m3::Vec<1, unsigned char, Qualifier>{std::numeric_limits<unsigned char>::max()}, "unsigned narrow span GLM");
    if constexpr (std::numeric_limits<long double>::digits >= 64) {
        const auto maximum = std::numeric_limits<std::uint64_t>::max();
        span(m3::Vec<1, std::uint64_t, Qualifier>{maximum},
             m3::Vec<1, std::uint64_t, Qualifier>{maximum - 1}, "adjacent uint64 GLM");
        span(m3::Vec<1, std::int64_t, Qualifier>{std::numeric_limits<std::int64_t>::lowest()},
             m3::Vec<1, std::int64_t, Qualifier>{std::numeric_limits<std::int64_t>::max()}, "signed64 span GLM");
    } else if constexpr (Qualifier == Q::aligned_none) {
        std::cout << "SKIP uint64 adjacency: long double has fewer than 64 significand bits\n";
    }
    const m3::Vec<4, bool, Qualifier> a{true, true, false, true}, b{false, true, true, false};
    runner.check(m3::length(a), glm::length(test::as_glm<long double>(a)), "bool length extension GLM widened");
    runner.check(m3::distance(a, b),
                 glm::distance(test::as_glm<long double>(a), test::as_glm<long double>(b)),
                 "bool distance extension GLM widened");
}

template <typename T, Q Qualifier>
void transforms() {
    test::GMat<4, T> gm{T{1}};
    gm[0][0] = T{2}; gm[1][1] = T{3}; gm[2][2] = T{4};
    gm[3][0] = T{10}; gm[3][1] = T{20}; gm[3][2] = T{30};
    const test::GVec<3, T> gp{T{1}, T{2}, T{3}};
    const auto p = test::as_m3<Qualifier>(gp);
    auto matrix = test::as_m3<Qualifier>(gm);
    const auto homogeneous = gm * test::GVec<4, T>{gp, T{1}};
    runner.vector(m3::transform_point(matrix, p), test::GVec<3, T>{homogeneous} / homogeneous.w,
                  "affine transform GLM");
    runner.vector(m3::transform_direction(matrix, p),
                  test::GVec<3, T>{gm * test::GVec<4, T>{gp, T{0}}}, "direction transform GLM");
    gm[2][3] = T{1};
    matrix = test::as_m3<Qualifier>(gm);
    const auto projected = gm * test::GVec<4, T>{gp, T{1}};
    runner.vector(m3::transform_point(matrix, p), test::GVec<3, T>{projected} / projected.w,
                  "projective divide GLM");
    gm[3][3] = T{-3};
    matrix = test::as_m3<Qualifier>(gm);
    runner.vector(m3::transform_point(matrix, p),
                  test::GVec<3, T>{gm * test::GVec<4, T>{gp, T{1}}}, "M3 policy: zero w returns xyz");
}

template <typename T, Q Qualifier>
void type() {
    vectors<1, T, Qualifier>(); vectors<2, T, Qualifier>();
    vectors<3, T, Qualifier>(); vectors<4, T, Qualifier>();
    cross<T, Qualifier>();
    if constexpr (std::is_floating_point_v<T>) {
        extremes<1, T, Qualifier>(); extremes<2, T, Qualifier>();
        extremes<3, T, Qualifier>(); extremes<4, T, Qualifier>();
        transforms<T, Qualifier>();
    }
}

template <Q Qualifier>
void qualifier() {
    type<int, Qualifier>(); type<float, Qualifier>(); type<double, Qualifier>();
    integral_extremes<Qualifier>();
}

void constant_results() {
    constexpr auto integer_length = m3::length(m3::Vec<2, int>{50000, 0});
    runner.check(integer_length, glm::length(test::GVec<2, long double>{50000.0L, 0.0L}), "constexpr length GLM");
    if constexpr (std::numeric_limits<long double>::max_exponent
                  > 2 * std::numeric_limits<float>::max_exponent) {
        constexpr auto normal = m3::normalize(m3::Vec<2, float>{std::numeric_limits<float>::max(), 0.0f});
        runner.vector(normal, glm::normalize(test::GVec<2, long double>{std::numeric_limits<float>::max(), 0.0L}),
                      "constexpr normalize GLM widened");
        constexpr auto tiny_length = m3::length(m3::Vec<1, float>{std::numeric_limits<float>::denorm_min()});
        runner.relative(tiny_length, glm::length(test::GVec<1, long double>{std::numeric_limits<float>::denorm_min()}),
                        "constexpr subnormal length GLM widened");
    }
}
}

int main() {
    qualifier<Q::aligned_none>(); qualifier<Q::aligned_low>();
    qualifier<Q::aligned_medium>(); qualifier<Q::aligned_high>();
    constant_results();
    return runner.finish("Geometry GLM reference");
}
