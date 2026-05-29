// M3Lib — Vec test suite
//    xmake build tests && xmake run test_vec
//    mcpp build -p test_vec && ./target/*/bin/test_vec
//
// When a test fails, fix the library, not the test.

module;
#include <cstdio>
#include <cmath>
#include <source_location>
#include <string_view>
#include <vector>
#include <functional>
#include <stdexcept>
#include <string>

import m3;

// ---- test infrastructure ----

namespace {

// <source_location> replaces __FILE__/__LINE__ macros
inline void check(
    bool expr,
    std::source_location loc = std::source_location::current())
{
    if (!expr) {
        std::string msg = std::string{loc.file_name()}
            + ":" + std::to_string(loc.line())
            + ": check failed";
        throw std::runtime_error{msg};
    }
}

template<typename T>
void check_float_eq(
    T a, T b, T eps,
    std::source_location loc = std::source_location::current())
{
    if (std::fabs(static_cast<double>(a) - static_cast<double>(b))
        > static_cast<double>(eps)) {
        std::string msg = std::string{loc.file_name()}
            + ":" + std::to_string(loc.line())
            + ": |" + std::to_string(a) + " - " + std::to_string(b)
            + "| > " + std::to_string(eps);
        throw std::runtime_error{msg};
    }
}

// 测试运行器
class TestRunner {
public:
    using TestFunc = std::function<void()>;

    void add(std::string_view name, TestFunc func) {
        tests_.push_back({std::string{name}, std::move(func)});
    }

    int run() {
        int passed = 0;
        int failed = 0;

        for (auto& [name, func] : tests_) {
            std::printf("  %-48s", name.c_str());
            try {
                func();
                std::printf("PASSED\n");
                ++passed;
            } catch (const std::exception& e) {
                std::printf("FAILED\n");
                std::fprintf(stderr, "    %s\n", e.what());
                ++failed;
            }
        }

        std::printf("\n==========================\n");
        std::printf("%d passed, %d failed\n", passed, failed);
        return failed > 0 ? 1 : 0;
    }

private:
    struct Entry {
        std::string name;
        TestFunc   func;
    };
    std::vector<Entry> tests_;
};

} // anonymous namespace


// ---- 测试用例 ----

