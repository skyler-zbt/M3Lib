#include "support/glm_reference.hpp"

namespace {
test::Runner runner;
using test::Q;

template <typename M>
concept HasInverse = requires(const M& matrix) { m3::inverse(matrix); };

static_assert(!HasInverse<m3::Mat2<int>>);
static_assert(!HasInverse<m3::Mat3<unsigned>>);
static_assert(!HasInverse<m3::Mat4<bool>>);

template <int N, typename T, Q Qualifier>
void compare(const test::GMat<N, T>& input, std::string_view name) {
    using M = m3::Mat<N, N, T, Qualifier>;
    static_assert(HasInverse<M>);
    static_assert(std::same_as<decltype(m3::inverse(std::declval<const M&>())), M>);
    static_assert(noexcept(m3::inverse(std::declval<const M&>())));
    const M matrix = test::as_m3<Qualifier>(input);
    const auto result = m3::inverse(matrix);
    runner.matrix(result, glm::inverse(input), name);
    runner.matrix(matrix * result, test::GMat<N, T>{T{1}},
                  std::string{name} + " right identity residual");
    runner.matrix(result * matrix, test::GMat<N, T>{T{1}},
                  std::string{name} + " left identity residual");
    runner.matrix(matrix, input, std::string{name} + " preserves input");
}

template <int N, typename T, Q Qualifier>
void matrices() {
    using G = test::GMat<N, T>;
    compare<N, T, Qualifier>(G{T{1}}, "identity inverse");

    G diagonal{T{1}};
    for (int i = 0; i < N; ++i)
        diagonal[i][i] = static_cast<T>((i + 1) * (i % 2 == 0 ? 1 : -1));
    compare<N, T, Qualifier>(diagonal, "signed diagonal inverse");

    // A zero leading diagonal forces a row exchange despite an invertible matrix.
    G pivot{T{1}};
    pivot[0][0] = T{0}; pivot[1][0] = T{2};
    pivot[0][1] = T{4}; pivot[1][1] = T{0};
    compare<N, T, Qualifier>(pivot, "row pivot inverse");

    std::mt19937 generator{0x494e5652U + N};
    std::uniform_int_distribution<int> distribution{-4, 4};
    for (int sample = 0; sample < 24; ++sample) {
        G input{T{0}};
        for (int c = 0; c < N; ++c)
            for (int r = 0; r < N; ++r)
                input[c][r] = static_cast<T>(distribution(generator)) / T{8};
        for (int i = 0; i < N; ++i)
            input[i][i] += static_cast<T>(N);
        compare<N, T, Qualifier>(input, "random diagonally dominant inverse");
    }

    // Bound the exponent so GLM's determinant/cofactor implementation remains
    // representable too. Relative comparisons catch errors in the tiny inverse.
    constexpr int exponent = (std::numeric_limits<T>::max_exponent - 1) / (2 * N);
    for (int sign : {-1, 1}) {
        const T scale = std::ldexp(T{1}, sign * exponent);
        const G input = pivot * scale;
        const auto result = m3::inverse(test::as_m3<Qualifier>(input));
        const G expected = glm::inverse(input);
        for (int c = 0; c < N; ++c)
            for (int r = 0; r < N; ++r)
                runner.relative(result(c, r), expected[c][r], "scaled inverse relative GLM");
        runner.matrix(test::as_m3<Qualifier>(input) * result, G{T{1}},
                      "scaled inverse identity residual");
    }

    // Exactly representable binary inputs expose a small nonzero pivot without
    // assigning an arbitrary epsilon threshold to invertibility.
    G near_singular{T{1}};
    near_singular[1][0] = T{1}; near_singular[0][1] = T{1};
    near_singular[1][1] = T{1} + T{4} * std::numeric_limits<T>::epsilon();
    compare<N, T, Qualifier>(near_singular, "near-singular binary fixture");

    G other{T{1}};
    other[1][0] = T{3}; other[0][0] = T{2};
    const auto a = test::as_m3<Qualifier>(other);
    const auto b = test::as_m3<Qualifier>(pivot);
    const auto inv_b = m3::inverse(b);
    const G ginverse = glm::inverse(pivot);
    runner.truth(other * ginverse != ginverse * other,
                 "left/right inverse fixtures do not commute");
    runner.matrix(a * inv_b, other * ginverse, "explicit right inverse multiplication");
    runner.matrix(inv_b * a, ginverse * other, "explicit left inverse multiplication");
}

template <typename T, Q Qualifier>
void affine() {
    test::GMat<4, T> translation{T{1}}, scale{T{1}}, shear{T{1}};
    translation[3] = test::GVec<4, T>{T{3}, T{-5}, T{7}, T{1}};
    scale[0][0] = T{-2}; scale[1][1] = T{4}; scale[2][2] = T{8};
    shear[1][0] = T{0.5}; shear[2][1] = T{-0.25};
    compare<4, T, Qualifier>(translation * shear * scale, "composed affine inverse");
}

template <typename T, Q Qualifier>
constexpr auto constant_inverse() {
    m3::Mat2<T, Qualifier> matrix;
    matrix(0, 0) = T{0}; matrix(1, 0) = T{2};
    matrix(0, 1) = T{4}; matrix(1, 1) = T{0};
    return m3::inverse(matrix);
}

template <typename T, Q Qualifier>
void type() {
    matrices<2, T, Qualifier>(); matrices<3, T, Qualifier>(); matrices<4, T, Qualifier>();
    affine<T, Qualifier>();
    constexpr auto constant_result = constant_inverse<T, Qualifier>();
    const test::GMat<2, T> input{T{0}, T{4}, T{2}, T{0}};
    runner.matrix(constant_result, glm::inverse(input), "constexpr pivot inverse");
}

template <Q Qualifier>
void qualifier() {
    type<float, Qualifier>(); type<double, Qualifier>(); type<long double, Qualifier>();
}

int invalid(std::string_view kind) {
    m3::Mat3<double> input{1.0};
    if (kind == "singular") {
        input[1] = input[0];
    } else if (kind == "zero") {
        input = m3::Mat3<double>{};
    } else if (kind == "nan") {
        input(2, 1) = std::numeric_limits<double>::quiet_NaN();
    } else if (kind == "infinity") {
        input(1, 2) = std::numeric_limits<double>::infinity();
    } else if (kind == "negative-infinity") {
        input(0, 2) = -std::numeric_limits<double>::infinity();
    } else if (kind == "overflow") {
        input(2, 2) = std::numeric_limits<double>::denorm_min();
    } else {
        return 2;
    }
    const auto result = m3::inverse(input);
    const volatile double observed = result(0, 0); (void)observed;
    return 0;
}
}

int main(int argc, char* argv[]) {
    if (argc == 3 && std::string_view{argv[1]} == "--invalid")
        return invalid(argv[2]);
    if (argc != 1) return 2;
    qualifier<Q::aligned_none>(); qualifier<Q::aligned_low>();
    qualifier<Q::aligned_medium>(); qualifier<Q::aligned_high>();
    return runner.finish("Inverse GLM reference tests");
}
