// Shared test infrastructure for M3Lib.
// Provides check / check_float_eq helpers and a TestRunner harness.
// Import this module instead of duplicating the boilerplate in each test target.
//
// M3Lib 共享测试基础设施。
// 提供 check / check_float_eq 辅助函数和 TestRunner 框架。
// 导入此模块即可，无需在每个测试目标中重复样板代码。
export module test_common;

import std;

export {
    using TestResult = std::expected<void, std::string>;

    inline TestResult check(bool expr, std::source_location loc = std::source_location::current()) {
        if (!expr) {
            return std::unexpected(std::string{loc.file_name()} + ":" + std::to_string(loc.line()) +
                                   ": check failed");
        }
        return {};
    }

    template <typename T>
    inline TestResult check_float_eq(T a, T b, T eps,
                                     std::source_location loc = std::source_location::current()) {
        if (std::fabs(static_cast<double>(a) - static_cast<double>(b)) > static_cast<double>(eps)) {
            return std::unexpected(std::string{loc.file_name()} + ":" + std::to_string(loc.line()) +
                                   ": |" + std::to_string(a) + " - " + std::to_string(b) + "| > " +
                                   std::to_string(eps));
        }
        return {};
    }

    class TestRunner {
    public:
        using TestFunc = std::function<TestResult()>;
        void add(std::string_view name, TestFunc func) {
            tests_.push_back({std::string{name}, std::move(func)});
        }
        int run() {
            int passed = 0, failed = 0;
            for (auto& [name, func] : tests_) {
                std::printf("  %-50s", name.c_str());
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
            std::printf("\n==========================\n%d passed, %d failed\n", passed, failed);
            return failed > 0 ? 1 : 0;
        }
    private:
        struct Entry {
            std::string name;
            TestFunc func;
        };
        std::vector<Entry> tests_;
    };
}
