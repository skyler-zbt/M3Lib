// 独立测试：contract_assert 在 observe 模式下的行为 (C++26 P2900R14)。
// 使用 observe 语义构建，违规记录到 stderr 但不终止程序。
// 此测试安全——不触发越界访问或任何未定义行为。
//
// 构建与运行：
//   gcc -std=c++26 -fcontracts -fcontract-evaluation-semantic=observe \
//       tests/test_contract_standalone.cpp -o test_standalone \
//       -lstdc++exp -lstdc++ -B/usr/bin
//   ./test_standalone 2>/tmp/stderr.txt
//
// Standalone test for contract_assert in observe mode (C++26 P2900R14).
// Built with observe semantic so violations log to stderr and continue.
// This test is safe — no OOB access or undefined behaviour.

#include <cstdio>

int main() {

    int violations_fired = 0;

    // contract_assert(false) 在 observe 模式下：
    // 1. 调用违规处理函数（默认打印到 stderr）
    // 2. 从 assert 后的下一条语句继续执行
    // 没有 OOB 访问，没有 UB。
    //
    // contract_assert(false) in observe mode:
    // 1. Invokes the violation handler (default: prints to stderr)
    // 2. Continues execution at the next statement
    // No OOB access, no UB.
    contract_assert(false);
    ++violations_fired;

    // 第二次违规——证明可以多次触发
    //
    // Second violation — proves multiple violations are handled
    contract_assert(false);
    ++violations_fired;

    // 第三次违规
    //
    // Third violation
    contract_assert(false);
    ++violations_fired;

    // 到达这里证明 observe 模式在违规后继续执行
    //
    // Reaching here proves observe mode continues after violation
    std::printf("所有 %d 次 contract_assert 违规后程序继续执行（observe 模式）。\n",
                violations_fired);
    std::printf("请检查 stderr 中应有 %d 条契约违规消息。\n", violations_fired);
    return 0;
}
