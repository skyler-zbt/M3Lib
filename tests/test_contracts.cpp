// M3Lib — Contract observe mode verification tests.
// Built with -fcontract-evaluation-semantic=observe so violations log to stderr
// without terminating.  These tests NEVER trigger undefined behaviour:
// contract_assert(false) is used to verify the observe semantic in a safe
// context where no OOB memory access follows the violation.
//    xmake build test_contracts && xmake run test_contracts
//    mcpp build -p test_contracts && ./target/*/bin/test_contracts
//
// M3Lib —— 契约 observe 模式验证测试。
// 使用 -fcontract-evaluation-semantic=observe 构建，违规记录到 stderr 但不终止。
// 这些测试绝不会触发未定义行为：使用 contract_assert(false) 在安全上下文中
// 验证 observe 语义，违规后不跟随任何越界内存访问。

import std;
import m3;

// === test infrastructure ===

import test_common;

// === helper: trigger a contract violation without any OOB access ===
//
// === 辅助函数：触发契约违规但不产生越界访问 ===

static void fire_contract_violation() {
    // contract_assert(false) always fires in observe/enforce mode.
    // In observe mode the handler runs (logs to stderr), then execution
    // continues at the next statement — no UB because there is no OOB
    // memory access following the assertion.
    //
    // contract_assert(false) 在 observe/enforce 模式下始终触发。
    // observe 模式下处理函数运行（记录到 stderr），然后在下一个语句继续执行——
    // 没有 UB，因为断言后不跟随越界内存访问。
    contract_assert(false);
}

// === test cases ===

int main() {
    TestRunner runner;

    // Valid boundary indices — no contract violation expected
    //
    // 合法边界索引——预期无契约违规
    runner.add("operator[] lower bound (index 0)", [] -> TestResult {
        m3::Vec<4, int> v{0};
        v[0] = 42;
        if (auto r = check(v[0] == 42); !r)
            return r;
        return {};
    });

    runner.add("operator[] upper bound (index L-1)", [] -> TestResult {
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

    // Observe mode: contract_assert(false) logs to stderr, does not terminate.
    // This is a safe test because no OOB memory access follows the assertion.
    //
    // Observe 模式：contract_assert(false) 记录到 stderr，不终止程序。
    // 这是安全的测试，因为断言后不跟随越界内存访问。

    runner.add("contract_assert violation survives observe", [] -> TestResult {
        fire_contract_violation();  // logged, execution continues
        return {};                  // reaching here proves observe mode works
    });

    runner.add("multiple contract_assert violations survive", [] -> TestResult {
        fire_contract_violation();
        fire_contract_violation();
        fire_contract_violation();
        return {};
    });

    // Valid access after contract_assert violation — proves the program
    // remains fully functional after a violation in observe mode.
    //
    // 契约违规后的合法访问——证明 observe 模式下违规后程序保持完全功能。
    runner.add("vec usable after contract_assert violation", [] -> TestResult {
        m3::Vec<3, float> v{1.0f, 2.0f, 3.0f};

        fire_contract_violation();  // unrelated violation, no effect on vec

        // Vec is unchanged and fully usable
        //
        // Vec 未改变且完全可用
        if (auto r = check_float_eq(v[0], 1.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(v[1], 2.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(v[2], 3.0f, 1e-6f); !r)
            return r;
        auto sum = v + v;
        if (auto r = check_float_eq(sum[0], 2.0f, 1e-6f); !r)
            return r;
        return {};
    });

    return runner.run();
}
