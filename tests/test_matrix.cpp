#include "support/glm_reference.hpp"

namespace {
test::Runner runner;
using test::Q;

template <typename M>
concept MatrixDivision = requires(const M& a, const M& b) { a / b; };

template <typename M>
concept MatrixDivisionAssignment = requires(M& a, const M& b) { a /= b; };

template <typename M>
concept ScalarDivision = requires(M& a, typename M::value_type scalar) {
    { a / scalar } -> std::same_as<M>;
    { a /= scalar } -> std::same_as<M&>;
};

template <typename T, Q Qualifier>
constexpr bool matrix_api() {
    using M = m3::Mat3<T, Qualifier>;
    static_assert(!MatrixDivision<M>);
    static_assert(!MatrixDivisionAssignment<M>);
    static_assert(ScalarDivision<M>);
    static_assert(std::same_as<decltype(m3::component_multiply(std::declval<const M&>(),
                                                             std::declval<const M&>())), M>);
    static_assert(std::same_as<decltype(m3::component_divide(std::declval<const M&>(),
                                                           std::declval<const M&>())), M>);
    static_assert(noexcept(m3::component_multiply(std::declval<const M&>(),
                                                 std::declval<const M&>())));
    static_assert(noexcept(m3::component_divide(std::declval<const M&>(),
                                               std::declval<const M&>())));
    return true;
}

constexpr m3::Mat2<int> constant_product() {
    m3::Mat2<int> a, b;
    a(0, 0) = 1; a(1, 0) = 2; a(0, 1) = 3; a(1, 1) = 4;
    b(0, 0) = 5; b(1, 0) = 6; b(0, 1) = 7; b(1, 1) = 8;
    return a * b;
}

constexpr auto constant_components() {
    m3::Mat2<double> a, b;
    a(0, 0) = 1; a(1, 0) = 2; a(0, 1) = 3; a(1, 1) = 4;
    b(0, 0) = 5; b(1, 0) = 6; b(0, 1) = 7; b(1, 1) = 8;
    return std::array{m3::component_multiply(a, b), m3::component_divide(a, b)};
}

template <int N, typename T, Q Qualifier>
void products() {
    using G = test::GMat<N, T>;
    std::mt19937 generator{0x4d334cU + N};
    std::uniform_int_distribution<int> distribution{-6, 6};
    for (int sample = 0; sample < 16; ++sample) {
        G ga{T{0}}, gb{T{0}};
        test::GVec<N, T> gv{T{0}};
        for (int c = 0; c < N; ++c) {
            gv[c] = static_cast<T>(distribution(generator));
            for (int r = 0; r < N; ++r) {
                const T divisor = std::is_integral_v<T> ? T{1} : T{4};
                ga[c][r] = static_cast<T>(distribution(generator)) / divisor;
                gb[c][r] = static_cast<T>(distribution(generator)) / divisor;
            }
        }
        const auto a = test::as_m3<Qualifier>(ga), b = test::as_m3<Qualifier>(gb);
        const auto v = test::as_m3<Qualifier>(gv);
        runner.matrix(a * b, ga * gb, "A * B");
        runner.matrix(b * a, gb * ga, "B * A");
        runner.matrix(a * m3::Mat<N, N, T, Qualifier>{T{1}}, ga * G{T{1}}, "A * I");
        runner.matrix(m3::Mat<N, N, T, Qualifier>{T{1}} * a, G{T{1}} * ga, "I * A");
        runner.matrix(a * m3::Mat<N, N, T, Qualifier>{}, ga * G{T{0}}, "A * zero");
        auto compound = a; compound *= b;
        runner.matrix(compound, ga * gb, "A *= B");
        compound = a; compound *= compound;
        runner.matrix(compound, ga * ga, "A *= A alias");
        runner.vector(a * v, ga * gv, "matrix * column vector");
        if constexpr (std::is_integral_v<T>) {
            // GLM's row-vector path uses dot(), which accepts floating point only.
            // The bounded integral fixtures are exactly representable in long double.
            runner.vector(v * a, test::as_glm<long double>(v) * test::as_glm<long double>(a),
                          "integer row vector * matrix (widened GLM)");
        } else {
            runner.vector(v * a, gv * ga, "row vector * matrix");
        }
        runner.vector((a * b) * v, (ga * gb) * gv, "composed matrix-vector product");
    }
}

template <typename T, Q Qualifier>
void transforms() {
    test::GMat<4, T> translation{T{1}}, scale{T{1}};
    translation[3] = test::GVec<4, T>{T{10}, T{20}, T{30}, T{1}};
    scale[0][0] = T{2}; scale[1][1] = T{3}; scale[2][2] = T{4};
    const test::GVec<3, T> gp{T{1}, T{2}, T{3}};
    const auto p = test::as_m3<Qualifier>(gp);
    const auto mt = test::as_m3<Qualifier>(translation), ms = test::as_m3<Qualifier>(scale);
    const auto expected_point = (translation * scale) * test::GVec<4, T>{gp, T{1}};
    const auto expected_reverse = (scale * translation) * test::GVec<4, T>{gp, T{1}};
    runner.vector(m3::transform_point(mt * ms, p), test::GVec<3, T>{expected_point} / expected_point.w,
                  "translate * scale point");
    runner.vector(m3::transform_point(ms * mt, p), test::GVec<3, T>{expected_reverse} / expected_reverse.w,
                  "scale * translate point");
    runner.vector(m3::transform_direction(mt * ms, p),
                  test::GVec<3, T>{(translation * scale) * test::GVec<4, T>{gp, T{0}}},
                  "composed direction transform");
}

template <typename T, Q Qualifier> void type() {
    static_assert(matrix_api<T, Qualifier>());
    products<2, T, Qualifier>(); products<3, T, Qualifier>(); products<4, T, Qualifier>();
    if constexpr (std::is_floating_point_v<T>) transforms<T, Qualifier>();
}
template <Q Qualifier> void qualifier() {
    type<int, Qualifier>(); type<float, Qualifier>(); type<double, Qualifier>();
}
}

int main() {
    constexpr auto compiled = constant_product();
    const test::GMat<2, int> ga{1, 3, 2, 4}, gb{5, 7, 6, 8};
    runner.matrix(compiled, ga * gb, "constexpr noncommuting matrix product");
    runner.truth(ga * gb != gb * ga, "fixture matrices do not commute");
    constexpr auto components = constant_components();
    const test::GMat<2, double> gca{1, 3, 2, 4}, gcb{5, 7, 6, 8};
    runner.matrix(components[0], glm::matrixCompMult(gca, gcb),
                  "constexpr component multiplication");
    runner.matrix(components[1], test::component_divide(gca, gcb),
                  "constexpr component division");
    qualifier<Q::aligned_none>(); qualifier<Q::aligned_low>();
    qualifier<Q::aligned_medium>(); qualifier<Q::aligned_high>();
    return runner.finish("Matrix reference tests");
}
