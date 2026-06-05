// M3Lib — Vec type tests: construction, accessors, operators, comparison, formatter.
//    xmake build test_vec && xmake run test_vec
//    mcpp build -p test_vec && ./target/*/bin/test_vec

import std;
import m3;

// === test infrastructure ===

import test_common;

// === test cases ===

int main() {
    TestRunner runner;

    // ---- Construction ----

    runner.add("default construct zero-initialises", [] -> TestResult {
        m3::Vec<3, float> v{};
        if (auto r = check(v[0] == 0.0f && v[1] == 0.0f && v[2] == 0.0f); !r)
            return r;
        return {};
    });

    runner.add("broadcast scalar construct", [] -> TestResult {
        m3::Vec<3, float> v{2.5f};
        if (auto r = check_float_eq(v[0], 2.5f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(v[1], 2.5f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(v[2], 2.5f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("copy construct preserves values", [] -> TestResult {
        m3::Vec<3, float> a{3.0f};
        m3::Vec<3, float> b = a;
        if (auto r = check_float_eq(b[0], 3.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(b[1], 3.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(b[2], 3.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("default construct int is zero", [] -> TestResult {
        m3::Vec<4, int> v{};
        if (auto r = check(v[0] == 0 && v[1] == 0 && v[2] == 0 && v[3] == 0); !r)
            return r;
        return {};
    });

    runner.add("broadcast construct double", [] -> TestResult {
        m3::Vec<2, double> v{1.5};
        if (auto r = check_float_eq(v[0], 1.5, 1e-12); !r)
            return r;
        if (auto r = check_float_eq(v[1], 1.5, 1e-12); !r)
            return r;
        return {};
    });

    runner.add("component construct vec2", [] -> TestResult {
        m3::Vec<2, float> v{3.0f, 7.0f};
        if (auto r = check_float_eq(v[0], 3.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(v[1], 7.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("component construct vec3", [] -> TestResult {
        m3::Vec<3, int> v{1, 2, 3};
        if (auto r = check(v[0] == 1 && v[1] == 2 && v[2] == 3); !r)
            return r;
        return {};
    });

    runner.add("component construct vec4", [] -> TestResult {
        m3::Vec<4, float> v{1.0f, 2.0f, 3.0f, 4.0f};
        if (auto r = check_float_eq(v[0], 1.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(v[3], 4.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("pointer construct vec2", [] -> TestResult {
        float arr[] = {7.0f, 13.0f};
        m3::Vec<2, float> v{arr};
        if (auto r = check_float_eq(v[0], 7.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(v[1], 13.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("pointer construct vec3", [] -> TestResult {
        int arr[] = {10, 20, 30};
        m3::Vec<3, int> v{arr};
        if (auto r = check(v[0] == 10 && v[1] == 20 && v[2] == 30); !r)
            return r;
        return {};
    });

    runner.add("pointer construct vec4", [] -> TestResult {
        float arr[] = {0.1f, 0.2f, 0.3f, 0.4f};
        m3::Vec<4, float> v{arr};
        if (auto r = check_float_eq(v[0], 0.1f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(v[3], 0.4f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("pointer construct roundtrip via value_ptr", [] -> TestResult {
        float src[] = {1.0f, 2.0f, 3.0f};
        m3::Vec<3, float> v{src};
        const float* internal = v.value_ptr();
        for (int i = 0; i < 3; ++i) {
            if (auto r = check_float_eq(internal[i], src[i], 1e-6f); !r)
                return r;
        }
        return {};
    });

    // ---- Vec1 accessors ----

    runner.add("vec1 x/r/s accessors alias element 0", [] -> TestResult {
        m3::Vec<1, float> v{};
        v.x() = 7.0f;
        if (auto r = check_float_eq(v.x(), 7.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(v.r(), 7.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(v.s(), 7.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(v[0], 7.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("vec1 const accessor", [] -> TestResult {
        const m3::Vec<1, float> v{42.0f};
        if (auto r = check_float_eq(v.x(), 42.0f, 1e-6f); !r)
            return r;
        return {};
    });

    // ---- Vec2 accessors ----

    runner.add("vec2 x/y accessors", [] -> TestResult {
        m3::Vec<2, float> v{};
        v.x() = 1.0f;
        v.y() = 2.0f;
        if (auto r = check_float_eq(v.x(), 1.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(v.y(), 2.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("vec2 r/g alias x/y", [] -> TestResult {
        m3::Vec<2, float> v{};
        v.r() = 0.2f;
        v.g() = 0.8f;
        if (auto r = check_float_eq(v.x(), 0.2f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(v.y(), 0.8f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("vec2 s/t accessors", [] -> TestResult {
        m3::Vec<2, float> v{};
        v.s() = 0.1f;
        v.t() = 0.9f;
        if (auto r = check_float_eq(v[0], 0.1f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(v[1], 0.9f, 1e-6f); !r)
            return r;
        return {};
    });

    // ---- Vec3 accessors ----

    runner.add("vec3 x/y/z accessors", [] -> TestResult {
        m3::Vec<3, float> v{};
        v.x() = 1.0f;
        v.y() = 2.0f;
        v.z() = 3.0f;
        if (auto r = check_float_eq(v.x(), 1.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(v.y(), 2.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(v.z(), 3.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("vec3 r/g/b alias x/y/z", [] -> TestResult {
        m3::Vec<3, float> v{};
        v.r() = 1.0f;
        v.g() = 0.5f;
        v.b() = 0.0f;
        if (auto r = check_float_eq(v[0], 1.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(v[1], 0.5f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(v[2], 0.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("vec3 s/t/p accessors", [] -> TestResult {
        m3::Vec<3, float> v{};
        v.s() = 0.1f;
        v.t() = 0.2f;
        v.p() = 0.3f;
        if (auto r = check_float_eq(v[0], 0.1f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(v[1], 0.2f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(v[2], 0.3f, 1e-6f); !r)
            return r;
        return {};
    });

    // ---- Vec4 accessors ----

    runner.add("vec4 x/y/z/w accessors", [] -> TestResult {
        m3::Vec<4, float> v{};
        v.x() = 1.0f;
        v.y() = 2.0f;
        v.z() = 3.0f;
        v.w() = 4.0f;
        if (auto r = check_float_eq(v.x(), 1.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(v.w(), 4.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("vec4 r/g/b/a alias x/y/z/w", [] -> TestResult {
        m3::Vec<4, float> v{};
        v.r() = 1.0f;
        v.g() = 0.5f;
        v.b() = 0.0f;
        v.a() = 1.0f;
        if (auto r = check_float_eq(v[0], 1.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(v[3], 1.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("vec4 s/t/p/q accessors", [] -> TestResult {
        m3::Vec<4, float> v{};
        v.s() = 0.1f;
        v.t() = 0.2f;
        v.p() = 0.3f;
        v.q() = 0.4f;
        if (auto r = check_float_eq(v[3], 0.4f, 1e-6f); !r)
            return r;
        return {};
    });

    // ---- operator[] / value_ptr ----

    runner.add("operator[] read broadcast values", [] -> TestResult {
        m3::Vec<3, float> v{1.0f};
        if (auto r = check_float_eq(v[0], 1.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(v[1], 1.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(v[2], 1.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("operator[] write then read", [] -> TestResult {
        m3::Vec<3, float> v{};
        v[0] = 10.0f;
        v[1] = 20.0f;
        v[2] = 30.0f;
        if (auto r = check_float_eq(v[0], 10.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(v[1], 20.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(v[2], 30.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("operator[] const access", [] -> TestResult {
        const m3::Vec<2, int> v{5};
        if (auto r = check(v[0] == 5 && v[1] == 5); !r)
            return r;
        return {};
    });

    runner.add("value_ptr returns non-null and correct data", [] -> TestResult {
        m3::Vec<3, float> v{};
        v[0] = 1.0f;
        v[1] = 2.0f;
        v[2] = 3.0f;
        const float* ptr = v.value_ptr();
        if (auto r = check(ptr != nullptr); !r)
            return r;
        if (auto r = check_float_eq(ptr[0], 1.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(ptr[1], 2.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(ptr[2], 3.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("value_ptr const returns non-null", [] -> TestResult {
        const m3::Vec<2, float> v{4.0f};
        const float* ptr = v.value_ptr();
        if (auto r = check(ptr != nullptr); !r)
            return r;
        if (auto r = check_float_eq(ptr[0], 4.0f, 1e-6f); !r)
            return r;
        return {};
    });

    // ---- Component-wise operators ----

    runner.add("vector add", [] -> TestResult {
        m3::Vec<3, float> a{1.0f, 2.0f, 3.0f};
        m3::Vec<3, float> b{4.0f, 5.0f, 6.0f};
        auto c = a + b;
        if (auto r = check_float_eq(c[0], 5.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(c[1], 7.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(c[2], 9.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("vector subtract", [] -> TestResult {
        m3::Vec<3, float> a{10.0f, 8.0f, 6.0f};
        m3::Vec<3, float> b{1.0f, 2.0f, 3.0f};
        auto c = a - b;
        if (auto r = check_float_eq(c[0], 9.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(c[1], 6.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(c[2], 3.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("vector multiply hadamard", [] -> TestResult {
        m3::Vec<2, float> a{2.0f, 3.0f};
        m3::Vec<2, float> b{4.0f, 5.0f};
        auto c = a * b;
        if (auto r = check_float_eq(c[0], 8.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(c[1], 15.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("vector divide", [] -> TestResult {
        m3::Vec<2, float> a{8.0f, 12.0f};
        m3::Vec<2, float> b{2.0f, 3.0f};
        auto c = a / b;
        if (auto r = check_float_eq(c[0], 4.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(c[1], 4.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("vector negate", [] -> TestResult {
        m3::Vec<3, float> a{1.0f, -2.0f, 0.0f};
        auto c = -a;
        if (auto r = check_float_eq(c[0], -1.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(c[1], 2.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(c[2], 0.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("negate zero vector is zero", [] -> TestResult {
        m3::Vec<4, int> a{0};
        auto c = -a;
        if (auto r = check(c[0] == 0 && c[1] == 0 && c[2] == 0 && c[3] == 0); !r)
            return r;
        return {};
    });

    runner.add("negate int vector", [] -> TestResult {
        m3::Vec<2, int> a{5, -3};
        auto c = -a;
        if (auto r = check(c[0] == -5 && c[1] == 3); !r)
            return r;
        return {};
    });

    // ---- Scalar operators ----

    runner.add("vec mul scalar right", [] -> TestResult {
        m3::Vec<3, float> a{1.0f, 2.0f, 3.0f};
        auto c = a * 2.0f;
        if (auto r = check_float_eq(c[0], 2.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(c[1], 4.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(c[2], 6.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("vec mul scalar left", [] -> TestResult {
        m3::Vec<3, float> a{1.0f, 2.0f, 3.0f};
        auto c = 2.0f * a;
        if (auto r = check_float_eq(c[0], 2.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(c[1], 4.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(c[2], 6.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("vec div scalar", [] -> TestResult {
        m3::Vec<2, float> a{10.0f, 20.0f};
        auto c = a / 2.0f;
        if (auto r = check_float_eq(c[0], 5.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(c[1], 10.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("vec add scalar", [] -> TestResult {
        m3::Vec<3, float> a{1.0f, 2.0f, 3.0f};
        auto c = a + 10.0f;
        if (auto r = check_float_eq(c[0], 11.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(c[1], 12.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(c[2], 13.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("scalar add vec", [] -> TestResult {
        m3::Vec<3, float> a{1.0f, 2.0f, 3.0f};
        auto c = 10.0f + a;
        if (auto r = check_float_eq(c[0], 11.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(c[1], 12.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(c[2], 13.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("vec sub scalar", [] -> TestResult {
        m3::Vec<2, float> a{10.0f, 20.0f};
        auto c = a - 3.0f;
        if (auto r = check_float_eq(c[0], 7.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(c[1], 17.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("scalar sub vec", [] -> TestResult {
        m3::Vec<2, float> a{3.0f, 5.0f};
        auto c = 10.0f - a;
        if (auto r = check_float_eq(c[0], 7.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(c[1], 5.0f, 1e-6f); !r)
            return r;
        return {};
    });

    // ---- Compound assignment operators ----
    // Verify that +=, -=, *=, /= produce correct results and mutate
    // the left-hand operand in place.
    //
    // ---- 复合赋值运算符 ----
    // 验证 +=、-=、*=、/= 产生正确结果并原地修改左操作数。

    runner.add("vector +=", [] -> TestResult {
        m3::Vec<3, float> a{1.0f, 2.0f, 3.0f};
        m3::Vec<3, float> b{4.0f, 5.0f, 6.0f};
        a += b;
        if (auto r = check_float_eq(a[0], 5.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(a[1], 7.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(a[2], 9.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("vector -=", [] -> TestResult {
        m3::Vec<3, float> a{10.0f, 8.0f, 6.0f};
        m3::Vec<3, float> b{1.0f, 2.0f, 3.0f};
        a -= b;
        if (auto r = check_float_eq(a[0], 9.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(a[1], 6.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(a[2], 3.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("vector *=", [] -> TestResult {
        m3::Vec<2, float> a{2.0f, 3.0f};
        m3::Vec<2, float> b{4.0f, 5.0f};
        a *= b;
        if (auto r = check_float_eq(a[0], 8.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(a[1], 15.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("vector /=", [] -> TestResult {
        m3::Vec<2, float> a{8.0f, 12.0f};
        m3::Vec<2, float> b{2.0f, 3.0f};
        a /= b;
        if (auto r = check_float_eq(a[0], 4.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(a[1], 4.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("vector *= scalar", [] -> TestResult {
        m3::Vec<3, float> a{1.0f, 2.0f, 3.0f};
        a *= 2.0f;
        if (auto r = check_float_eq(a[0], 2.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(a[1], 4.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(a[2], 6.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("vector /= scalar", [] -> TestResult {
        m3::Vec<2, float> a{10.0f, 20.0f};
        a /= 2.0f;
        if (auto r = check_float_eq(a[0], 5.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(a[1], 10.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("vector += scalar", [] -> TestResult {
        m3::Vec<3, float> a{1.0f, 2.0f, 3.0f};
        a += 10.0f;
        if (auto r = check_float_eq(a[0], 11.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(a[1], 12.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(a[2], 13.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("vector -= scalar", [] -> TestResult {
        m3::Vec<2, float> a{10.0f, 20.0f};
        a -= 3.0f;
        if (auto r = check_float_eq(a[0], 7.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(a[1], 17.0f, 1e-6f); !r)
            return r;
        return {};
    });

    // ---- Comparison ----

    runner.add("equal vectors", [] -> TestResult {
        m3::Vec<3, float> a{1.0f, 2.0f, 3.0f};
        m3::Vec<3, float> b{1.0f, 2.0f, 3.0f};
        if (auto r = check(a == b); !r)
            return r;
        return {};
    });

    runner.add("not equal vectors", [] -> TestResult {
        m3::Vec<3, float> a{1.0f, 2.0f, 3.0f};
        m3::Vec<3, float> b{1.0f, 2.0f, 4.0f};
        if (auto r = check(a != b); !r)
            return r;
        if (auto r = check(!(a == b)); !r)
            return r;
        return {};
    });

    runner.add("equal zero vectors", [] -> TestResult {
        m3::Vec<2, float> a{0.0f, 0.0f};
        m3::Vec<2, float> b{0.0f, 0.0f};
        if (auto r = check(a == b); !r)
            return r;
        return {};
    });

    // ---- Edge cases ----

    runner.add("zero add zero is zero", [] -> TestResult {
        m3::Vec<3, float> a{};
        m3::Vec<3, float> b{};
        auto c = a + b;
        if (auto r = check_float_eq(c[0], 0.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(c[1], 0.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(c[2], 0.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("identity mul by one", [] -> TestResult {
        m3::Vec<4, float> a{3.0f, 7.0f, 1.0f, 9.0f};
        auto c = a * 1.0f;
        if (auto r = check(c == a); !r)
            return r;
        return {};
    });

    runner.add("identity add zero", [] -> TestResult {
        m3::Vec<3, float> a{5.0f, 3.0f, 1.0f};
        auto c = a + 0.0f;
        if (auto r = check(c == a); !r)
            return r;
        return {};
    });

    runner.add("negate twice is identity", [] -> TestResult {
        m3::Vec<3, float> a{1.0f, -2.0f, 3.0f};
        auto c = -(-a);
        if (auto r = check(c == a); !r)
            return r;
        return {};
    });

    runner.add("subtract self is zero", [] -> TestResult {
        m3::Vec<3, float> a{5.0f, 5.0f, 5.0f};
        auto c = a - a;
        if (auto r = check_float_eq(c[0], 0.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(c[1], 0.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(c[2], 0.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("negative values add correctly", [] -> TestResult {
        m3::Vec<3, float> a{-1.0f, -2.0f, -3.0f};
        m3::Vec<3, float> b{-4.0f, 1.0f, 2.0f};
        auto c = a + b;
        if (auto r = check_float_eq(c[0], -5.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(c[1], -1.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(c[2], -1.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("int multiply", [] -> TestResult {
        m3::Vec<4, int> a{2, 3, 4, 5};
        auto c = a * 3;
        if (auto r = check(c[0] == 6 && c[1] == 9 && c[2] == 12 && c[3] == 15); !r)
            return r;
        return {};
    });

    runner.add("int division truncation", [] -> TestResult {
        // C++ integer division truncates toward zero
        m3::Vec<2, int> a{5, 7};
        auto c = a / 2;
        if (auto r = check(c[0] == 2 && c[1] == 3); !r)
            return r;
        return {};
    });

    runner.add("large float values", [] -> TestResult {
        m3::Vec<2, float> a{1.0e10f, 2.0e10f};
        m3::Vec<2, float> b{1.0e10f, 1.0e10f};
        auto c = a + b;
        if (auto r = check_float_eq(c[0], 2.0e10f, 1e4f); !r)
            return r;
        if (auto r = check_float_eq(c[1], 3.0e10f, 1e4f); !r)
            return r;
        return {};
    });

    runner.add("small float values", [] -> TestResult {
        m3::Vec<2, float> a{1.0e-10f, 2.0e-10f};
        auto c = a * 2.0f;
        if (auto r = check_float_eq(c[0], 2.0e-10f, 1e-16f); !r)
            return r;
        if (auto r = check_float_eq(c[1], 4.0e-10f, 1e-16f); !r)
            return r;
        return {};
    });

    // ---- Integration ----

    runner.add("vec1 all vector operations", [] -> TestResult {
        m3::Vec<1, float> a{3.0f};
        m3::Vec<1, float> b{2.0f};
        if (auto r = check_float_eq((a + b)[0], 5.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq((a - b)[0], 1.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq((a * b)[0], 6.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq((a / b)[0], 1.5f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("vec1 all scalar operations", [] -> TestResult {
        m3::Vec<1, float> a{5.0f};
        if (auto r = check_float_eq((a * 2.0f)[0], 10.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq((3.0f * a)[0], 15.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq((a + 1.0f)[0], 6.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq((10.0f - a)[0], 5.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("vec4 all vector operations", [] -> TestResult {
        m3::Vec<4, float> a{1.0f, 2.0f, 3.0f, 4.0f};
        m3::Vec<4, float> b{2.0f, 2.0f, 2.0f, 2.0f};
        auto sum = a + b;
        if (auto r = check_float_eq(sum[0], 3.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(sum[1], 4.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(sum[2], 5.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(sum[3], 6.0f, 1e-6f); !r)
            return r;
        auto neg = -a;
        if (auto r = check_float_eq(neg[3], -4.0f, 1e-6f); !r)
            return r;
        return {};
    });

    // ---- std::formatter ----

    runner.add("format vec3 int", [] -> TestResult {
        m3::Vec<3, int> v{1, 2, 3};
        auto s = std::format("{}", v);
        if (auto r = check(s == "[1, 2, 3]"); !r)
            return r;
        return {};
    });

    runner.add("format vec1 single element", [] -> TestResult {
        m3::Vec<1, int> v{42};
        auto s = std::format("{}", v);
        if (auto r = check(s == "[42]"); !r)
            return r;
        return {};
    });

    runner.add("format vec4", [] -> TestResult {
        m3::Vec<4, int> v{10, 20, 30, 40};
        auto s = std::format("{}", v);
        if (auto r = check(s == "[10, 20, 30, 40]"); !r)
            return r;
        return {};
    });

    runner.add("format vec2 float produces bracketed output", [] -> TestResult {
        m3::Vec<2, float> v{3.5f, -7.25f};
        auto s = std::format("{}", v);
        if (auto r = check(s.size() > 0 && s[0] == '[' && s.back() == ']'); !r)
            return r;
        return {};
    });

    return runner.run();
}
