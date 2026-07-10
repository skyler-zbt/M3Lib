// M3Lib — Homogeneous transform tests: transform_point, transform_direction.
//    xmake build test_transform && xmake run test_transform

import std;
import m3;

// === test infrastructure ===

import test_common;

// === test cases ===

int main() {
    TestRunner runner;

    // ---- transform_point ----

    runner.add("transform_point applies translation", [] -> TestResult {
        // Translation matrix: translate by (5, 0, 0)
        float data[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 5, 0, 0, 1};
        m3::Mat4 m{data};
        m3::vec3 p{1.0f, 2.0f, 3.0f};
        auto result = m3::transform_point(m, p);
        if (auto r = check_float_eq(result[0], 6.0f, 1e-5f); !r)
            return r;
        if (auto r = check_float_eq(result[1], 2.0f, 1e-5f); !r)
            return r;
        if (auto r = check_float_eq(result[2], 3.0f, 1e-5f); !r)
            return r;
        return {};
    });

    runner.add("transform_point applies scale", [] -> TestResult {
        // Scale matrix: scale by (2, 3, 4)
        float data[16] = {2, 0, 0, 0, 0, 3, 0, 0, 0, 0, 4, 0, 0, 0, 0, 1};
        m3::Mat4 m{data};
        m3::vec3 p{1.0f, 1.0f, 1.0f};
        auto result = m3::transform_point(m, p);
        if (auto r = check_float_eq(result[0], 2.0f, 1e-5f); !r)
            return r;
        if (auto r = check_float_eq(result[1], 3.0f, 1e-5f); !r)
            return r;
        if (auto r = check_float_eq(result[2], 4.0f, 1e-5f); !r)
            return r;
        return {};
    });

    runner.add("transform_point perspective divide", [] -> TestResult {
        // Matrix that produces w=2: scale w component by 2
        // Identity except m[3][3] = 2 (column 3, row 3)
        float data[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 2};
        m3::Mat4 m{data};
        m3::vec3 p{4.0f, 6.0f, 8.0f};
        auto result = m3::transform_point(m, p);
        // After perspective divide: (4/2, 6/2, 8/2) = (2, 3, 4)
        if (auto r = check_float_eq(result[0], 2.0f, 1e-5f); !r)
            return r;
        if (auto r = check_float_eq(result[1], 3.0f, 1e-5f); !r)
            return r;
        if (auto r = check_float_eq(result[2], 4.0f, 1e-5f); !r)
            return r;
        return {};
    });

    runner.add("transform_point combined scale and translate", [] -> TestResult {
        // Scale by 2 then translate by (1, 0, 0)
        // Matrix = Translate * Scale (column-major)
        // Result: point (x,y,z) -> (2x+1, 2y, 2z)
        float data[16] = {2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 1, 0, 0, 1};
        m3::Mat4 m{data};
        m3::vec3 p{1.0f, 2.0f, 3.0f};
        auto result = m3::transform_point(m, p);
        // (2*1+1, 2*2, 2*3) = (3, 4, 6)
        if (auto r = check_float_eq(result[0], 3.0f, 1e-5f); !r)
            return r;
        if (auto r = check_float_eq(result[1], 4.0f, 1e-5f); !r)
            return r;
        if (auto r = check_float_eq(result[2], 6.0f, 1e-5f); !r)
            return r;
        return {};
    });

    // ---- transform_direction ----

    runner.add("transform_direction ignores translation", [] -> TestResult {
        // Translation matrix: translate by (5, 0, 0)
        float data[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 5, 0, 0, 1};
        m3::Mat4 m{data};
        m3::vec3 d{1.0f, 0.0f, 0.0f};
        auto result = m3::transform_direction(m, d);
        if (auto r = check_float_eq(result[0], 1.0f, 1e-5f); !r)
            return r;
        if (auto r = check_float_eq(result[1], 0.0f, 1e-5f); !r)
            return r;
        if (auto r = check_float_eq(result[2], 0.0f, 1e-5f); !r)
            return r;
        return {};
    });

    // ---- Mat4 * Vec4 direct multiply ----

    runner.add("Mat4 * Vec4 direct multiply", [] -> TestResult {
        // Identity matrix
        float data[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
        m3::Mat4 m{data};
        m3::vec4 v{1.0f, 2.0f, 3.0f, 1.0f};
        auto result = m * v;
        if (auto r = check_float_eq(result[0], 1.0f, 1e-5f); !r)
            return r;
        if (auto r = check_float_eq(result[1], 2.0f, 1e-5f); !r)
            return r;
        if (auto r = check_float_eq(result[2], 3.0f, 1e-5f); !r)
            return r;
        if (auto r = check_float_eq(result[3], 1.0f, 1e-5f); !r)
            return r;
        return {};
    });

    return runner.run();
}
