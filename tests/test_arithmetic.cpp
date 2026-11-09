#include "support/glm_reference.hpp"

namespace {
test::Runner runner;
using test::Q;

template <int L, typename T, Q Qualifier>
void vectors() {
    using V = m3::Vec<L, T, Qualifier>;
    using G = test::GVec<L, T>;
    const T scalar{3};
    runner.vector(V{}, G{T{0}}, "M3 default vector zero policy");
    runner.vector(V{scalar}, G{scalar}, "scalar vector constructor");
    std::mt19937 generator{0x41524954U + L};
    std::uniform_int_distribution<int> distribution{-12, 12};
    for (int sample = 0; sample < 12; ++sample) {
        G ga{T{0}}, gb{T{0}};
        for (int i = 0; i < L; ++i) {
            ga[i] = static_cast<T>(distribution(generator));
            gb[i] = static_cast<T>((i % 3 + 1) * (sample % 2 == 0 ? 1 : -1));
        }
        const V a{glm::value_ptr(ga)}, b{glm::value_ptr(gb)};
        runner.vector(a, ga, "pointer vector constructor");
        runner.vector(V{a}, ga, "copy vector constructor");
        for (int i = 0; i < L; ++i)
            runner.check(a.value_ptr()[i], ga[i], "vector value_ptr");
        runner.check(a.x(), ga.x, "x accessor");
        runner.check(a.r(), ga.r, "r accessor");
        runner.check(a.s(), ga.s, "s accessor");
        if constexpr (L >= 2) {
            runner.check(a.y(), ga.y, "y accessor"); runner.check(a.g(), ga.g, "g accessor");
            runner.check(a.t(), ga.t, "t accessor");
        }
        if constexpr (L >= 3) {
            runner.check(a.z(), ga.z, "z accessor"); runner.check(a.b(), ga.b, "b accessor");
            runner.check(a.p(), ga.p, "p accessor");
        }
        if constexpr (L == 4) {
            runner.check(a.w(), ga.w, "w accessor"); runner.check(a.a(), ga.a, "a accessor");
            runner.check(a.q(), ga.q, "q accessor");
        }
        runner.vector(a + b, ga + gb, "vector + vector");
        runner.vector(a - b, ga - gb, "vector - vector");
        runner.vector(a * b, ga * gb, "vector * vector");
        runner.vector(a / b, ga / gb, "vector / vector");
        runner.vector(-a, -ga, "vector negation");
        runner.vector(a + scalar, ga + scalar, "vector + scalar");
        runner.vector(scalar + a, scalar + ga, "scalar + vector");
        runner.vector(a - scalar, ga - scalar, "vector - scalar");
        runner.vector(scalar - a, scalar - ga, "scalar - vector");
        runner.vector(a * scalar, ga * scalar, "vector * scalar");
        runner.vector(scalar * a, scalar * ga, "scalar * vector");
        runner.vector(a / scalar, ga / scalar, "vector / scalar");
        auto value = a;
        runner.truth(&(value += b) == &value, "vector += returns self");
        runner.vector(value, ga + gb, "vector += vector");
        value = a; value -= b; runner.vector(value, ga - gb, "vector -= vector");
        value = a; value *= b; runner.vector(value, ga * gb, "vector *= vector");
        value = a; value /= b; runner.vector(value, ga / gb, "vector /= vector");
        value = a; value += scalar; runner.vector(value, ga + scalar, "vector += scalar");
        value = a; value -= scalar; runner.vector(value, ga - scalar, "vector -= scalar");
        value = a; value *= scalar; runner.vector(value, ga * scalar, "vector *= scalar");
        value = a; value /= scalar; runner.vector(value, ga / scalar, "vector /= scalar");
        value = a; value += value; runner.vector(value, ga + ga, "self +=");
        value = a; value -= value; runner.vector(value, ga - ga, "self -=");
        value = a; value *= value; runner.vector(value, ga * ga, "self *=");
        value = b; value /= value; runner.vector(value, gb / gb, "self /=");
        runner.check(a == V{a}, ga == G{ga}, "vector equality");
        auto different = a; auto gdifferent = ga;
        different[L - 1] += T{1}; gdifferent[L - 1] += T{1};
        runner.check(a != different, ga != gdifferent, "vector inequality last component");
    }
}

template <int N, typename T, Q Qualifier>
void matrices() {
    using M = m3::Mat<N, N, T, Qualifier>;
    using G = test::GMat<N, T>;
    const T scalar{3};
    runner.matrix(M{}, G{T{0}}, "M3 default matrix zero policy");
    runner.matrix(M{scalar}, G{scalar}, "diagonal matrix constructor");
    std::mt19937 generator{0x4d415448U + N};
    std::uniform_int_distribution<int> distribution{-12, 12};
    for (int sample = 0; sample < 12; ++sample) {
        G ga{T{0}}, gb{T{0}};
        for (int c = 0; c < N; ++c)
            for (int r = 0; r < N; ++r) {
                ga[c][r] = static_cast<T>(distribution(generator));
                gb[c][r] = static_cast<T>(((c * N + r) % 3 + 1) * (sample % 2 == 0 ? 1 : -1));
            }
        const M a{glm::value_ptr(ga)}, b{glm::value_ptr(gb)};
        runner.matrix(a, ga, "column-major matrix constructor");
        runner.matrix(M{a}, ga, "copy matrix constructor");
        for (int c = 0; c < N; ++c)
            for (int r = 0; r < N; ++r)
                runner.check(a[c][r], ga[c][r], "matrix column accessor");
        // Padded matrix columns are not one tightly packed scalar array.
        for (int r = 0; r < N; ++r)
            runner.check(a.value_ptr()[r], ga[0][r], "matrix first column pointer");
        runner.matrix(a + b, ga + gb, "matrix + matrix");
        runner.matrix(a - b, ga - gb, "matrix - matrix");
        runner.matrix(m3::component_multiply(a, b), glm::matrixCompMult(ga, gb),
                      "component matrix multiplication");
        runner.matrix(m3::component_divide(a, b), test::component_divide(ga, gb),
                      "component matrix division");
        runner.matrix(-a, -ga, "matrix negation");
        runner.matrix(a + scalar, ga + scalar, "matrix + scalar");
        runner.matrix(scalar + a, scalar + ga, "scalar + matrix");
        runner.matrix(a - scalar, ga - scalar, "matrix - scalar");
        runner.matrix(a * scalar, ga * scalar, "matrix * scalar");
        runner.matrix(scalar * a, scalar * ga, "scalar * matrix");
        runner.matrix(a / scalar, ga / scalar, "matrix / scalar");
        auto value = a;
        runner.truth(&(value += b) == &value, "matrix += returns self");
        runner.matrix(value, ga + gb, "matrix += matrix");
        value = a; value -= b; runner.matrix(value, ga - gb, "matrix -= matrix");
        value = a; value += scalar; runner.matrix(value, ga + scalar, "matrix += scalar");
        value = a; value -= scalar; runner.matrix(value, ga - scalar, "matrix -= scalar");
        value = a; value *= scalar; runner.matrix(value, ga * scalar, "matrix *= scalar");
        value = a; value /= scalar; runner.matrix(value, ga / scalar, "matrix /= scalar");
        value = a; value += value; runner.matrix(value, ga + ga, "matrix self +=");
        value = a; value -= value; runner.matrix(value, ga - ga, "matrix self -=");
        value = b; value = m3::component_divide(value, value);
        runner.matrix(value, test::component_divide(gb, gb), "component self division assignment");
        runner.check(a == M{a}, ga == G{ga}, "matrix equality");
        auto different = a; auto gdifferent = ga;
        different(N - 1, N - 1) += T{1}; gdifferent[N - 1][N - 1] += T{1};
        runner.check(a != different, ga != gdifferent, "matrix inequality last element");
    }
}

template <typename T, Q Qualifier>
void type() {
    vectors<1, T, Qualifier>(); vectors<2, T, Qualifier>();
    vectors<3, T, Qualifier>(); vectors<4, T, Qualifier>();
    matrices<2, T, Qualifier>(); matrices<3, T, Qualifier>(); matrices<4, T, Qualifier>();
}
template <Q Qualifier> void qualifier() {
    type<int, Qualifier>(); type<float, Qualifier>(); type<double, Qualifier>();
}
}

int main(int argc, char* argv[]) {
    if (argc == 2) {
        const std::string_view mode{argv[1]};
        if (mode == "--matrix-divide-zero" || mode == "--matrix-scalar-divide-zero") {
            m3::Mat2<double> value{1.0};
            if (mode == "--matrix-divide-zero")
                value = m3::component_divide(value, m3::Mat2<double>{});
            else value /= 0.0;
            const volatile double observed = value(0, 0); (void)observed;
            return 0;
        }
        if (mode == "--matrix-negate-min") {
            m3::Mat2<int> value; value(0, 0) = std::numeric_limits<int>::lowest();
            value = -value;
            const volatile int observed = value(0, 0); (void)observed;
            return 0;
        }
        return 2;
    }
    qualifier<Q::aligned_none>(); qualifier<Q::aligned_low>();
    qualifier<Q::aligned_medium>(); qualifier<Q::aligned_high>();
    return runner.finish("Arithmetic reference tests");
}
