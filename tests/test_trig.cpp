// M3Lib — Trigonometric function tests: sin, cos, tan, asin, acos, atan,
// atan2, radians, degrees.
//    xmake build test_trig && xmake run test_trig

import std;
import m3;
import test_common;

int main() {
    TestRunner runner;

    // ---- sin / cos scalar ----

    runner.add("sin scalar", [] -> TestResult {
        if (auto r = check_float_eq(m3::sin(0.0f), 0.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(m3::sin(m3::radians(90.0f)), 1.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(m3::sin(m3::radians(180.0f)), 0.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("cos scalar", [] -> TestResult {
        if (auto r = check_float_eq(m3::cos(0.0f), 1.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(m3::cos(m3::radians(90.0f)), 0.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(m3::cos(m3::radians(180.0f)), -1.0f, 1e-6f); !r)
            return r;
        return {};
    });

    // ---- sin / cos vector ----

    runner.add("sin vec3", [] -> TestResult {
        m3::vec3 v{0.0f, m3::radians(90.0f), m3::radians(180.0f)};
        auto result = m3::sin(v);
        if (auto r = check_float_eq(result[0], 0.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(result[1], 1.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(result[2], 0.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("cos vec2", [] -> TestResult {
        m3::vec2 v{0.0f, m3::radians(180.0f)};
        auto result = m3::cos(v);
        if (auto r = check_float_eq(result[0], 1.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(result[1], -1.0f, 1e-6f); !r)
            return r;
        return {};
    });

    // ---- tan / asin / acos / atan ----

    runner.add("tan scalar", [] -> TestResult {
        if (auto r = check_float_eq(m3::tan(0.0f), 0.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(m3::tan(m3::radians(45.0f)), 1.0f, 1e-5f); !r)
            return r;
        return {};
    });

    runner.add("asin acos scalar", [] -> TestResult {
        if (auto r = check_float_eq(m3::asin(0.0f), 0.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(m3::asin(1.0f), m3::radians(90.0f), 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(m3::acos(1.0f), 0.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(m3::acos(0.0f), m3::radians(90.0f), 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("atan scalar", [] -> TestResult {
        if (auto r = check_float_eq(m3::atan(0.0f), 0.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(m3::atan(1.0f), m3::radians(45.0f), 1e-6f); !r)
            return r;
        return {};
    });

    // ---- atan2 ----

    runner.add("atan2 scalar", [] -> TestResult {
        if (auto r = check_float_eq(m3::atan2(1.0f, 1.0f), m3::radians(45.0f), 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(m3::atan2(0.0f, 1.0f), 0.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(m3::atan2(1.0f, 0.0f), m3::radians(90.0f), 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("atan2 vec2", [] -> TestResult {
        m3::vec2 y{1.0f, 0.0f};
        m3::vec2 x{1.0f, 1.0f};
        auto result = m3::atan2(y, x);
        if (auto r = check_float_eq(result[0], m3::radians(45.0f), 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(result[1], 0.0f, 1e-6f); !r)
            return r;
        return {};
    });

    // ---- radians / degrees round-trip ----

    runner.add("radians degrees round-trip", [] -> TestResult {
        if (auto r = check_float_eq(m3::degrees(m3::radians(45.0f)), 45.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(m3::radians(m3::degrees(1.0f)), 1.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("radians vec3", [] -> TestResult {
        m3::vec3 deg{0.0f, 90.0f, 180.0f};
        auto rad = m3::radians(deg);
        if (auto r = check_float_eq(rad[0], 0.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(rad[1], m3::radians(90.0f), 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(rad[2], m3::radians(180.0f), 1e-6f); !r)
            return r;
        return {};
    });

    return runner.run();
}
