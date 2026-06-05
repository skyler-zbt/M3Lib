// M3Lib — Boundary tests: out-of-bounds access verification.
// Built in enforce mode: every OOB access must trigger std::abort().
// Each test forks a child process; the parent asserts the child was
// terminated by a signal (SIGABRT), confirming the defense-in-depth
// bounds check in VectorBase::operator[] is active.
//
// M3Lib — 边界测试：越界访问验证。
// 在 enforce 模式下构建：每次 OOB 访问必须触发 std::abort()。
// 每个测试 fork 子进程；父进程断言子进程被信号终止（SIGABRT），
// 确认 VectorBase::operator[] 的纵深防御边界检查已生效。

import std;

// POSIX process control — declared manually to avoid header conflicts
// with C++ modules.
//
// POSIX 进程控制——手动声明以避免与 C++ modules 的头文件冲突。
extern "C" {
int fork();
int waitpid(int pid, int* status, int options);
}

constexpr int signal_abort = 6;  // SIGABRT

constexpr bool was_signalled(int status) {
    return (status & 0x7f) != 0;
}
constexpr int term_signal(int status) {
    return status & 0x7f;
}

import m3;
import test_common;

// === helper: run fn in a child process and check if it aborted ===
//
// === 辅助函数：在子进程中运行 fn，检查是否 abort ===

static bool expect_abort(auto fn) {
    int pid = fork();
    if (pid == 0) {
        fn();
        std::_Exit(0);
    }
    int status = 0;
    waitpid(pid, &status, 0);
    return was_signalled(status) && (term_signal(status) == signal_abort);
}

// === test cases ===

int main() {
    TestRunner runner;

    // ---- operator[] OOB write tests ----
    //
    // ---- operator[] 越界写入测试 ----

    runner.add("Vec<1>[1] OOB write aborts", [] -> TestResult {
        if (!expect_abort([] {
                m3::Vec<1, int> v{};
                v[1] = 0;
            }))
            return std::unexpected("no abort on Vec<1>[1] write");
        return {};
    });

    runner.add("Vec<2>[2] OOB write aborts", [] -> TestResult {
        if (!expect_abort([] {
                m3::Vec<2, int> v{};
                v[2] = 0;
            }))
            return std::unexpected("no abort on Vec<2>[2] write");
        return {};
    });

    runner.add("Vec<3>[3] OOB write aborts", [] -> TestResult {
        if (!expect_abort([] {
                m3::Vec<3, int> v{};
                v[3] = 0;
            }))
            return std::unexpected("no abort on Vec<3>[3] write");
        return {};
    });

    runner.add("Vec<4>[4] OOB write aborts", [] -> TestResult {
        if (!expect_abort([] {
                m3::Vec<4, int> v{};
                v[4] = 0;
            }))
            return std::unexpected("no abort on Vec<4>[4] write");
        return {};
    });

    runner.add("Vec<4> large OOB index aborts", [] -> TestResult {
        if (!expect_abort([] {
                m3::Vec<4, float> v{};
                v[999] = 0.f;
            }))
            return std::unexpected("no abort on large OOB write");
        return {};
    });

    // ---- operator[] OOB read tests ----
    //
    // ---- operator[] 越界读取测试 ----

    runner.add("Vec<3>[3] OOB read aborts", [] -> TestResult {
        if (!expect_abort([] {
                const m3::Vec<3, float> v{1.f, 2.f, 3.f};
                auto x = v[3];
            }))
            return std::unexpected("no abort on OOB read");
        return {};
    });

    runner.add("Vec<2> far OOB read aborts", [] -> TestResult {
        if (!expect_abort([] {
                const m3::Vec<2, int> v{0};
                auto x = v[100];
            }))
            return std::unexpected("no abort on far OOB read");
        return {};
    });

    // ---- Valid access at boundaries (regression tests) ----
    //
    // ---- 合法边界访问（回归测试） ----

    runner.add("Vec<1>[0] valid access passes", [] -> TestResult {
        m3::Vec<1, int> v{42};
        if (auto r = check(v[0] == 42); !r)
            return r;
        return {};
    });

    runner.add("Vec<4>[0] and [3] valid access passes", [] -> TestResult {
        m3::Vec<4, int> v{1, 2, 3, 4};
        if (auto r = check(v[0] == 1); !r)
            return r;
        if (auto r = check(v[3] == 4); !r)
            return r;
        return {};
    });

    // ---- Contract mode self-check ----
    //
    // ---- 契约模式自检 ----

    runner.add("contracts are active (self-check)", [] -> TestResult {
        contract_assert(true);  // always passes, verifies contracts compile
        return {};
    });

    // ---- Integer functor safety via ops chain ----
    // Div pre(b != 0) and Neg INT_MIN check.
    //
    // ---- 通过运算链验证整数 functor 安全性 ----
    // Div pre(b != 0) 和 Neg INT_MIN 检查。

    runner.add("Vec<2> int div-by-zero aborts", [] -> TestResult {
        if (!expect_abort([] {
                m3::Vec<2, int> v{10, 20};
                auto r = v / 0;
            }))
            return std::unexpected("no abort on int div by zero");
        return {};
    });

    runner.add("Neg of INT_MIN aborts", [] -> TestResult {
        if (!expect_abort([] {
                auto x = std::numeric_limits<int>::lowest();
                m3::Vec<1, int> v{x};
                auto r = -v;
            }))
            return std::unexpected("no abort on negating INT_MIN");
        return {};
    });

    // ---- Clamp vector pre-condition (minVal <= maxVal) ----
    //
    // ---- clamp 向量 pre 条件 (minVal <= maxVal) ----

    runner.add("clamp invalid range aborts", [] -> TestResult {
        if (!expect_abort([] {
                m3::Vec<3, int> v{1, 5, 10};
                auto r = m3::clamp(v, 10, 0);  // minVal > maxVal
            }))
            return std::unexpected("no abort on clamp invalid range");
        return {};
    });

    return runner.run();
}
