#include "support/glm_reference.hpp"

extern "C" int benchmark_pin_cpu(int preferred);

#if defined(__GNUC__) && !defined(__clang__)
#define BENCH_KERNEL __attribute__((noinline, noipa))
#define BENCH_RESTRICT __restrict__
#elif defined(__clang__)
#define BENCH_KERNEL __attribute__((noinline))
#define BENCH_RESTRICT __restrict__
#elif defined(_MSC_VER)
#define BENCH_KERNEL __declspec(noinline)
#define BENCH_RESTRICT __restrict
#else
#define BENCH_KERNEL
#define BENCH_RESTRICT
#endif

namespace {

enum class VectorOp {
    add, subtract, multiply, divide, scalar_divide, add_into, dot, length, distance,
    normalize, cross, reflect, refract, chain, mix, clamp, mod, smoothstep,
    sin, atan2, pow, exp, log, sqrt
};
enum class MatrixOp {
    add, multiply, component_multiply, component_divide, inverse,
    multiply_inverse_right, multiply_inverse_left, mat_vec, vec_mat, point, direction
};

template <bool M3, int L>
using Vector = std::conditional_t<M3, m3::Vec<L, float>, glm::vec<L, float, glm::defaultp>>;
template <bool M3, int N>
using Matrix = std::conditional_t<M3, m3::Mat<N, N, float>, glm::mat<N, N, float, glm::defaultp>>;
template <VectorOp Op>
constexpr bool scalar_output = Op == VectorOp::dot || Op == VectorOp::length || Op == VectorOp::distance;

template <bool M3, VectorOp Op, int L>
BENCH_KERNEL void vector_kernel(const Vector<M3, L>* BENCH_RESTRICT a,
                                const Vector<M3, L>* BENCH_RESTRICT b,
                                std::conditional_t<scalar_output<Op>, float, Vector<M3, L>>* BENCH_RESTRICT out,
                                std::size_t count) {
    for (std::size_t i = 0; i < count; ++i) {
        if constexpr (Op == VectorOp::add) out[i] = a[i] + b[i];
        else if constexpr (Op == VectorOp::subtract) out[i] = a[i] - b[i];
        else if constexpr (Op == VectorOp::multiply) out[i] = a[i] * b[i];
        else if constexpr (Op == VectorOp::divide) out[i] = a[i] / b[i];
        else if constexpr (Op == VectorOp::scalar_divide) out[i] = a[i] / b[i][0];
        else if constexpr (Op == VectorOp::chain) out[i] = (a[i] + b[i]) * 0.375f - a[i] * 0.125f;
        else if constexpr (Op == VectorOp::add_into) {
            if constexpr (M3) m3::add_into(out[i], a[i], b[i]);
            else out[i] = a[i] + b[i];
        } else if constexpr (M3) {
            if constexpr (Op == VectorOp::dot) out[i] = m3::dot(a[i], b[i]);
            else if constexpr (Op == VectorOp::length) out[i] = m3::length(a[i]);
            else if constexpr (Op == VectorOp::distance) out[i] = m3::distance(a[i], b[i]);
            else if constexpr (Op == VectorOp::normalize) out[i] = m3::normalize(a[i]);
            else if constexpr (Op == VectorOp::cross) out[i] = m3::cross(a[i], b[i]);
            else if constexpr (Op == VectorOp::reflect) out[i] = m3::reflect(a[i], b[i]);
            else if constexpr (Op == VectorOp::refract) out[i] = m3::refract(a[i], b[i], 0.5f);
            else if constexpr (Op == VectorOp::mix) out[i] = m3::mix(a[i], b[i], 0.375f);
            else if constexpr (Op == VectorOp::clamp) out[i] = m3::clamp(a[i], 0.5f, 1.5f);
            else if constexpr (Op == VectorOp::mod) out[i] = m3::mod(a[i], b[i]);
            else if constexpr (Op == VectorOp::smoothstep) out[i] = m3::smoothstep(0.0f, 3.0f, a[i]);
            else if constexpr (Op == VectorOp::sin) out[i] = m3::sin(a[i]);
            else if constexpr (Op == VectorOp::atan2) out[i] = m3::atan2(a[i], b[i]);
            else if constexpr (Op == VectorOp::pow) out[i] = m3::pow(a[i], b[i]);
            else if constexpr (Op == VectorOp::exp) out[i] = m3::exp(a[i]);
            else if constexpr (Op == VectorOp::log) out[i] = m3::log(a[i]);
            else if constexpr (Op == VectorOp::sqrt) out[i] = m3::sqrt(a[i]);
        } else {
            if constexpr (Op == VectorOp::dot) out[i] = glm::dot(a[i], b[i]);
            else if constexpr (Op == VectorOp::length) out[i] = glm::length(a[i]);
            else if constexpr (Op == VectorOp::distance) out[i] = glm::distance(a[i], b[i]);
            else if constexpr (Op == VectorOp::normalize) out[i] = glm::normalize(a[i]);
            else if constexpr (Op == VectorOp::cross) out[i] = glm::cross(a[i], b[i]);
            else if constexpr (Op == VectorOp::reflect) out[i] = glm::reflect(a[i], b[i]);
            else if constexpr (Op == VectorOp::refract) out[i] = glm::refract(a[i], b[i], 0.5f);
            else if constexpr (Op == VectorOp::mix) out[i] = glm::mix(a[i], b[i], 0.375f);
            else if constexpr (Op == VectorOp::clamp) out[i] = glm::clamp(a[i], 0.5f, 1.5f);
            else if constexpr (Op == VectorOp::mod) out[i] = glm::mod(a[i], b[i]);
            else if constexpr (Op == VectorOp::smoothstep) out[i] = glm::smoothstep(0.0f, 3.0f, a[i]);
            else if constexpr (Op == VectorOp::sin) out[i] = glm::sin(a[i]);
            else if constexpr (Op == VectorOp::atan2) out[i] = glm::atan(a[i], b[i]);
            else if constexpr (Op == VectorOp::pow) out[i] = glm::pow(a[i], b[i]);
            else if constexpr (Op == VectorOp::exp) out[i] = glm::exp(a[i]);
            else if constexpr (Op == VectorOp::log) out[i] = glm::log(a[i]);
            else if constexpr (Op == VectorOp::sqrt) out[i] = glm::sqrt(a[i]);
        }
    }
}

template <MatrixOp Op>
constexpr bool transform_op = Op == MatrixOp::point || Op == MatrixOp::direction;
template <bool M3, MatrixOp Op, int N>
using MatrixInputB = std::conditional_t<transform_op<Op>, Vector<M3, 3>,
    std::conditional_t<Op == MatrixOp::mat_vec || Op == MatrixOp::vec_mat, Vector<M3, N>, Matrix<M3, N>>>;
template <bool M3, MatrixOp Op, int N>
using MatrixOutput = std::conditional_t<transform_op<Op>, Vector<M3, 3>,
    std::conditional_t<Op == MatrixOp::mat_vec || Op == MatrixOp::vec_mat, Vector<M3, N>, Matrix<M3, N>>>;

template <bool M3, MatrixOp Op, int N>
BENCH_KERNEL void matrix_kernel(const Matrix<M3, N>* BENCH_RESTRICT a,
                                const MatrixInputB<M3, Op, N>* BENCH_RESTRICT b,
                                MatrixOutput<M3, Op, N>* BENCH_RESTRICT out, std::size_t count) {
    for (std::size_t i = 0; i < count; ++i) {
        if constexpr (Op == MatrixOp::add) out[i] = a[i] + b[i];
        else if constexpr (Op == MatrixOp::multiply) out[i] = a[i] * b[i];
        else if constexpr (Op == MatrixOp::component_multiply) {
            if constexpr (M3) out[i] = m3::component_multiply(a[i], b[i]);
            else out[i] = glm::matrixCompMult(a[i], b[i]);
        }
        else if constexpr (Op == MatrixOp::component_divide) {
            if constexpr (M3) out[i] = m3::component_divide(a[i], b[i]);
            else out[i] = test::component_divide(a[i], b[i]);
        } else if constexpr (Op == MatrixOp::inverse) {
            if constexpr (M3) out[i] = m3::inverse(a[i]);
            else out[i] = glm::inverse(a[i]);
        } else if constexpr (Op == MatrixOp::multiply_inverse_right) out[i] = a[i] * b[i];
        else if constexpr (Op == MatrixOp::multiply_inverse_left) out[i] = b[i] * a[i];
        else if constexpr (Op == MatrixOp::mat_vec) out[i] = a[i] * b[i];
        else if constexpr (Op == MatrixOp::vec_mat) out[i] = b[i] * a[i];
        else if constexpr (M3) {
            if constexpr (Op == MatrixOp::point) out[i] = m3::transform_point(a[i], b[i]);
            else out[i] = m3::transform_direction(a[i], b[i]);
        } else {
            const auto homogeneous = a[i] * glm::vec4{b[i], Op == MatrixOp::point ? 1.0f : 0.0f};
            if constexpr (Op == MatrixOp::point) out[i] = glm::vec3{homogeneous} / homogeneous.w;
            else out[i] = glm::vec3{homogeneous};
        }
    }
}

struct Config {
    std::size_t count = 4096;
    int samples = 7;
    double min_sample_ms = 12;
    int cpu = 2;
    std::ofstream raw;
};
volatile double result_sink = 0;

template <typename V>
constexpr int components() {
    if constexpr (std::is_arithmetic_v<V>) return 1;
    else if constexpr (requires { V::columns; }) return V::columns * V::rows;
    else if constexpr (requires { V::dimension; }) return V::dimension;
    else if constexpr (requires { typename V::col_type; }) return V::length() * V::col_type::length();
    else return V::length();
}

template <typename V>
decltype(auto) element(V& value, int index) {
    using Plain = std::remove_cv_t<V>;
    if constexpr (std::is_arithmetic_v<Plain>) return (value);
    else if constexpr (requires { Plain::columns; }) return (value(index / Plain::rows, index % Plain::rows));
    else if constexpr (requires { typename Plain::col_type; })
        return (value[index / Plain::col_type::length()][index % Plain::col_type::length()]);
    else return (value[index]);
}

float next_value(std::uint32_t& state) {
    state = state * 1664525u + 1013904223u;
    return 0.25f + static_cast<float>(state >> 8) / 8388608.0f;
}

template <typename V>
V input_value(std::uint32_t& state) {
    V value{};
    for (int i = 0; i < components<V>(); ++i)
        element(value, i) = next_value(state);
    return value;
}

void compiler_barrier() {
#if defined(__GNUC__) || defined(__clang__)
    asm volatile("" ::: "memory");
#else
    std::atomic_signal_fence(std::memory_order_seq_cst);
#endif
}

template <typename MA, typename MB, typename GA, typename GB, typename MO, typename GO>
void run_pair(Config& config, const std::string& name, const std::vector<MA>& ma,
              const std::vector<MB>& mb, const std::vector<GA>& ga, const std::vector<GB>& gb,
              void (*m3_kernel)(const MA*, const MB*, MO*, std::size_t),
              void (*glm_kernel)(const GA*, const GB*, GO*, std::size_t)) {
    std::vector<MO> mo(config.count);
    std::vector<GO> go(config.count);
    const auto m3_call = [&] { m3_kernel(ma.data(), mb.data(), mo.data(), config.count); compiler_barrier(); };
    const auto glm_call = [&] { glm_kernel(ga.data(), gb.data(), go.data(), config.count); compiler_barrier(); };
    m3_call(); glm_call();
    double max_error = 0;
    for (std::size_t i = 0; i < config.count; ++i)
        for (int lane = 0; lane < components<MO>(); ++lane) {
            const double actual = element(mo[i], lane);
            const double expected = element(go[i], lane);
            const double error = std::abs(actual - expected) / (1.0 + std::abs(expected));
            if (!std::isfinite(actual) || !std::isfinite(expected) || error > 64 * std::numeric_limits<float>::epsilon())
                throw std::runtime_error("GLM verification failed before timing: " + name);
            max_error = std::max(max_error, error);
        }
    const auto measure = [](const auto& call, int repetitions) {
        const auto start = std::chrono::steady_clock::now();
        for (int repeat = 0; repeat < repetitions; ++repeat) call();
        return std::chrono::duration<double, std::nano>(std::chrono::steady_clock::now() - start).count();
    };
    int repetitions = 1;
    while (repetitions < (1 << 24)) {
        const double m3_time = measure(m3_call, repetitions);
        const double glm_time = measure(glm_call, repetitions);
        if (std::min(m3_time, glm_time) >= config.min_sample_ms * 1e6) break;
        repetitions *= 2;
    }
    std::vector<double> m3_times, glm_times;
    for (int sample = 0; sample < config.samples; ++sample) {
        double m3_time, glm_time;
        if (sample % 2 == 0) {
            m3_time = measure(m3_call, repetitions);
            glm_time = measure(glm_call, repetitions);
        } else {
            glm_time = measure(glm_call, repetitions);
            m3_time = measure(m3_call, repetitions);
        }
        const double objects = static_cast<double>(config.count) * repetitions;
        m3_time /= objects; glm_time /= objects;
        m3_times.push_back(m3_time); glm_times.push_back(glm_time);
        if (config.raw)
            config.raw << name << ',' << sample << ',' << (sample % 2 == 0 ? "M3-first" : "GLM-first")
                       << ',' << std::setprecision(12) << m3_time << ',' << glm_time << '\n';
    }
    double m3_sum = 0, glm_sum = 0;
    for (std::size_t i = 0; i < config.count; ++i)
        for (int lane = 0; lane < components<MO>(); ++lane) {
            m3_sum += element(mo[i], lane); glm_sum += element(go[i], lane);
        }
    result_sink = m3_sum + glm_sum;
    std::sort(m3_times.begin(), m3_times.end());
    std::sort(glm_times.begin(), glm_times.end());
    const auto middle = config.samples / 2;
    std::cout << name << ',' << config.count << ',' << repetitions << ',' << config.samples
              << ',' << m3_times[middle] << ',' << glm_times[middle] << ',' << m3_times[middle] / glm_times[middle]
              << ',' << m3_times.front() << ',' << m3_times.back() << ',' << glm_times.front() << ',' << glm_times.back()
              << ',' << max_error << ',' << m3_sum << ',' << glm_sum << '\n';
}

template <VectorOp Op, int L>
void vector_case(Config& config, std::string_view operation) {
    using MV = Vector<true, L>;
    using GV = Vector<false, L>;
    std::vector<MV> ma(config.count), mb(config.count);
    std::vector<GV> ga(config.count), gb(config.count);
    std::uint32_t state = 0x4d33474cU + L;
    for (std::size_t i = 0; i < config.count; ++i) {
        ga[i] = input_value<GV>(state); gb[i] = input_value<GV>(state);
        if constexpr (Op == VectorOp::reflect || Op == VectorOp::refract) {
            ga[i] = glm::normalize(ga[i]); gb[i] = glm::normalize(gb[i]);
        }
        ma[i] = test::as_m3(ga[i]); mb[i] = test::as_m3(gb[i]);
    }
    run_pair(config, "vec" + std::to_string(L) + "/" + std::string{operation}, ma, mb, ga, gb,
             vector_kernel<true, Op, L>, vector_kernel<false, Op, L>);
}

template <int L>
void vector_core(Config& config) {
    vector_case<VectorOp::add, L>(config, "add");
    vector_case<VectorOp::subtract, L>(config, "subtract");
    vector_case<VectorOp::multiply, L>(config, "multiply");
    vector_case<VectorOp::divide, L>(config, "divide");
    vector_case<VectorOp::scalar_divide, L>(config, "scalar_divide");
    vector_case<VectorOp::dot, L>(config, "dot");
    vector_case<VectorOp::length, L>(config, "length");
    vector_case<VectorOp::distance, L>(config, "distance");
    vector_case<VectorOp::normalize, L>(config, "normalize");
}

template <MatrixOp Op, int N>
void matrix_case(Config& config, std::string_view operation) {
    std::vector<Matrix<true, N>> ma(config.count);
    std::vector<Matrix<false, N>> ga(config.count);
    std::vector<MatrixInputB<true, Op, N>> mb(config.count);
    std::vector<MatrixInputB<false, Op, N>> gb(config.count);
    std::uint32_t state = 0x4d415452U + N;
    for (std::size_t i = 0; i < config.count; ++i) {
        ga[i] = input_value<Matrix<false, N>>(state);
        gb[i] = input_value<MatrixInputB<false, Op, N>>(state);
        if constexpr (Op == MatrixOp::inverse) {
            // The diagonal is larger than the sum of all off-diagonal terms.
            // Input construction is outside the timed kernels, not an inverse oracle.
            ga[i] *= 0.125f;
            for (int diagonal = 0; diagonal < N; ++diagonal)
                ga[i][diagonal][diagonal] += static_cast<float>(N);
        } else if constexpr (Op == MatrixOp::multiply_inverse_right
                             || Op == MatrixOp::multiply_inverse_left) {
            gb[i] *= 0.125f;
            for (int diagonal = 0; diagonal < N; ++diagonal)
                gb[i][diagonal][diagonal] += static_cast<float>(N);
            // These cases time only reuse of an inverse. Generate it once with
            // GLM and share its exact float bits with M3; inverse() has its own case.
            gb[i] = glm::inverse(gb[i]);
        }
        ma[i] = test::as_m3(ga[i]); mb[i] = test::as_m3(gb[i]);
    }
    run_pair(config, "mat" + std::to_string(N) + "/" + std::string{operation}, ma, mb, ga, gb,
             matrix_kernel<true, Op, N>, matrix_kernel<false, Op, N>);
}

template <int N>
void matrix_core(Config& config) {
    matrix_case<MatrixOp::add, N>(config, "add");
    matrix_case<MatrixOp::multiply, N>(config, "multiply");
    matrix_case<MatrixOp::component_multiply, N>(config, "component_multiply");
    matrix_case<MatrixOp::component_divide, N>(config, "component_divide");
    matrix_case<MatrixOp::inverse, N>(config, "inverse");
    matrix_case<MatrixOp::multiply_inverse_right, N>(config, "multiply_precomputed_inverse_right");
    matrix_case<MatrixOp::multiply_inverse_left, N>(config, "multiply_precomputed_inverse_left");
    matrix_case<MatrixOp::mat_vec, N>(config, "matrix_vector");
    matrix_case<MatrixOp::vec_mat, N>(config, "vector_matrix");
}

}

