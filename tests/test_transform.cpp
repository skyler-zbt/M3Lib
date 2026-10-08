
import std;
import m3;

import test_common;

int main() {
    TestRunner runner;

    runner.add("transform_point applies translation", [] -> TestResult {

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

        float data[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 2};
        m3::Mat4 m{data};
        m3::vec3 p{4.0f, 6.0f, 8.0f};
        auto result = m3::transform_point(m, p);

        if (auto r = check_float_eq(result[0], 2.0f, 1e-5f); !r)
            return r;
        if (auto r = check_float_eq(result[1], 3.0f, 1e-5f); !r)
            return r;
        if (auto r = check_float_eq(result[2], 4.0f, 1e-5f); !r)
            return r;
        return {};
    });

    runner.add("transform_point combined scale and translate", [] -> TestResult {

        float data[16] = {2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 1, 0, 0, 1};
        m3::Mat4 m{data};
        m3::vec3 p{1.0f, 2.0f, 3.0f};
        auto result = m3::transform_point(m, p);

        if (auto r = check_float_eq(result[0], 3.0f, 1e-5f); !r)
            return r;
        if (auto r = check_float_eq(result[1], 4.0f, 1e-5f); !r)
            return r;
        if (auto r = check_float_eq(result[2], 6.0f, 1e-5f); !r)
            return r;
        return {};
    });

    runner.add("transform_direction ignores translation", [] -> TestResult {

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

    runner.add("Mat4 * Vec4 direct multiply", [] -> TestResult {

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
