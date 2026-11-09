#include "support/glm_reference.hpp"

namespace {
test::Runner runner;
using test::Q;

template <int L, typename T, Q Qualifier>
void vectors() {
    using V = m3::Vec<L, T, Qualifier>;
    using G = test::GVec<L, T>;
    G ga{T{0}}, gb{T{0}};
    for (int i = 0; i < L; ++i) {
        ga[i] = static_cast<T>(6 * (i + 2));
        gb[i] = static_cast<T>(2 * (i + 2));
    }
    const auto a = test::as_m3<Qualifier>(ga), b = test::as_m3<Qualifier>(gb);
    const auto into = [](int operation, V& out, const V& first, const V& second) -> V& {
        if (operation == 0) return m3::add_into(out, first, second);
        if (operation == 1) return m3::subtract_into(out, first, second);
        if (operation == 2) return m3::multiply_into(out, first, second);
        return m3::divide_into(out, first, second);
    };
    for (int operation = 0; operation < 4; ++operation) {
        const G expected = operation == 0 ? ga + gb : operation == 1 ? ga - gb
            : operation == 2 ? ga * gb : ga / gb;
        V out, first = a, second = b;
        runner.truth(&into(operation, out, a, b) == &out, "into returns output reference");
        runner.truth(&into(operation, first, first, b) == &first, "into aliases first input");
        runner.truth(&into(operation, second, a, second) == &second, "into aliases second input");
        runner.vector(out, expected, "into result");
        runner.vector(first, expected, "into first alias result");
        runner.vector(second, expected, "into second alias result");
    }
    runner.vector(a / b, ga / gb, "vector division");
    runner.vector(a / T{6}, ga / T{6}, "scalar division");
    auto value = a; value /= b; runner.vector(value, ga / gb, "division assignment");
    value = a; value /= T{6}; runner.vector(value, ga / T{6}, "scalar division assignment");
    value = a; value /= value; runner.vector(value, ga / ga, "self division alias");
    value = a; value /= value[0]; runner.vector(value, ga / ga[0], "scalar aliases component");
    value = a; value += value; runner.vector(value, ga + ga, "self addition alias");
    value = a; value -= value; runner.vector(value, ga - ga, "self subtraction alias");
    value = b; value *= value; runner.vector(value, gb * gb, "self multiplication alias");
    if constexpr (std::is_floating_point_v<T>) {
        const T small = std::numeric_limits<T>::min();
        const T infinity = std::numeric_limits<T>::infinity();
        const T nan = std::numeric_limits<T>::quiet_NaN();
        const G gs{small}; const V ms{small};
        runner.vector(ms / ms, gs / gs, "tiny direct division");
        runner.vector(ms / small, gs / small, "tiny direct scalar division");
        value = ms; value /= ms; runner.vector(value, gs / gs, "tiny compound division");
        value = ms; value /= small; runner.vector(value, gs / small, "tiny compound scalar division");
        runner.vector(a / V{infinity}, ga / G{infinity}, "infinite denominator", true);
        runner.vector(a / V{nan}, ga / G{nan}, "NaN denominator");
        runner.vector(V{-infinity} / T{-2}, G{-infinity} / T{-2}, "infinite numerator");
        G gz{T{0}};
        for (int i = 0; i < L; ++i) gz[i] = i % 2 == 0 ? -T{0} : T{0};
        const auto zeros = test::as_m3<Qualifier>(gz);
        runner.vector(zeros / T{-2}, gz / T{-2}, "signed zero scalar division", true);
        runner.vector(zeros / V{T{-2}}, gz / G{T{-2}}, "signed zero vector division", true);
        value = zeros; value /= V{T{-2}};
        runner.vector(value, gz / G{T{-2}}, "signed zero compound division", true);
    }
}

template <int N, typename T, Q Qualifier>
void matrices() {
    test::GMat<N, T> ga{T{0}}, gb{T{0}};
    for (int c = 0; c < N; ++c)
        for (int r = 0; r < N; ++r) {
            ga[c][r] = static_cast<T>(6 * (1 + c * N + r));
            gb[c][r] = static_cast<T>(2 * (1 + c * N + r));
        }
    const auto a = test::as_m3<Qualifier>(ga), b = test::as_m3<Qualifier>(gb);
    runner.matrix(m3::component_multiply(a, b), glm::matrixCompMult(ga, gb),
                  "component matrix multiplication");
    runner.matrix(m3::component_divide(a, b), test::component_divide(ga, gb),
                  "component matrix division");
    runner.matrix(a / T{6}, ga / T{6}, "matrix scalar division");
    auto value = a; value = m3::component_divide(value, b);
    runner.matrix(value, test::component_divide(ga, gb), "component matrix division assignment");
    value = a; value = m3::component_divide(value, value);
    runner.matrix(value, test::component_divide(ga, ga), "matrix self division alias");
    value = a; value /= value(0, 0);
    runner.matrix(value, ga / ga[0][0], "matrix scalar aliases element");
}

template <Q Qualifier>
constexpr auto constant_operations() {
    m3::Vec<3, double, Qualifier> a{6.0, 12.0, 18.0};
    const m3::Vec<3, double, Qualifier> b{2.0, 3.0, 6.0};
    auto result = a / b;
    result /= 2.0; result *= 2.0; a /= a;
    m3::add_into(result, result, a); m3::subtract_into(result, result, a);
    m3::multiply_into(result, result, a); m3::divide_into(result, result, a);
    return result;
}

template <typename T, Q Qualifier> void type() {
    vectors<1, T, Qualifier>(); vectors<2, T, Qualifier>();
    vectors<3, T, Qualifier>(); vectors<4, T, Qualifier>();
    matrices<2, T, Qualifier>(); matrices<3, T, Qualifier>(); matrices<4, T, Qualifier>();
}
template <Q Qualifier> void qualifier() {
    type<int, Qualifier>(); type<float, Qualifier>(); type<double, Qualifier>();
    constexpr auto constant_result = constant_operations<Qualifier>();
    const glm::dvec3 ga{6.0, 12.0, 18.0}, gb{2.0, 3.0, 6.0};
    auto expected = ga / gb;
    expected /= 2.0; expected *= 2.0;
    const auto self = ga / ga;
    expected += self; expected -= self; expected *= self; expected /= self;
    runner.vector(constant_result, expected, "constexpr output and compound operations");
}

template <int L> int zero_vector(std::string_view kind, int lane, double zero) {
    using V = m3::Vec<L, double, Q::aligned_medium>;
    if (lane < 0 || lane >= L) return 2;
    V a{4.0}, b{2.0}, result; b[lane] = zero;
    if (kind == "vv") result = a / b;
    else if (kind == "into") { m3::divide_into(a, a, b); result = a; }
    else if (kind == "vv-assign") { a /= b; result = a; }
    else if (kind == "vs") result = a / zero;
    else if (kind == "vs-assign") { a /= zero; result = a; }
    else return 2;
    const volatile double observed = result[0]; (void)observed;
    return 0;
}
template <int N> int zero_matrix(std::string_view kind, int lane, double zero) {
    using M = m3::Mat<N, N, double, Q::aligned_medium>;
    if (lane < 0 || lane >= N * N) return 2;
    M a, b, result;
    for (int c = 0; c < N; ++c)
        for (int r = 0; r < N; ++r) { a(c, r) = 4.0; b(c, r) = 2.0; }
    b(lane / N, lane % N) = zero;
    if (kind == "mm") result = m3::component_divide(a, b);
    else if (kind == "mm-assign") { a = m3::component_divide(a, b); result = a; }
    else if (kind == "ms") result = a / zero;
    else if (kind == "ms-assign") { a /= zero; result = a; }
    else return 2;
    const volatile double observed = result(0, 0); (void)observed;
    return 0;
}

int policy(std::string_view kind) {
    if (kind == "vector-bounds") {
        m3::Vec<3, double> value;
        volatile std::size_t index = 3;
        const volatile double observed = value[index]; (void)observed;
    } else if (kind == "matrix-column-bounds") {
        m3::Mat3<double> value;
        volatile std::size_t index = 3;
        const volatile double observed = value[index][0]; (void)observed;
    } else if (kind == "matrix-row-bounds") {
        m3::Mat3<double> value;
        volatile std::size_t index = 3;
        const volatile double observed = value(0, index); (void)observed;
    } else if (kind == "vector-negate-min") {
        m3::Vec<1, int> value{std::numeric_limits<int>::lowest()};
        value = -value;
        const volatile int observed = value[0]; (void)observed;
    } else return 2;
    return 0;
}

static_assert(!std::is_constructible_v<m3::Vec<2, int>, m3::Vec<3, int>>);
static_assert(sizeof(m3::Vec<3, float>) == sizeof(glm::vec<3, float, glm::packed_highp>));
}