int main(int argc, char* argv[]) {
    try {
#ifndef NDEBUG
        throw std::runtime_error("Run this performance target in Release mode");
#endif
        Config config;
        for (int i = 1; i < argc; i += 2) {
            if (i + 1 >= argc) throw std::runtime_error("Missing benchmark option value");
            const std::string_view option{argv[i]};
            if (option == "--count") config.count = std::stoull(argv[i + 1]);
            else if (option == "--samples") config.samples = std::stoi(argv[i + 1]);
            else if (option == "--min-ms") config.min_sample_ms = std::stod(argv[i + 1]);
            else if (option == "--cpu") config.cpu = std::stoi(argv[i + 1]);
            else if (option == "--raw-output") {
                config.raw.open(argv[i + 1]);
                if (!config.raw) throw std::runtime_error("Cannot open raw sample output");
                config.raw << "case,sample,order,m3_ns,glm_ns\n";
            } else throw std::runtime_error("Unknown benchmark option");
        }
        if (config.count < 64 || config.count > 1048576 || config.samples < 3 || config.samples > 15
            || config.samples % 2 == 0 || config.min_sample_ms < 1 || config.min_sample_ms > 200)
            throw std::runtime_error("Invalid count/samples/min-ms");
        const int selected_cpu = benchmark_pin_cpu(config.cpu);
        if (selected_cpu < 0) throw std::runtime_error("Could not pin the benchmark thread to an allowed CPU");
        std::cerr << "compiler=" << __VERSION__ << "\nGLM=" << GLM_VERSION_MAJOR << '.' << GLM_VERSION_MINOR
                  << '.' << GLM_VERSION_PATCH << "\npinned_cpu=" << selected_cpu
                  << "\nobjects=" << config.count << "\nsamples=" << config.samples
                  << "\nminimum_sample_ms=" << config.min_sample_ms
                  << "\nlayout=packed default; float; GLM defaultp; no forced GLM intrinsics"
                  << "\ninverse_inputs=strictly diagonally dominant; inverse() is timed separately"
                  << "\nprecomputed_inverse_inputs=GLM inverse computed before timing; identical bits shared with M3"
                  << "\nM3_vec2_size_align=" << sizeof(Vector<true, 2>) << '/' << alignof(Vector<true, 2>)
                  << "\nGLM_vec2_size_align=" << sizeof(Vector<false, 2>) << '/' << alignof(Vector<false, 2>)
                  << "\nM3_vec3_size_align=" << sizeof(Vector<true, 3>) << '/' << alignof(Vector<true, 3>)
                  << "\nGLM_vec3_size_align=" << sizeof(Vector<false, 3>) << '/' << alignof(Vector<false, 3>)
                  << "\nM3_vec4_size_align=" << sizeof(Vector<true, 4>) << '/' << alignof(Vector<true, 4>)
                  << "\nGLM_vec4_size_align=" << sizeof(Vector<false, 4>) << '/' << alignof(Vector<false, 4>)
                  << "\nGLM_CONFIG_SIMD=" << GLM_CONFIG_SIMD
                  << "\nGLM_CONFIG_ALIGNED_GENTYPES=" << GLM_CONFIG_ALIGNED_GENTYPES << '\n';
        std::cout << "case,objects,repetitions,samples,m3_median_ns,glm_median_ns,m3_over_glm,"
                     "m3_min_ns,m3_max_ns,glm_min_ns,glm_max_ns,max_relative_error,m3_checksum,glm_checksum\n"
                  << std::fixed << std::setprecision(9);
        vector_core<2>(config); vector_core<3>(config); vector_core<4>(config);
        vector_case<VectorOp::add_into, 3>(config, "add_into");
        vector_case<VectorOp::cross, 3>(config, "cross");
        vector_case<VectorOp::reflect, 3>(config, "reflect");
        vector_case<VectorOp::refract, 3>(config, "refract");
        vector_case<VectorOp::chain, 4>(config, "chain");
        vector_case<VectorOp::mix, 4>(config, "mix");
        vector_case<VectorOp::clamp, 4>(config, "clamp");
        vector_case<VectorOp::mod, 4>(config, "mod");
        vector_case<VectorOp::smoothstep, 4>(config, "smoothstep");
        vector_case<VectorOp::sin, 4>(config, "sin");
        vector_case<VectorOp::atan2, 4>(config, "atan2");
        vector_case<VectorOp::pow, 4>(config, "pow");
        vector_case<VectorOp::exp, 4>(config, "exp");
        vector_case<VectorOp::log, 4>(config, "log");
        vector_case<VectorOp::sqrt, 4>(config, "sqrt");
        matrix_core<2>(config); matrix_core<3>(config); matrix_core<4>(config);
        matrix_case<MatrixOp::point, 4>(config, "projective_point");
        matrix_case<MatrixOp::direction, 4>(config, "direction");
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
