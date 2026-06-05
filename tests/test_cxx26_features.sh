#!/bin/bash
# C++26 特性验证脚本（M3Lib）
# 测试：contracts (observe + enforce)、= delete("reason")、structured bindings
#
# C++26 feature verification script for M3Lib
# Tests: contracts (observe + enforce), = delete("reason"), structured bindings
#
# 用法 / Usage: bash tests/test_cxx26_features.sh
# 前提 / Prerequisites: system gcc with -lstdc++exp -lstdc++ available

set -euo pipefail
GCC="gcc -std=c++26 -fcontracts -B/usr/bin"
LDFLAGS="-lstdc++exp -lstdc++"
PASS=0; FAIL=0

CYAN='\033[0;36m'; GREEN='\033[0;32m'; RED='\033[0;31m'; NC='\033[0m'

pass() { echo -e "  ${GREEN}PASS${NC} $1"; PASS=$((PASS+1)); }
fail() { echo -e "  ${RED}FAIL${NC} $1 — $2"; FAIL=$((FAIL+1)); }

echo -e "${CYAN}=== C++26 Contracts P2900R14 — contract_assert ===${NC}"

# 1. Observe 模式：违规记录到 stderr，不终止程序
#    使用 contract_assert(false) —— 安全，无越界访问
#
# 1. Observe mode: violations log to stderr, don't terminate
#    Uses contract_assert(false) — safe, no OOB access
$GCC -fcontract-evaluation-semantic=observe -O2 \
    tests/test_contract_standalone.cpp -o /tmp/test_standalone $LDFLAGS 2>/dev/null \
    && /tmp/test_standalone 2>/tmp/standalone_stderr.txt >/dev/null

# contract_assert 的违规消息包含 "contract violation" 和 "assertion_kind: contract_assert"
# contract_assert violation messages contain "contract violation" and "assertion_kind: contract_assert"
if grep -q "contract violation" /tmp/standalone_stderr.txt; then
    count=$(grep -c "contract violation" /tmp/standalone_stderr.txt)
    pass "observe mode: $count contract_assert violations logged to stderr"
else
    fail "observe mode" "no contract violation on stderr"
fi

# 显示违规消息 / Show the violation messages
echo -e "  → violation messages:"
grep "contract violation\|assertion_kind" /tmp/standalone_stderr.txt | while read line; do
    echo "    $line"
done

# 2. Enforce 模式：违规终止程序
#
# 2. Enforce mode: violations terminate the program
$GCC -fcontract-evaluation-semantic=enforce -O2 \
    tests/test_contract_standalone.cpp -o /tmp/test_enforce $LDFLAGS 2>/dev/null

if /tmp/test_enforce 2>/tmp/enforce_stderr.txt; then
    fail "enforce mode" "should have terminated on contract violation"
else
    if grep -q "terminating: yes" /tmp/enforce_stderr.txt; then
        pass "enforce mode: contract violation terminates program"
    else
        fail "enforce mode" "terminated but no contract message"
    fi
fi

echo ""
echo -e "${CYAN}=== C++26 = delete(\"reason\") P2573R2 ===${NC}"

# 3. 编译期：跨维度构造产生可读的错误消息
#
# 3. Compile-time: cross-dimension construction gives readable error
$GCC -fsyntax-only tests/test_delete_reason.cpp 2>/tmp/delete_reason_err.txt && true
if grep -q "cannot construct from a vector" /tmp/delete_reason_err.txt; then
    pass "delete reason message appears in compiler error"
else
    fail "delete reason" "expected message not found in compiler output"
fi
echo -e "  → error excerpt:"
grep "cannot construct\|deleted function" /tmp/delete_reason_err.txt | head -5 | while read line; do
    echo "    $line"
done

echo ""
echo -e "${CYAN}=== C++26 Structured Bindings P2686R4 ===${NC}"

# 4. 验证 tuple 协议可编译——使用 xmake 构建并运行
#
# 4. Verify tuple protocol compiles — use xmake to build+run
if cd /home/skyler/Dev/M3Lib && xmake f -m release 2>/dev/null && xmake run test_vec 2>/dev/null | grep -q "passed, 0 failed"; then
    pass "structured bindings + tuple protocol (test_vec all passed)"
else
    fail "structured bindings" "test_vec failed"
fi

echo ""
echo -e "${CYAN}========================================${NC}"
echo -e "结果 / Results: ${GREEN}$PASS passed${NC}, ${RED}$FAIL failed${NC}"
exit $FAIL