int main()
{
    TestRunner runner;

    // ====== 构造 ======
    runner.add("vec default construct", [] {
        m3::Vec<3, float> v{};
        check(v[0] == 0.0f);
        check(v[1] == 0.0f);
        check(v[2] == 0.0f);
    });

    runner.add("vec scalar construct", [] {
        m3::Vec<3, float> v{2.5f};
        check_float_eq(v[0], 2.5f, 1e-6f);
        check_float_eq(v[1], 2.5f, 1e-6f);
        check_float_eq(v[2], 2.5f, 1e-6f);
    });

    runner.add("vec copy construct", [] {
        m3::Vec<3, float> a{3.0f};
        m3::Vec<3, float> b = a;
        check_float_eq(b[0], 3.0f, 1e-6f);
        check_float_eq(b[1], 3.0f, 1e-6f);
        check_float_eq(b[2], 3.0f, 1e-6f);
    });

    runner.add("vec int default zero", [] {
        m3::Vec<4, int> v{};
        check(v[0] == 0);
        check(v[1] == 0);
        check(v[2] == 0);
        check(v[3] == 0);
    });

    runner.add("vec double construct", [] {
        m3::Vec<2, double> v{1.5};
        check_float_eq(v[0], 1.5, 1e-12);
        check_float_eq(v[1], 1.5, 1e-12);
    });

    // ====== Vec1 访问器 ======
    runner.add("vec1 accessors xyz", [] {
        m3::Vec<1, float> v{};
        v.x() = 7.0f;
        check_float_eq(v.x(), 7.0f, 1e-6f);
    });

    runner.add("vec1 accessors rgb share memory with xyz", [] {
        m3::Vec<1, float> v{};
        v.r() = 0.5f;
        check_float_eq(v.x(), 0.5f, 1e-6f);
    });

    runner.add("vec1 accessors stp", [] {
        m3::Vec<1, float> v{};
        v.s() = 3.0f;
        check_float_eq(v.s(), 3.0f, 1e-6f);
        check_float_eq(v[0], 3.0f, 1e-6f);
    });

    runner.add("vec1 const accessor", [] {
        const m3::Vec<1, float> v{42.0f};
        check_float_eq(v.x(), 42.0f, 1e-6f);
    });

    // ====== Vec2 访问器 ======
    runner.add("vec2 xy accessors", [] {
        m3::Vec<2, float> v{};
        v.x() = 1.0f; v.y() = 2.0f;
        check_float_eq(v.x(), 1.0f, 1e-6f);
        check_float_eq(v.y(), 2.0f, 1e-6f);
    });

    runner.add("vec2 rg accessors alias xy", [] {
        m3::Vec<2, float> v{};
        v.r() = 0.2f; v.g() = 0.8f;
        check_float_eq(v.x(), 0.2f, 1e-6f);
        check_float_eq(v.y(), 0.8f, 1e-6f);
    });

    runner.add("vec2 st accessors", [] {
        m3::Vec<2, float> v{};
        v.s() = 0.1f; v.t() = 0.9f;
        check_float_eq(v[0], 0.1f, 1e-6f);
        check_float_eq(v[1], 0.9f, 1e-6f);
    });

    // ====== Vec3 访问器 ======
    runner.add("vec3 xyz accessors", [] {
        m3::Vec<3, float> v{};
        v.x() = 1.0f; v.y() = 2.0f; v.z() = 3.0f;
        check_float_eq(v.x(), 1.0f, 1e-6f);
        check_float_eq(v.y(), 2.0f, 1e-6f);
        check_float_eq(v.z(), 3.0f, 1e-6f);
    });

    runner.add("vec3 rgb accessors alias xyz", [] {
        m3::Vec<3, float> v{};
        v.r() = 1.0f; v.g() = 0.5f; v.b() = 0.0f;
        check_float_eq(v[0], 1.0f, 1e-6f);
        check_float_eq(v[1], 0.5f, 1e-6f);
        check_float_eq(v[2], 0.0f, 1e-6f);
    });

    runner.add("vec3 stp accessors", [] {
        m3::Vec<3, float> v{};
        v.s() = 0.1f; v.t() = 0.2f; v.p() = 0.3f;
        check_float_eq(v[0], 0.1f, 1e-6f);
        check_float_eq(v[1], 0.2f, 1e-6f);
        check_float_eq(v[2], 0.3f, 1e-6f);
    });

    // ====== Vec4 访问器 ======
    runner.add("vec4 xyzw accessors", [] {
        m3::Vec<4, float> v{};
        v.x() = 1.0f; v.y() = 2.0f; v.z() = 3.0f; v.w() = 4.0f;
        check_float_eq(v.x(), 1.0f, 1e-6f);
        check_float_eq(v.y(), 2.0f, 1e-6f);
        check_float_eq(v.z(), 3.0f, 1e-6f);
        check_float_eq(v.w(), 4.0f, 1e-6f);
    });

    runner.add("vec4 rgba accessors alias xyzw", [] {
        m3::Vec<4, float> v{};
        v.r() = 1.0f; v.g() = 0.5f; v.b() = 0.0f; v.a() = 1.0f;
        check_float_eq(v[0], 1.0f, 1e-6f);
        check_float_eq(v[3], 1.0f, 1e-6f);
    });

    runner.add("vec4 stpq accessors", [] {
        m3::Vec<4, float> v{};
        v.s() = 0.1f; v.t() = 0.2f; v.p() = 0.3f; v.q() = 0.4f;
        check_float_eq(v[3], 0.4f, 1e-6f);
    });

    // ====== operator[] / value_ptr ======
    runner.add("vec operator[] read", [] {
        m3::Vec<3, float> v{1.0f};
        check_float_eq(v[0], 1.0f, 1e-6f);
        check_float_eq(v[1], 1.0f, 1e-6f);
        check_float_eq(v[2], 1.0f, 1e-6f);
    });

    runner.add("vec operator[] write", [] {
        m3::Vec<3, float> v{};
        v[0] = 10.0f; v[1] = 20.0f; v[2] = 30.0f;
        check_float_eq(v[0], 10.0f, 1e-6f);
        check_float_eq(v[1], 20.0f, 1e-6f);
        check_float_eq(v[2], 30.0f, 1e-6f);
    });

    runner.add("vec const operator[]", [] {
        const m3::Vec<2, int> v{5};
        check(v[0] == 5);
        check(v[1] == 5);
    });

    runner.add("vec value_ptr", [] {
        m3::Vec<3, float> v{};
        v[0] = 1.0f; v[1] = 2.0f; v[2] = 3.0f;
        const float* ptr = v.value_ptr();
        check(ptr != nullptr);
        check_float_eq(ptr[0], 1.0f, 1e-6f);
        check_float_eq(ptr[1], 2.0f, 1e-6f);
        check_float_eq(ptr[2], 3.0f, 1e-6f);
    });

    runner.add("vec value_ptr const", [] {
        const m3::Vec<2, float> v{4.0f};
        const float* ptr = v.value_ptr();
        check(ptr != nullptr);
        check_float_eq(ptr[0], 4.0f, 1e-6f);
    });

    // ====== 向量-向量运算 ======
    runner.add("vec add", [] {
        m3::Vec<3, float> a{}; a[0] = 1.0f; a[1] = 2.0f; a[2] = 3.0f;
        m3::Vec<3, float> b{}; b[0] = 4.0f; b[1] = 5.0f; b[2] = 6.0f;
        auto c = a + b;
        check_float_eq(c[0], 5.0f, 1e-6f);
        check_float_eq(c[1], 7.0f, 1e-6f);
        check_float_eq(c[2], 9.0f, 1e-6f);
    });

    runner.add("vec sub", [] {
        m3::Vec<3, float> a{}; a[0] = 10.0f; a[1] = 8.0f; a[2] = 6.0f;
        m3::Vec<3, float> b{}; b[0] = 1.0f;  b[1] = 2.0f;  b[2] = 3.0f;
        auto c = a - b;
        check_float_eq(c[0], 9.0f, 1e-6f);
        check_float_eq(c[1], 6.0f, 1e-6f);
        check_float_eq(c[2], 3.0f, 1e-6f);
    });

    runner.add("vec mul hadamard", [] {
        m3::Vec<2, float> a{}; a[0] = 2.0f; a[1] = 3.0f;
        m3::Vec<2, float> b{}; b[0] = 4.0f; b[1] = 5.0f;
        auto c = a * b;
        check_float_eq(c[0], 8.0f, 1e-6f);
        check_float_eq(c[1], 15.0f, 1e-6f);
    });

    runner.add("vec div", [] {
        m3::Vec<2, float> a{}; a[0] = 8.0f; a[1] = 12.0f;
        m3::Vec<2, float> b{}; b[0] = 2.0f; b[1] = 3.0f;
        auto c = a / b;
        check_float_eq(c[0], 4.0f, 1e-6f);
        check_float_eq(c[1], 4.0f, 1e-6f);
    });

    runner.add("vec negate", [] {
        m3::Vec<3, float> a{}; a[0] = 1.0f; a[1] = -2.0f; a[2] = 0.0f;
        auto c = -a;
        check_float_eq(c[0], -1.0f, 1e-6f);
        check_float_eq(c[1], 2.0f, 1e-6f);
        check_float_eq(c[2], 0.0f, 1e-6f);
    });

    runner.add("vec negate zero", [] {
        m3::Vec<4, int> a{0};
        auto c = -a;
        check(c[0] == 0 && c[1] == 0 && c[2] == 0 && c[3] == 0);
    });

    runner.add("vec negate int", [] {
        m3::Vec<2, int> a{}; a[0] = 5; a[1] = -3;
        auto c = -a;
        check(c[0] == -5);
        check(c[1] == 3);
    });

    // ====== 向量-标量运算 ======
    runner.add("vec mul scalar right", [] {
        m3::Vec<3, float> a{}; a[0] = 1.0f; a[1] = 2.0f; a[2] = 3.0f;
        auto c = a * 2.0f;
        check_float_eq(c[0], 2.0f, 1e-6f);
        check_float_eq(c[1], 4.0f, 1e-6f);
        check_float_eq(c[2], 6.0f, 1e-6f);
    });

    runner.add("vec mul scalar left", [] {
        m3::Vec<3, float> a{}; a[0] = 1.0f; a[1] = 2.0f; a[2] = 3.0f;
        auto c = 2.0f * a;
        check_float_eq(c[0], 2.0f, 1e-6f);
        check_float_eq(c[1], 4.0f, 1e-6f);
        check_float_eq(c[2], 6.0f, 1e-6f);
    });

    runner.add("vec div scalar", [] {
        m3::Vec<2, float> a{}; a[0] = 10.0f; a[1] = 20.0f;
        auto c = a / 2.0f;
        check_float_eq(c[0], 5.0f, 1e-6f);
        check_float_eq(c[1], 10.0f, 1e-6f);
    });

    runner.add("vec add scalar", [] {
        m3::Vec<3, float> a{}; a[0] = 1.0f; a[1] = 2.0f; a[2] = 3.0f;
        auto c = a + 10.0f;
        check_float_eq(c[0], 11.0f, 1e-6f);
        check_float_eq(c[1], 12.0f, 1e-6f);
        check_float_eq(c[2], 13.0f, 1e-6f);
    });

    runner.add("vec scalar add", [] {
        m3::Vec<3, float> a{}; a[0] = 1.0f; a[1] = 2.0f; a[2] = 3.0f;
        auto c = 10.0f + a;
        check_float_eq(c[0], 11.0f, 1e-6f);
        check_float_eq(c[1], 12.0f, 1e-6f);
        check_float_eq(c[2], 13.0f, 1e-6f);
    });

    runner.add("vec sub scalar", [] {
        m3::Vec<2, float> a{}; a[0] = 10.0f; a[1] = 20.0f;
        auto c = a - 3.0f;
        check_float_eq(c[0], 7.0f, 1e-6f);
        check_float_eq(c[1], 17.0f, 1e-6f);
    });

    runner.add("vec scalar sub", [] {
        m3::Vec<2, float> a{}; a[0] = 3.0f; a[1] = 5.0f;
        auto c = 10.0f - a;
        check_float_eq(c[0], 7.0f, 1e-6f);
        check_float_eq(c[1], 5.0f, 1e-6f);
    });

    // ====== 比较运算 ======
    runner.add("vec equal", [] {
        m3::Vec<3, float> a{}; a[0] = 1.0f; a[1] = 2.0f; a[2] = 3.0f;
        m3::Vec<3, float> b{}; b[0] = 1.0f; b[1] = 2.0f; b[2] = 3.0f;
        check(a == b);
    });

    runner.add("vec not equal", [] {
        m3::Vec<3, float> a{}; a[0] = 1.0f; a[1] = 2.0f; a[2] = 3.0f;
        m3::Vec<3, float> b{}; b[0] = 1.0f; b[1] = 2.0f; b[2] = 4.0f;
        check(a != b);
        check(!(a == b));
    });

    runner.add("vec equal different", [] {
        m3::Vec<4, int> a{1};
        m3::Vec<4, int> b{}; b[0] = 1; b[1] = 1; b[2] = 1; b[3] = 0;
        check(a != b);
    });

    runner.add("vec equal zero", [] {
        m3::Vec<2, float> a{}; a[0] = 0.0f; a[1] = 0.0f;
        m3::Vec<2, float> b{}; b[0] = 0.0f; b[1] = 0.0f;
        check(a == b);
    });

    // ====== 边界情况 ======
    runner.add("vec zero add zero", [] {
        m3::Vec<3, float> a{};
        m3::Vec<3, float> b{};
        auto c = a + b;
        check_float_eq(c[0], 0.0f, 1e-6f);
        check_float_eq(c[1], 0.0f, 1e-6f);
        check_float_eq(c[2], 0.0f, 1e-6f);
    });

    runner.add("vec identity mul one", [] {
        m3::Vec<4, float> a{}; a[0] = 3.0f; a[1] = 7.0f;
        a[2] = 1.0f; a[3] = 9.0f;
        auto c = a * 1.0f;
        check(c == a);
    });

    runner.add("vec identity add zero", [] {
        m3::Vec<3, float> a{}; a[0] = 5.0f; a[1] = 3.0f; a[2] = 1.0f;
        auto c = a + 0.0f;
        check(c == a);
    });

    runner.add("vec negate twice is identity", [] {
        m3::Vec<3, float> a{}; a[0] = 1.0f; a[1] = -2.0f; a[2] = 3.0f;
        auto c = -(-a);
        check(c == a);
    });

    runner.add("vec sub self is zero", [] {
        m3::Vec<3, float> a{}; a[0] = 5.0f; a[1] = 5.0f; a[2] = 5.0f;
        auto c = a - a;
        check_float_eq(c[0], 0.0f, 1e-6f);
        check_float_eq(c[1], 0.0f, 1e-6f);
        check_float_eq(c[2], 0.0f, 1e-6f);
    });

    runner.add("vec negative values", [] {
        m3::Vec<3, float> a{}; a[0] = -1.0f; a[1] = -2.0f; a[2] = -3.0f;
        m3::Vec<3, float> b{}; b[0] = -4.0f; b[1] = 1.0f;  b[2] = 2.0f;
        auto c = a + b;
        check_float_eq(c[0], -5.0f, 1e-6f);
        check_float_eq(c[1], -1.0f, 1e-6f);
        check_float_eq(c[2], -1.0f, 1e-6f);
    });

    runner.add("vec int multiply", [] {
        m3::Vec<4, int> a{}; a[0] = 2; a[1] = 3; a[2] = 4; a[3] = 5;
        auto c = a * 3;
        check(c[0] == 6);
        check(c[1] == 9);
        check(c[2] == 12);
        check(c[3] == 15);
    });

    runner.add("vec int division truncation", [] {
        // truncation per C++ integer-division rules
        m3::Vec<2, int> a{}; a[0] = 5; a[1] = 7;
        auto c = a / 2;
        check(c[0] == 2);
        check(c[1] == 3);
    });

    runner.add("vec large values", [] {
        m3::Vec<2, float> a{}; a[0] = 1.0e10f; a[1] = 2.0e10f;
        m3::Vec<2, float> b{}; b[0] = 1.0e10f; b[1] = 1.0e10f;
        auto c = a + b;
        check_float_eq(c[0], 2.0e10f, 1e4f);
        check_float_eq(c[1], 3.0e10f, 1e4f);
    });

    runner.add("vec small values", [] {
        m3::Vec<2, float> a{}; a[0] = 1.0e-10f; a[1] = 2.0e-10f;
        auto c = a * 2.0f;
        check_float_eq(c[0], 2.0e-10f, 1e-16f);
        check_float_eq(c[1], 4.0e-10f, 1e-16f);
    });

    // ====== 综合 ======
    runner.add("vec1 all vector ops", [] {
        m3::Vec<1, float> a{}; a[0] = 3.0f;
        m3::Vec<1, float> b{}; b[0] = 2.0f;
        check_float_eq((a + b)[0], 5.0f, 1e-6f);
        check_float_eq((a - b)[0], 1.0f, 1e-6f);
        check_float_eq((a * b)[0], 6.0f, 1e-6f);
        check_float_eq((a / b)[0], 1.5f, 1e-6f);
    });

    runner.add("vec1 all scalar ops", [] {
        m3::Vec<1, float> a{}; a[0] = 5.0f;
        check_float_eq((a * 2.0f)[0], 10.0f, 1e-6f);
        check_float_eq((3.0f * a)[0], 15.0f, 1e-6f);
        check_float_eq((a + 1.0f)[0], 6.0f, 1e-6f);
        check_float_eq((10.0f - a)[0], 5.0f, 1e-6f);
    });

    runner.add("vec4 all vector ops", [] {
        m3::Vec<4, float> a{}; a[0]=1.0f; a[1]=2.0f; a[2]=3.0f; a[3]=4.0f;
        m3::Vec<4, float> b{}; b[0]=2.0f; b[1]=2.0f; b[2]=2.0f; b[3]=2.0f;
        auto sum = a + b;
        check_float_eq(sum[0], 3.0f, 1e-6f);
        check_float_eq(sum[1], 4.0f, 1e-6f);
        check_float_eq(sum[2], 5.0f, 1e-6f);
        check_float_eq(sum[3], 6.0f, 1e-6f);
        auto neg = -a;
        check_float_eq(neg[3], -4.0f, 1e-6f);
    });

    return runner.run();
}
