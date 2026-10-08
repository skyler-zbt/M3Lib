
import std;
import m3;

import test_common;

int main() {
    TestRunner runner;

    runner.add("structured binding vec2", [] -> TestResult {
        m3::Vec<2, float> v{};
        v[0] = 3.0f;
        v[1] = 7.0f;
        auto [a, b] = v;
        if (auto r = check_float_eq(a, 3.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(b, 7.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("structured binding vec3", [] -> TestResult {
        m3::Vec<3, int> v{};
        v[0] = 10;
        v[1] = 20;
        v[2] = 30;
        auto [x, y, z] = v;
        if (auto r = check(x == 10 && y == 20 && z == 30); !r)
            return r;
        return {};
    });

    runner.add("structured binding vec4 modify via reference", [] -> TestResult {
        m3::Vec<4, int> v{0};
        auto& [a, b, c, d] = v;
        a = 1;
        b = 2;
        c = 3;
        d = 4;
        if (auto r = check(v[0] == 1 && v[1] == 2 && v[2] == 3 && v[3] == 4); !r)
            return r;
        return {};
    });

    runner.add("structured binding vec1 single element", [] -> TestResult {
        m3::Vec<1, float> v{3.14f};
        auto [x] = v;
        if (auto r = check_float_eq(x, 3.14f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("get<I> reference modifies element", [] -> TestResult {
        m3::Vec<3, float> v{};
        v[0] = 1.0f;
        v[1] = 2.0f;
        v[2] = 3.0f;
        if (auto r = check_float_eq(m3::get<0>(v), 1.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(m3::get<1>(v), 2.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(m3::get<2>(v), 3.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("get<I> constexpr", [] -> TestResult {
        constexpr m3::Vec<2, int> v{5};
        static_assert(m3::get<0>(v) == 5);
        static_assert(m3::get<1>(v) == 5);
        return {};
    });

    runner.add("tuple_size matches dimension", [] -> TestResult {
        static_assert(std::tuple_size_v<m3::Vec<1, int>> == 1);
        static_assert(std::tuple_size_v<m3::Vec<2, float>> == 2);
        static_assert(std::tuple_size_v<m3::Vec<3, double>> == 3);
        static_assert(std::tuple_size_v<m3::Vec<4, int>> == 4);
        return {};
    });

    runner.add("tuple_element yields element type", [] -> TestResult {
        static_assert(std::is_same_v<std::tuple_element_t<0, m3::Vec<2, int>>, int>);
        static_assert(std::is_same_v<std::tuple_element_t<1, m3::Vec<3, float>>, float>);
        return {};
    });

    runner.add("operator[] valid index 0 passes contract", [] -> TestResult {
        m3::Vec<4, int> v{0};
        v[0] = 42;
        if (auto r = check(v[0] == 42); !r)
            return r;
        return {};
    });

    runner.add("operator[] valid index L-1 passes contract", [] -> TestResult {
        m3::Vec<4, int> v{0};
        v[3] = 99;
        if (auto r = check(v[3] == 99); !r)
            return r;
        return {};
    });

    runner.add("operator[] const valid access passes contract", [] -> TestResult {
        const m3::Vec<3, float> v{1.0f, 2.0f, 3.0f};

        if (auto r = check_float_eq(v[0], 1.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(v[1], 2.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(v[2], 3.0f, 1e-6f); !r)
            return r;
        return {};
    });

    return runner.run();
}
