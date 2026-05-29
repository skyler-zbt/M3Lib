// M3Lib — Vec test suite
//    xmake build tests && xmake run test_vec
//    mcpp build -p test_vec && ./target/*/bin/test_vec

import std;
import m3;

// ---- test infrastructure ----

using TestResult = std::expected<void, std::string>;

namespace {

// Use C++20 <source_location> for file/line diagnostics — avoids legacy macros
inline TestResult check(
    bool expr,
    std::source_location loc = std::source_location::current())
{
    if (!expr) {
        return std::unexpected(
            std::string{loc.file_name()} + ":" + std::to_string(loc.line()) + ": check failed");
    }
    return {};
}

template<typename T>
TestResult check_float_eq(
    T a, T b, T eps,
    std::source_location loc = std::source_location::current())
{
    if (std::fabs(static_cast<double>(a) - static_cast<double>(b))
        > static_cast<double>(eps)) {
        return std::unexpected(
            std::string{loc.file_name()} + ":" + std::to_string(loc.line())
            + ": |" + std::to_string(a) + " - " + std::to_string(b)
            + "| > " + std::to_string(eps));
    }
    return {};
}

// Test runner
class TestRunner {
public:
    using TestFunc = std::function<TestResult()>;

    void add(std::string_view name, TestFunc func) {
        tests_.push_back({std::string{name}, std::move(func)});
    }

