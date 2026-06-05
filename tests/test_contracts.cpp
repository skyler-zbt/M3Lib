// M3Lib — Contract violation tests (compiled with observe semantic).
//    xmake build test_contracts && xmake run test_contracts
//    mcpp build -p test_contracts && ./target/*/bin/test_contracts
//
// These tests intentionally trigger pre(i < L) violations on operator[].
// The observe semantic logs violations to stderr without terminating,
// so each test body should execute to completion.

import std;
import m3;

// === test infrastructure ===

import test_common;

// === test cases ===

int main()
{
    TestRunner runner;

    // Valid boundary indices — no violation
    runner.add("operator[] lower bound (index 0)", [] -> TestResult {
        m3::Vec<4, int> v{0};
        v[0] = 42;
        if (auto r = check(v[0] == 42); !r) return r;
        return {};
    });

    runner.add("operator[] upper bound (index L-1)", [] -> TestResult {
        m3::Vec<4, int> v{0};
        v[3] = 99;
        if (auto r = check(v[3] == 99); !r) return r;
        return {};
    });

    // Out-of-bounds — pre(i < L) violation, observe mode survives
    runner.add("OOB write index L survives observe", [] -> TestResult {
        m3::Vec<3, int> v{0};
        v[3] = 1;   // pre(3 < 3) = false → violation logged
        return {};  // reaching here proves observe mode works
    });

    runner.add("OOB write index L+2 survives observe", [] -> TestResult {
        m3::Vec<2, int> v{0};
        v[4] = 1;   // pre(4 < 2) = false → violation logged
        return {};
    });

    runner.add("OOB const read index L+7 survives observe", [] -> TestResult {
        const m3::Vec<2, int> v{0};
        [[maybe_unused]] int x = v[9];  // pre(9 < 2) = false → violation logged
        return {};
    });

    // Vector remains usable after contract violation
    runner.add("vec still usable after contract violation", [] -> TestResult {
        m3::Vec<3, float> v{};
        v[0] = 1.0f; v[1] = 2.0f; v[2] = 3.0f;
        v[4] = 0.0f;  // violation logged — but v[0..2] untouched
        if (auto r = check_float_eq(v[0], 1.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(v[1], 2.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(v[2], 3.0f, 1e-6f); !r) return r;
        auto sum = v + v;
        if (auto r = check_float_eq(sum[0], 2.0f, 1e-6f); !r) return r;
        return {};
    });

    return runner.run();
}
