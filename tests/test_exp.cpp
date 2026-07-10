// M3Lib — Exponential function tests: pow, exp, log, exp2, log2, sqrt,
// inversesqrt.
//    xmake build test_exp && xmake run test_exp

import std;
import m3;
import test_common;

int main() {
    TestRunner runner;

    // ---- pow ----

    runner.add("pow scalar", [] -> TestResult {
        if (auto r = check_float_eq(m3::pow(2.0f, 3.0f), 8.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(m3::pow(4.0f, 0.5f), 2.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("pow vec2", [] -> TestResult {
        m3::vec2 x{2.0f, 3.0f};
        m3::vec2 y{2.0f, 2.0f};
        auto result = m3::pow(x, y);
        if (auto r = check_float_eq(result[0], 4.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(result[1], 9.0f, 1e-6f); !r)
            return r;
        return {};
    });

    // ---- exp / log ----

    runner.add("exp log scalar", [] -> TestResult {
        if (auto r = check_float_eq(m3::exp(0.0f), 1.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(m3::exp(1.0f), std::numbers::e_v<float>, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(m3::log(1.0f), 0.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(m3::log(std::numbers::e_v<float>), 1.0f, 1e-6f); !r)
            return r;
        // exp and log are inverses
        if (auto r = check_float_eq(m3::log(m3::exp(2.5f)), 2.5f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("exp vec3", [] -> TestResult {
        m3::vec3 v{0.0f, 1.0f, 2.0f};
        auto result = m3::exp(v);
        if (auto r = check_float_eq(result[0], 1.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(result[1], std::numbers::e_v<float>, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(result[2], std::numbers::e_v<float> * std::numbers::e_v<float>,
                                    1e-5f);
            !r)
            return r;
        return {};
    });

    // ---- exp2 / log2 ----

    runner.add("exp2 log2 scalar", [] -> TestResult {
        if (auto r = check_float_eq(m3::exp2(0.0f), 1.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(m3::exp2(3.0f), 8.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(m3::log2(8.0f), 3.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(m3::log2(1.0f), 0.0f, 1e-6f); !r)
            return r;
        // exp2 and log2 are inverses
        if (auto r = check_float_eq(m3::log2(m3::exp2(5.0f)), 5.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("exp2 vec2", [] -> TestResult {
        m3::vec2 v{0.0f, 4.0f};
        auto result = m3::exp2(v);
        if (auto r = check_float_eq(result[0], 1.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(result[1], 16.0f, 1e-6f); !r)
            return r;
        return {};
    });

    // ---- sqrt / inversesqrt ----

    runner.add("sqrt scalar", [] -> TestResult {
        if (auto r = check_float_eq(m3::sqrt(4.0f), 2.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(m3::sqrt(0.0f), 0.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(m3::sqrt(9.0f), 3.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("inversesqrt scalar", [] -> TestResult {
        if (auto r = check_float_eq(m3::inversesqrt(4.0f), 0.5f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(m3::inversesqrt(1.0f), 1.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(m3::inversesqrt(16.0f), 0.25f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("sqrt vec3", [] -> TestResult {
        m3::vec3 v{4.0f, 9.0f, 16.0f};
        auto result = m3::sqrt(v);
        if (auto r = check_float_eq(result[0], 2.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(result[1], 3.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(result[2], 4.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("inversesqrt vec3", [] -> TestResult {
        m3::vec3 v{4.0f, 1.0f, 16.0f};
        auto result = m3::inversesqrt(v);
        if (auto r = check_float_eq(result[0], 0.5f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(result[1], 1.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(result[2], 0.25f, 1e-6f); !r)
            return r;
        return {};
    });

    return runner.run();
}
