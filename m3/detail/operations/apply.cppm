export module m3.detail.operations.apply;

import m3.detail;
import m3.vector.vec;

export namespace m3::detail {

    template<typename Op, int L, Arithmetic T, Qualifier Q>
    constexpr Vec<L, T, Q> apply_binary(const Vec<L, T, Q>& a, const Vec<L, T, Q>& b) {
        Vec<L, T, Q> result;
        Op op;
        for (int i = 0; i < L; ++i)
            result[i] = op(a[i], b[i]);
        return result;
    }

    template<typename Op, int L, Arithmetic T, Qualifier Q>
    constexpr Vec<L, T, Q> apply_unary(const Vec<L, T, Q>& a) {
        Vec<L, T, Q> result;
        Op op;
        for (int i = 0; i < L; ++i)
            result[i] = op(a[i]);
        return result;
    }

}