    int run() {
        int passed = 0;
        int failed = 0;

        for (auto& [name, func] : tests_) {
            std::printf("  %-48s", name.c_str());
            auto result = func();
            if (result) {
                std::printf("PASSED\n");
                ++passed;
            } else {
                std::printf("FAILED\n");
                std::cerr << "    " << result.error() << '\n';
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


// ---- Test cases ----

int main()
{
    TestRunner runner;

    // ====== Construction ======
    runner.add("vec default construct", [] -> TestResult {
        m3::Vec<3, float> v{};
        if (auto r = check(v[0] == 0.0f); !r) return r;
        if (auto r = check(v[1] == 0.0f); !r) return r;
        if (auto r = check(v[2] == 0.0f); !r) return r;
        return {};

    });

    runner.add("vec scalar construct", [] -> TestResult {
        m3::Vec<3, float> v{2.5f};
        if (auto r = check_float_eq(v[0], 2.5f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(v[1], 2.5f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(v[2], 2.5f, 1e-6f); !r) return r;
        return {};

    });

    runner.add("vec copy construct", [] -> TestResult {
        m3::Vec<3, float> a{3.0f};
        m3::Vec<3, float> b = a;
        if (auto r = check_float_eq(b[0], 3.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(b[1], 3.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(b[2], 3.0f, 1e-6f); !r) return r;
        return {};

    });

    runner.add("vec int default zero", [] -> TestResult {
        m3::Vec<4, int> v{};
        if (auto r = check(v[0] == 0); !r) return r;
        if (auto r = check(v[1] == 0); !r) return r;
        if (auto r = check(v[2] == 0); !r) return r;
        if (auto r = check(v[3] == 0); !r) return r;
        return {};

    });

    runner.add("vec double construct", [] -> TestResult {
        m3::Vec<2, double> v{1.5};
        if (auto r = check_float_eq(v[0], 1.5, 1e-12); !r) return r;
        if (auto r = check_float_eq(v[1], 1.5, 1e-12); !r) return r;
        return {};

    });

    // ====== Vec1 accessors ======
    runner.add("vec1 accessors xyz", [] -> TestResult {
        m3::Vec<1, float> v{};
        v.x() = 7.0f;
        if (auto r = check_float_eq(v.x(), 7.0f, 1e-6f); !r) return r;
        return {};

    });

    runner.add("vec1 accessors rgb share memory with xyz", [] -> TestResult {
        m3::Vec<1, float> v{};
        v.r() = 0.5f;
        if (auto r = check_float_eq(v.x(), 0.5f, 1e-6f); !r) return r;
        return {};

    });

    runner.add("vec1 accessors stp", [] -> TestResult {
        m3::Vec<1, float> v{};
        v.s() = 3.0f;
        if (auto r = check_float_eq(v.s(), 3.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(v[0], 3.0f, 1e-6f); !r) return r;
        return {};

    });

    runner.add("vec1 const accessor", [] -> TestResult {
        const m3::Vec<1, float> v{42.0f};
        if (auto r = check_float_eq(v.x(), 42.0f, 1e-6f); !r) return r;
        return {};

    });

    // ====== Vec2 accessors ======
    runner.add("vec2 xy accessors", [] -> TestResult {
        m3::Vec<2, float> v{};
        v.x() = 1.0f; v.y() = 2.0f;
        if (auto r = check_float_eq(v.x(), 1.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(v.y(), 2.0f, 1e-6f); !r) return r;
        return {};

    });

    runner.add("vec2 rg accessors alias xy", [] -> TestResult {
        m3::Vec<2, float> v{};
        v.r() = 0.2f; v.g() = 0.8f;
        if (auto r = check_float_eq(v.x(), 0.2f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(v.y(), 0.8f, 1e-6f); !r) return r;
        return {};

    });

    runner.add("vec2 st accessors", [] -> TestResult {
        m3::Vec<2, float> v{};
        v.s() = 0.1f; v.t() = 0.9f;
        if (auto r = check_float_eq(v[0], 0.1f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(v[1], 0.9f, 1e-6f); !r) return r;
        return {};

    });

    // ====== Vec3 accessors ======
    runner.add("vec3 xyz accessors", [] -> TestResult {
        m3::Vec<3, float> v{};
        v.x() = 1.0f; v.y() = 2.0f; v.z() = 3.0f;
        if (auto r = check_float_eq(v.x(), 1.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(v.y(), 2.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(v.z(), 3.0f, 1e-6f); !r) return r;
        return {};

    });

    runner.add("vec3 rgb accessors alias xyz", [] -> TestResult {
        m3::Vec<3, float> v{};
        v.r() = 1.0f; v.g() = 0.5f; v.b() = 0.0f;
        if (auto r = check_float_eq(v[0], 1.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(v[1], 0.5f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(v[2], 0.0f, 1e-6f); !r) return r;
        return {};

    });

    runner.add("vec3 stp accessors", [] -> TestResult {
        m3::Vec<3, float> v{};
        v.s() = 0.1f; v.t() = 0.2f; v.p() = 0.3f;
        if (auto r = check_float_eq(v[0], 0.1f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(v[1], 0.2f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(v[2], 0.3f, 1e-6f); !r) return r;
        return {};

    });

    // ====== Vec4 accessors ======
    runner.add("vec4 xyzw accessors", [] -> TestResult {
        m3::Vec<4, float> v{};
        v.x() = 1.0f; v.y() = 2.0f; v.z() = 3.0f; v.w() = 4.0f;
        if (auto r = check_float_eq(v.x(), 1.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(v.y(), 2.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(v.z(), 3.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(v.w(), 4.0f, 1e-6f); !r) return r;
        return {};

    });

    runner.add("vec4 rgba accessors alias xyzw", [] -> TestResult {
        m3::Vec<4, float> v{};
        v.r() = 1.0f; v.g() = 0.5f; v.b() = 0.0f; v.a() = 1.0f;
        if (auto r = check_float_eq(v[0], 1.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(v[3], 1.0f, 1e-6f); !r) return r;
        return {};

    });

    runner.add("vec4 stpq accessors", [] -> TestResult {
        m3::Vec<4, float> v{};
        v.s() = 0.1f; v.t() = 0.2f; v.p() = 0.3f; v.q() = 0.4f;
        if (auto r = check_float_eq(v[3], 0.4f, 1e-6f); !r) return r;
        return {};

    });

    // ====== operator[] / value_ptr ======
    runner.add("vec operator[] read", [] -> TestResult {
        m3::Vec<3, float> v{1.0f};
        if (auto r = check_float_eq(v[0], 1.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(v[1], 1.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(v[2], 1.0f, 1e-6f); !r) return r;
        return {};

    });

    runner.add("vec operator[] write", [] -> TestResult {
        m3::Vec<3, float> v{};
        v[0] = 10.0f; v[1] = 20.0f; v[2] = 30.0f;
        if (auto r = check_float_eq(v[0], 10.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(v[1], 20.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(v[2], 30.0f, 1e-6f); !r) return r;
        return {};

    });

    runner.add("vec const operator[]", [] -> TestResult {
        const m3::Vec<2, int> v{5};
        if (auto r = check(v[0] == 5); !r) return r;
        if (auto r = check(v[1] == 5); !r) return r;
        return {};

    });

    runner.add("vec value_ptr", [] -> TestResult {
        m3::Vec<3, float> v{};
        v[0] = 1.0f; v[1] = 2.0f; v[2] = 3.0f;
        const float* ptr = v.value_ptr();
        if (auto r = check(ptr != nullptr); !r) return r;
        if (auto r = check_float_eq(ptr[0], 1.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(ptr[1], 2.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(ptr[2], 3.0f, 1e-6f); !r) return r;
        return {};

    });

    runner.add("vec value_ptr const", [] -> TestResult {
        const m3::Vec<2, float> v{4.0f};
        const float* ptr = v.value_ptr();
        if (auto r = check(ptr != nullptr); !r) return r;
        if (auto r = check_float_eq(ptr[0], 4.0f, 1e-6f); !r) return r;
        return {};

    });

    // ====== Vector-vector operations ======
    runner.add("vec add", [] -> TestResult {
        m3::Vec<3, float> a{}; a[0] = 1.0f; a[1] = 2.0f; a[2] = 3.0f;
        m3::Vec<3, float> b{}; b[0] = 4.0f; b[1] = 5.0f; b[2] = 6.0f;
        auto c = a + b;
        if (auto r = check_float_eq(c[0], 5.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(c[1], 7.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(c[2], 9.0f, 1e-6f); !r) return r;
        return {};

    });

    runner.add("vec sub", [] -> TestResult {
        m3::Vec<3, float> a{}; a[0] = 10.0f; a[1] = 8.0f; a[2] = 6.0f;
        m3::Vec<3, float> b{}; b[0] = 1.0f;  b[1] = 2.0f;  b[2] = 3.0f;
        auto c = a - b;
        if (auto r = check_float_eq(c[0], 9.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(c[1], 6.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(c[2], 3.0f, 1e-6f); !r) return r;
        return {};

    });

    runner.add("vec mul hadamard", [] -> TestResult {
        m3::Vec<2, float> a{}; a[0] = 2.0f; a[1] = 3.0f;
        m3::Vec<2, float> b{}; b[0] = 4.0f; b[1] = 5.0f;
        auto c = a * b;
        if (auto r = check_float_eq(c[0], 8.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(c[1], 15.0f, 1e-6f); !r) return r;
        return {};

    });

    runner.add("vec div", [] -> TestResult {
        m3::Vec<2, float> a{}; a[0] = 8.0f; a[1] = 12.0f;
        m3::Vec<2, float> b{}; b[0] = 2.0f; b[1] = 3.0f;
        auto c = a / b;
        if (auto r = check_float_eq(c[0], 4.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(c[1], 4.0f, 1e-6f); !r) return r;
        return {};

    });

    runner.add("vec negate", [] -> TestResult {
        m3::Vec<3, float> a{}; a[0] = 1.0f; a[1] = -2.0f; a[2] = 0.0f;
        auto c = -a;
        if (auto r = check_float_eq(c[0], -1.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(c[1], 2.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(c[2], 0.0f, 1e-6f); !r) return r;
        return {};

    });

    runner.add("vec negate zero", [] -> TestResult {
        m3::Vec<4, int> a{0};
        auto c = -a;
        if (auto r = check(c[0] == 0 && c[1] == 0 && c[2] == 0 && c[3] == 0); !r) return r;
        return {};

    });

    runner.add("vec negate int", [] -> TestResult {
        m3::Vec<2, int> a{}; a[0] = 5; a[1] = -3;
        auto c = -a;
        if (auto r = check(c[0] == -5); !r) return r;
        if (auto r = check(c[1] == 3); !r) return r;
        return {};

    });

    // ====== Vector-scalar operations ======
    runner.add("vec mul scalar right", [] -> TestResult {
        m3::Vec<3, float> a{}; a[0] = 1.0f; a[1] = 2.0f; a[2] = 3.0f;
        auto c = a * 2.0f;
        if (auto r = check_float_eq(c[0], 2.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(c[1], 4.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(c[2], 6.0f, 1e-6f); !r) return r;
        return {};

    });

    runner.add("vec mul scalar left", [] -> TestResult {
        m3::Vec<3, float> a{}; a[0] = 1.0f; a[1] = 2.0f; a[2] = 3.0f;
        auto c = 2.0f * a;
        if (auto r = check_float_eq(c[0], 2.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(c[1], 4.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(c[2], 6.0f, 1e-6f); !r) return r;
        return {};

    });

    runner.add("vec div scalar", [] -> TestResult {
        m3::Vec<2, float> a{}; a[0] = 10.0f; a[1] = 20.0f;
        auto c = a / 2.0f;
        if (auto r = check_float_eq(c[0], 5.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(c[1], 10.0f, 1e-6f); !r) return r;
        return {};

    });

    runner.add("vec add scalar", [] -> TestResult {
        m3::Vec<3, float> a{}; a[0] = 1.0f; a[1] = 2.0f; a[2] = 3.0f;
        auto c = a + 10.0f;
        if (auto r = check_float_eq(c[0], 11.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(c[1], 12.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(c[2], 13.0f, 1e-6f); !r) return r;
        return {};

    });

    runner.add("vec scalar add", [] -> TestResult {
        m3::Vec<3, float> a{}; a[0] = 1.0f; a[1] = 2.0f; a[2] = 3.0f;
        auto c = 10.0f + a;
        if (auto r = check_float_eq(c[0], 11.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(c[1], 12.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(c[2], 13.0f, 1e-6f); !r) return r;
        return {};

    });

    runner.add("vec sub scalar", [] -> TestResult {
        m3::Vec<2, float> a{}; a[0] = 10.0f; a[1] = 20.0f;
        auto c = a - 3.0f;
        if (auto r = check_float_eq(c[0], 7.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(c[1], 17.0f, 1e-6f); !r) return r;
        return {};

    });

    runner.add("vec scalar sub", [] -> TestResult {
        m3::Vec<2, float> a{}; a[0] = 3.0f; a[1] = 5.0f;
        auto c = 10.0f - a;
        if (auto r = check_float_eq(c[0], 7.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(c[1], 5.0f, 1e-6f); !r) return r;
        return {};

    });

    // ====== Comparison operators ======
    runner.add("vec equal", [] -> TestResult {
        m3::Vec<3, float> a{}; a[0] = 1.0f; a[1] = 2.0f; a[2] = 3.0f;
        m3::Vec<3, float> b{}; b[0] = 1.0f; b[1] = 2.0f; b[2] = 3.0f;
        if (auto r = check(a == b); !r) return r;
        return {};

    });

    runner.add("vec not equal", [] -> TestResult {
        m3::Vec<3, float> a{}; a[0] = 1.0f; a[1] = 2.0f; a[2] = 3.0f;
        m3::Vec<3, float> b{}; b[0] = 1.0f; b[1] = 2.0f; b[2] = 4.0f;
        if (auto r = check(a != b); !r) return r;
        if (auto r = check(!(a == b)); !r) return r;
        return {};

    });

    runner.add("vec equal different", [] -> TestResult {
        m3::Vec<4, int> a{1};
        m3::Vec<4, int> b{}; b[0] = 1; b[1] = 1; b[2] = 1; b[3] = 0;
        if (auto r = check(a != b); !r) return r;
        return {};

    });

    runner.add("vec equal zero", [] -> TestResult {
        m3::Vec<2, float> a{}; a[0] = 0.0f; a[1] = 0.0f;
        m3::Vec<2, float> b{}; b[0] = 0.0f; b[1] = 0.0f;
        if (auto r = check(a == b); !r) return r;
        return {};

    });

    // ====== Edge cases ======
    runner.add("vec zero add zero", [] -> TestResult {
        m3::Vec<3, float> a{};
        m3::Vec<3, float> b{};
        auto c = a + b;
        if (auto r = check_float_eq(c[0], 0.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(c[1], 0.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(c[2], 0.0f, 1e-6f); !r) return r;
        return {};

    });

    runner.add("vec identity mul one", [] -> TestResult {
        m3::Vec<4, float> a{}; a[0] = 3.0f; a[1] = 7.0f;
        a[2] = 1.0f; a[3] = 9.0f;
        auto c = a * 1.0f;
        if (auto r = check(c == a); !r) return r;
        return {};

    });

    runner.add("vec identity add zero", [] -> TestResult {
        m3::Vec<3, float> a{}; a[0] = 5.0f; a[1] = 3.0f; a[2] = 1.0f;
        auto c = a + 0.0f;
        if (auto r = check(c == a); !r) return r;
        return {};

    });

    runner.add("vec negate twice is identity", [] -> TestResult {
        m3::Vec<3, float> a{}; a[0] = 1.0f; a[1] = -2.0f; a[2] = 3.0f;
        auto c = -(-a);
        if (auto r = check(c == a); !r) return r;
        return {};

    });

    runner.add("vec sub self is zero", [] -> TestResult {
        m3::Vec<3, float> a{}; a[0] = 5.0f; a[1] = 5.0f; a[2] = 5.0f;
        auto c = a - a;
        if (auto r = check_float_eq(c[0], 0.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(c[1], 0.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(c[2], 0.0f, 1e-6f); !r) return r;
        return {};

    });

    runner.add("vec negative values", [] -> TestResult {
        m3::Vec<3, float> a{}; a[0] = -1.0f; a[1] = -2.0f; a[2] = -3.0f;
        m3::Vec<3, float> b{}; b[0] = -4.0f; b[1] = 1.0f;  b[2] = 2.0f;
        auto c = a + b;
        if (auto r = check_float_eq(c[0], -5.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(c[1], -1.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(c[2], -1.0f, 1e-6f); !r) return r;
        return {};

    });

    runner.add("vec int multiply", [] -> TestResult {
        m3::Vec<4, int> a{}; a[0] = 2; a[1] = 3; a[2] = 4; a[3] = 5;
        auto c = a * 3;
        if (auto r = check(c[0] == 6); !r) return r;
        if (auto r = check(c[1] == 9); !r) return r;
        if (auto r = check(c[2] == 12); !r) return r;
        if (auto r = check(c[3] == 15); !r) return r;
        return {};

    });

    runner.add("vec int division truncation", [] -> TestResult {
        // truncation per C++ integer-division rules
        m3::Vec<2, int> a{}; a[0] = 5; a[1] = 7;
        auto c = a / 2;
        if (auto r = check(c[0] == 2); !r) return r;
        if (auto r = check(c[1] == 3); !r) return r;
        return {};

    });

    runner.add("vec large values", [] -> TestResult {
        m3::Vec<2, float> a{}; a[0] = 1.0e10f; a[1] = 2.0e10f;
        m3::Vec<2, float> b{}; b[0] = 1.0e10f; b[1] = 1.0e10f;
        auto c = a + b;
        if (auto r = check_float_eq(c[0], 2.0e10f, 1e4f); !r) return r;
        if (auto r = check_float_eq(c[1], 3.0e10f, 1e4f); !r) return r;
        return {};

    });

    runner.add("vec small values", [] -> TestResult {
        m3::Vec<2, float> a{}; a[0] = 1.0e-10f; a[1] = 2.0e-10f;
        auto c = a * 2.0f;
        if (auto r = check_float_eq(c[0], 2.0e-10f, 1e-16f); !r) return r;
        if (auto r = check_float_eq(c[1], 4.0e-10f, 1e-16f); !r) return r;
        return {};

    });

    // ====== Integration ======
    runner.add("vec1 all vector ops", [] -> TestResult {
        m3::Vec<1, float> a{}; a[0] = 3.0f;
        m3::Vec<1, float> b{}; b[0] = 2.0f;
        if (auto r = check_float_eq((a + b)[0], 5.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq((a - b)[0], 1.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq((a * b)[0], 6.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq((a / b)[0], 1.5f, 1e-6f); !r) return r;
        return {};

    });

    runner.add("vec1 all scalar ops", [] -> TestResult {
        m3::Vec<1, float> a{}; a[0] = 5.0f;
        if (auto r = check_float_eq((a * 2.0f)[0], 10.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq((3.0f * a)[0], 15.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq((a + 1.0f)[0], 6.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq((10.0f - a)[0], 5.0f, 1e-6f); !r) return r;
        return {};

    });

    runner.add("vec4 all vector ops", [] -> TestResult {
        m3::Vec<4, float> a{}; a[0]=1.0f; a[1]=2.0f; a[2]=3.0f; a[3]=4.0f;
        m3::Vec<4, float> b{}; b[0]=2.0f; b[1]=2.0f; b[2]=2.0f; b[3]=2.0f;
        auto sum = a + b;
        if (auto r = check_float_eq(sum[0], 3.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(sum[1], 4.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(sum[2], 5.0f, 1e-6f); !r) return r;
        if (auto r = check_float_eq(sum[3], 6.0f, 1e-6f); !r) return r;
        auto neg = -a;
        if (auto r = check_float_eq(neg[3], -4.0f, 1e-6f); !r) return r;
        return {};

    });

    return runner.run();
}
