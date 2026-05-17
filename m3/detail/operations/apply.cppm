export module m3.detail.operations.apply;

import m3.detail.concepts;
import m3.detail.qualifier;
import m3.vector.vec;

export namespace m3::detail {

    template< int L, Arithmetic T, Qualifier Q, BinaryOp<T> Op>
    constexpr Vec<L, T, Q> apply_binary(const Vec<L, T, Q>& a, const Vec<L, T, Q>& b) {
        Vec<L, T, Q> result;
        if constexpr (L == 1) {
            result[0] = Op{}(a[0], b[0]);
        }
        else if constexpr (L == 2) {
            result[0] = Op{}(a[0], b[0]);
            result[1] = Op{}(a[1], b[1]);
        }
        else if constexpr (L == 3) {
            result[0] = Op{}(a[0], b[0]);
            result[1] = Op{}(a[1], b[1]);
            result[2] = Op{}(a[2], b[2]);
        }
        else if constexpr (L == 4) {
            result[0] = Op{}(a[0], b[0]);
            result[1] = Op{}(a[1], b[1]);
            result[2] = Op{}(a[2], b[2]);
            result[3] = Op{}(a[3], b[3]);
        }
        else {
            for (int i = 0; i < L; ++i) {
                result[i] = Op{}(a[i], b[i]);
            }
        }
        return result;
    }

    template<int L, Arithmetic T, Qualifier Q, UnaryOp<T>  Op>
    constexpr Vec<L, T, Q> apply_unary(const Vec<L, T, Q>& a) {
        Vec<L, T, Q> result;
        if constexpr (L == 1) {
            result[0] = Op{}(a[0]);
        }
        else if constexpr (L == 2) {
            result[0] = Op{}(a[0]);
            result[1] = Op{}(a[1]);
        }
        else if constexpr (L == 3) {
            result[0] = Op{}(a[0]);
            result[1] = Op{}(a[1]);
            result[2] = Op{}(a[2]);
        }
        else if constexpr (L == 4) {
            result[0] = Op{}(a[0]);
            result[1] = Op{}(a[1]);
            result[2] = Op{}(a[2]);
            result[3] = Op{}(a[3]);
        }
        else {
            for (int i = 0; i < L; ++i) {
                result[i] = Op{}(a[i]);
            }
        }
        return result;
    }

}
