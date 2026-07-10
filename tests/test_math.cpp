// M3Lib — Math function tests: dot, cross, normalize, length, distance,
// reflect, refract, mix, clamp, lerp.
//    xmake build test_math && xmake run test_math
//    mcpp build -p test_math && ./target/*/bin/test_math

import std;
import m3;

// === test infrastructure ===

import test_common;

// === test cases ===

int main() {
    TestRunner runner;

    // ---- dot ----

    runner.add("dot vec3", [] -> TestResult {
        m3::Vec<3, float> a{1.0f, 2.0f, 3.0f};
        m3::Vec<3, float> b{4.0f, 5.0f, 6.0f};
        if (auto r = check_float_eq(m3::dot(a, b), 32.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("dot vec2", [] -> TestResult {
        m3::Vec<2, float> a{3.0f, 4.0f};
        m3::Vec<2, float> b{1.0f, 2.0f};
        if (auto r = check_float_eq(m3::dot(a, b), 11.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("dot orthogonal vectors is zero", [] -> TestResult {
        m3::Vec<2, float> a{1.0f, 0.0f};
        m3::Vec<2, float> b{0.0f, 1.0f};
        if (auto r = check_float_eq(m3::dot(a, b), 0.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("dot zero vector yields zero", [] -> TestResult {
        m3::Vec<3, float> a{0.0f};
        m3::Vec<3, float> v{1.0f, 2.0f, 3.0f};
        if (auto r = check_float_eq(m3::dot(a, v), 0.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("dot vec1 scalar", [] -> TestResult {
        m3::Vec<1, int> a{5};
        m3::Vec<1, int> b{3};
        if (auto r = check(m3::dot(a, b) == 15); !r)
            return r;
        return {};
    });

    runner.add("dot vec4", [] -> TestResult {
        m3::Vec<4, int> a{1, 2, 3, 4};
        m3::Vec<4, int> b{2, 3, 4, 5};
        if (auto r = check(m3::dot(a, b) == 40); !r)
            return r;  // 2+6+12+20
        return {};
    });

    // ---- cross ----

    runner.add("cross unit x × unit y = unit z", [] -> TestResult {
        m3::Vec<3, float> a{1.0f, 0.0f, 0.0f};
        m3::Vec<3, float> b{0.0f, 1.0f, 0.0f};
        auto c = m3::cross(a, b);
        if (auto r = check_float_eq(c[0], 0.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(c[1], 0.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(c[2], 1.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("cross parallel vectors yields zero", [] -> TestResult {
        m3::Vec<3, float> a{2.0f, 4.0f, 6.0f};
        m3::Vec<3, float> b{1.0f, 2.0f, 3.0f};
        auto c = m3::cross(a, b);
        if (auto r = check_float_eq(m3::length(c), 0.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("cross anti-commutative: a×b = -(b×a)", [] -> TestResult {
        m3::Vec<3, float> a{1.0f, 2.0f, 3.0f};
        m3::Vec<3, float> b{3.0f, 1.0f, 2.0f};
        auto ab = m3::cross(a, b);
        auto ba = m3::cross(b, a);
        if (auto r = check(ab == -ba); !r)
            return r;
        return {};
    });

    // ---- length ----

    runner.add("length of 3-4-5 triangle", [] -> TestResult {
        m3::Vec<2, float> v{3.0f, 4.0f};
        if (auto r = check_float_eq(m3::length(v), 5.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("length zero vector is zero", [] -> TestResult {
        m3::Vec<3, float> v{0.0f};
        if (auto r = check_float_eq(m3::length(v), 0.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("length unit vector is one", [] -> TestResult {
        m3::Vec<3, float> v{1.0f, 0.0f, 0.0f};
        if (auto r = check_float_eq(m3::length(v), 1.0f, 1e-6f); !r)
            return r;
        return {};
    });

    // ---- distance ----

    runner.add("distance origin to (1,2,2) is 3", [] -> TestResult {
        m3::Vec<3, float> a{0.0f, 0.0f, 0.0f};
        m3::Vec<3, float> b{1.0f, 2.0f, 2.0f};
        if (auto r = check_float_eq(m3::distance(a, b), 3.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("distance same point is zero", [] -> TestResult {
        m3::Vec<3, float> a{5.0f, -3.0f, 7.0f};
        if (auto r = check_float_eq(m3::distance(a, a), 0.0f, 1e-6f); !r)
            return r;
        return {};
    });

    // ---- normalize ----

    runner.add("normalize unit vector unchanged", [] -> TestResult {
        m3::Vec<3, float> v{1.0f, 0.0f, 0.0f};
        auto n = m3::normalize(v);
        if (auto r = check_float_eq(n[0], 1.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(n[1], 0.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("normalize produces unit length", [] -> TestResult {
        m3::Vec<3, float> v{3.0f, 4.0f, 0.0f};
        auto n = m3::normalize(v);
        if (auto r = check_float_eq(m3::length(n), 1.0f, 1e-5f); !r)
            return r;
        return {};
    });

    runner.add("normalize zero vector returns zero", [] -> TestResult {
        m3::Vec<3, float> v{0.0f};
        auto n = m3::normalize(v);
        if (auto r = check_float_eq(m3::length(n), 0.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("normalize preserves direction", [] -> TestResult {
        m3::Vec<3, float> v{2.0f, 6.0f, 9.0f};
        auto n = m3::normalize(v);
        // Cross product of parallel vectors is zero
        auto c = m3::cross(v, n);
        if (auto r = check_float_eq(m3::length(c), 0.0f, 1e-5f); !r)
            return r;
        return {};
    });

    // ---- reflect ----

    runner.add("reflect 45 degree mirror", [] -> TestResult {
        m3::Vec<2, float> I{1.0f, -1.0f};  // incoming from top-right
        m3::Vec<2, float> N{0.0f, 1.0f};   // surface normal up
        auto R = m3::reflect(I, N);
        // R should be (1, 1) — reflected upward
        if (auto r = check_float_eq(R[0], 1.0f, 1e-5f); !r)
            return r;
        if (auto r = check_float_eq(R[1], 1.0f, 1e-5f); !r)
            return r;
        return {};
    });

    runner.add("reflect straight-on returns opposite", [] -> TestResult {
        m3::Vec<3, float> I{0.0f, -1.0f, 0.0f};
        m3::Vec<3, float> N{0.0f, 1.0f, 0.0f};
        auto R = m3::reflect(I, N);
        if (auto r = check_float_eq(R[1], 1.0f, 1e-6f); !r)
            return r;
        return {};
    });

    // ---- refract ----

    runner.add("refract glass to air bends away from normal", [] -> TestResult {
        // Going from glass (1.5) to air (1.0): eta = n1/n2 = 1.5
        m3::Vec<2, float> I{0.0f, -1.0f};  // straight down
        m3::Vec<2, float> N{0.0f, 1.0f};   // surface normal up
        auto R = m3::refract(m3::normalize(I), N, 1.0f / 1.5f);
        // Straight down through glass-to-air: no bending, just passes through
        if (auto r = check_float_eq(R[0], 0.0f, 1e-5f); !r)
            return r;
        if (auto r = check(R[1] < 0.0f); !r)
            return r;  // still downward
        return {};
    });

    runner.add("refract total internal reflection returns zero", [] -> TestResult {
        // Grazing angle from glass (1.5) to air (1.0): eta = 1.5/1.0 = 1.5
        m3::Vec<2, float> I{-1.0f, -0.05f};  // nearly parallel to surface
        m3::Vec<2, float> N{0.0f, 1.0f};
        auto R = m3::refract(m3::normalize(I), N, 1.5f);
        if (auto r = check_float_eq(m3::length(R), 0.0f, 1e-6f); !r)
            return r;
        return {};
    });

    // ---- mix ----

    runner.add("mix scalar at a=0.3", [] -> TestResult {
        if (auto r = check_float_eq(m3::mix(0.0f, 10.0f, 0.3f), 3.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("mix vec a=0 returns x", [] -> TestResult {
        m3::Vec<3, float> x{1.0f, 2.0f, 3.0f};
        m3::Vec<3, float> y{7.0f, 8.0f, 9.0f};
        if (auto r = check(m3::mix(x, y, 0.0f) == x); !r)
            return r;
        return {};
    });

    runner.add("mix vec a=1 returns y", [] -> TestResult {
        m3::Vec<3, float> x{1.0f, 2.0f, 3.0f};
        m3::Vec<3, float> y{7.0f, 8.0f, 9.0f};
        if (auto r = check(m3::mix(x, y, 1.0f) == y); !r)
            return r;
        return {};
    });

    runner.add("mix vec a=0.5 midpoint", [] -> TestResult {
        m3::Vec<2, float> x{0.0f, 10.0f};
        m3::Vec<2, float> y{10.0f, 0.0f};
        auto m = m3::mix(x, y, 0.5f);
        if (auto r = check_float_eq(m[0], 5.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(m[1], 5.0f, 1e-6f); !r)
            return r;
        return {};
    });

    // ---- clamp ----

    runner.add("clamp scalar in range", [] -> TestResult {
        if (auto r = check_float_eq(m3::clamp(5.0f, 0.0f, 10.0f), 5.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("clamp scalar below min", [] -> TestResult {
        if (auto r = check_float_eq(m3::clamp(-5.0f, 0.0f, 10.0f), 0.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("clamp scalar above max", [] -> TestResult {
        if (auto r = check_float_eq(m3::clamp(15.0f, 0.0f, 10.0f), 10.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("clamp scalar at exact min boundary", [] -> TestResult {
        if (auto r = check_float_eq(m3::clamp(0.0f, 0.0f, 10.0f), 0.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("clamp scalar at exact max boundary", [] -> TestResult {
        if (auto r = check_float_eq(m3::clamp(10.0f, 0.0f, 10.0f), 10.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("clamp vec component-wise", [] -> TestResult {
        m3::Vec<3, float> v{-1.0f, 5.0f, 15.0f};
        auto c = m3::clamp(v, 0.0f, 10.0f);
        if (auto r = check_float_eq(c[0], 0.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(c[1], 5.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(c[2], 10.0f, 1e-6f); !r)
            return r;
        return {};
    });

    // ---- lerp ----

    runner.add("lerp equals mix", [] -> TestResult {
        m3::Vec<2, float> x{0.0f, 10.0f};
        m3::Vec<2, float> y{10.0f, 0.0f};
        if (auto r = check(m3::lerp(x, y, 0.5f) == m3::mix(x, y, 0.5f)); !r)
            return r;
        return {};
    });

    runner.add("lerp scalar equals mix scalar", [] -> TestResult {
        if (auto r =
                check_float_eq(m3::lerp(10.0f, 20.0f, 0.25f), m3::mix(10.0f, 20.0f, 0.25f), 1e-6f);
            !r)
            return r;
        return {};
    });

    // ---- abs ----

    runner.add("abs scalar int", [] -> TestResult {
        if (auto r = check(m3::abs(-5) == 5); !r)
            return r;
        if (auto r = check(m3::abs(5) == 5); !r)
            return r;
        if (auto r = check(m3::abs(0) == 0); !r)
            return r;
        return {};
    });

    runner.add("abs scalar float", [] -> TestResult {
        if (auto r = check_float_eq(m3::abs(-3.14f), 3.14f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("abs vec3", [] -> TestResult {
        m3::vec3 v{-1.0f, 2.0f, -3.0f};
        auto result = m3::abs(v);
        if (auto r = check_float_eq(result[0], 1.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(result[1], 2.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(result[2], 3.0f, 1e-6f); !r)
            return r;
        return {};
    });

    // ---- sign ----

    runner.add("sign scalar", [] -> TestResult {
        if (auto r = check(m3::sign(-5.0f) == -1.0f); !r)
            return r;
        if (auto r = check(m3::sign(0.0f) == 0.0f); !r)
            return r;
        if (auto r = check(m3::sign(5.0f) == 1.0f); !r)
            return r;
        return {};
    });

    // ---- floor / ceil / fract ----

    runner.add("floor scalar", [] -> TestResult {
        if (auto r = check_float_eq(m3::floor(1.7f), 1.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(m3::floor(-1.2f), -2.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("ceil scalar", [] -> TestResult {
        if (auto r = check_float_eq(m3::ceil(1.2f), 2.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(m3::ceil(-1.7f), -1.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("fract scalar", [] -> TestResult {
        if (auto r = check_float_eq(m3::fract(1.25f), 0.25f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(m3::fract(-0.5f), 0.5f, 1e-6f); !r)
            return r;
        return {};
    });

    // ---- mod (GLSL semantics: x - y * floor(x/y)) ----

    runner.add("mod scalar GLSL semantics", [] -> TestResult {
        // GLSL mod(-1.5, 2.0) = -1.5 - 2.0 * floor(-0.75) = -1.5 + 2.0 = 0.5
        // std::fmod(-1.5, 2.0) = -1.5 (different!)
        if (auto r = check_float_eq(m3::mod(-1.5f, 2.0f), 0.5f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(m3::mod(5.5f, 2.0f), 1.5f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("mod vec3", [] -> TestResult {
        m3::vec3 x{5.5f, -1.5f, 3.0f};
        m3::vec3 y{2.0f, 2.0f, 2.0f};
        auto result = m3::mod(x, y);
        if (auto r = check_float_eq(result[0], 1.5f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(result[1], 0.5f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(result[2], 1.0f, 1e-6f); !r)
            return r;
        return {};
    });

    // ---- min / max ----

    runner.add("min vec3", [] -> TestResult {
        m3::vec3 a{1.0f, 5.0f, 3.0f};
        m3::vec3 b{4.0f, 2.0f, 6.0f};
        auto result = m3::min(a, b);
        if (auto r = check_float_eq(result[0], 1.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(result[1], 2.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(result[2], 3.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("max vec3 scalar broadcast", [] -> TestResult {
        m3::vec3 a{1.0f, 5.0f, 3.0f};
        auto result = m3::max(a, 4.0f);
        if (auto r = check_float_eq(result[0], 4.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(result[1], 5.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(result[2], 4.0f, 1e-6f); !r)
            return r;
        return {};
    });

    // ---- step / smoothstep ----

    runner.add("step scalar", [] -> TestResult {
        if (auto r = check_float_eq(m3::step(0.5f, 0.3f), 0.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(m3::step(0.5f, 0.7f), 1.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(m3::step(0.5f, 0.5f), 1.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("smoothstep scalar", [] -> TestResult {
        // smoothstep(0, 1, 0.5) = 0.5 (Hermite at t=0.5)
        if (auto r = check_float_eq(m3::smoothstep(0.0f, 1.0f, 0.5f), 0.5f, 1e-6f); !r)
            return r;
        // smoothstep(0, 1, 0) = 0
        if (auto r = check_float_eq(m3::smoothstep(0.0f, 1.0f, 0.0f), 0.0f, 1e-6f); !r)
            return r;
        // smoothstep(0, 1, 1) = 1
        if (auto r = check_float_eq(m3::smoothstep(0.0f, 1.0f, 1.0f), 1.0f, 1e-6f); !r)
            return r;
        return {};
    });

    return runner.run();
}