int main(int argc, char* argv[]) {
    if (argc == 3 && std::string_view{argv[1]} == "--policy") return policy(argv[2]);
    if (argc != 1) {
        if (argc != 6 || std::string_view{argv[1]} != "--zero") return 2;
        const std::string_view kind{argv[2]};
        const int dimension = std::atoi(argv[3]), lane = std::atoi(argv[4]);
        const double zero = std::atoi(argv[5]) != 0 ? -0.0 : 0.0;
        if (kind == "into" || kind.starts_with('v')) {
            switch (dimension) {
            case 1: return zero_vector<1>(kind, lane, zero);
            case 2: return zero_vector<2>(kind, lane, zero);
            case 3: return zero_vector<3>(kind, lane, zero);
            case 4: return zero_vector<4>(kind, lane, zero);
            }
        } else {
            switch (dimension) {
            case 2: return zero_matrix<2>(kind, lane, zero);
            case 3: return zero_matrix<3>(kind, lane, zero);
            case 4: return zero_matrix<4>(kind, lane, zero);
            }
        }
        return 2;
    }
    qualifier<Q::aligned_none>(); qualifier<Q::aligned_low>();
    qualifier<Q::aligned_medium>(); qualifier<Q::aligned_high>();
    return runner.finish("Division/output GLM reference tests");
}
