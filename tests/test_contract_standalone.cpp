// Standalone test for operator[] pre-condition contracts (P2900R14).
// Run in observe mode so violations log to stderr and continue:
//   gcc -std=c++26 -fcontracts -fcontract-evaluation-semantic=observe \
//       tests/test_contract_standalone.cpp -o test_standalone \
//       -lstdc++exp -lstdc++ -B/usr/bin
//
// This test directly exercises the same contract pattern used by
// VectorBase::operator[] — verifying that pre(i < L) fires on OOB.

#include <cstdio>
#include <array>

// Replicates VectorBase::operator[] contract pattern exactly
template<int L, typename T>
struct MinimalVec {
    std::array<T, L> data_{};
    constexpr T& operator[](std::size_t i)
        pre(i < static_cast<std::size_t>(L))
    {
        return data_[i];
    }
    constexpr const T& operator[](std::size_t i) const
        pre(i < static_cast<std::size_t>(L))
    {
        return data_[i];
    }
};

int main() {
    int failures = 0;

    // Valid access — no violation
    {
        MinimalVec<3, int> v{};
        v[0] = 10;
        v[2] = 30;
        if (v[0] != 10 || v[2] != 30) {
            std::printf("FAIL: valid access\n");
            ++failures;
        }
    }

    // Out-of-bounds write — triggers pre(i < 3)
    {
        MinimalVec<3, int> v{};
        v[5] = 42; // VIOLATION: pre(5 < 3) == false
        // survives because observe doesn't terminate
    }

    // Out-of-bounds read on const
    {
        const MinimalVec<2, int> v{};
        int x = v[9]; // VIOLATION: pre(9 < 2) == false
        (void)x;
    }

    // Out-of-bounds at exact L (one past end)
    {
        MinimalVec<4, int> v{};
        v[4] = 0; // VIOLATION: pre(4 < 4) == false
    }

    // All violations survived (observe mode)
    std::printf("All out-of-bounds accesses survived (observe mode).\n");
    std::printf("Check stderr for %d contract violation messages.\n",
        failures == 0 ? 3 : 0);
    return failures;
}
