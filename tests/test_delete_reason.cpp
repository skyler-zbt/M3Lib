// Standalone compile-time test for = delete("reason") (P2573R2).
// This file should FAIL to compile with a message containing
// "cannot construct from a vector of different dimension".
//
// Test:  ! gcc -std=c++26 -c tests/test_delete_reason.cpp 2>&1 \
//           | grep -q "cannot construct.*different dimension" \
//        && echo "PASS" || echo "FAIL"

#include <array>

template <int L, typename T>
struct VecBase {
    std::array<T, L> data_{};
    VecBase() = default;
    VecBase(const VecBase&) = default;

    template <int M>
    requires(M != L)
    VecBase(const VecBase<M, T>&) =
        delete ("cannot construct from a vector of different dimension; "
                "use explicit element-wise conversion instead");

    constexpr T& operator[](std::size_t i) {
        return data_[i];
    }
};

template <int L, typename T>
struct Vec : VecBase<L, T> {
    Vec() = default;
    using VecBase<L, T>::VecBase;
};

int main() {
    Vec<2, float> v2;
    v2[0] = 1.0f;
    v2[1] = 2.0f;

    // This line should fail to compile with the delete reason message
    Vec<3, float> bad{v2};  // COMPILE ERROR EXPECTED
}
