
export module m3.detail.operations.apply;

import std;

import m3.detail.concepts;

export namespace m3::detail {

template <typename Op, VectorLike V>
requires BinaryOp<Op, typename V::value_type>
[[nodiscard("element-wise operation: discarding the result is likely a bug")]]
constexpr V apply_binary(const V& a, const V& b) {
    V result;
    constexpr int L = V::dimension;
    if constexpr (L == 1) {
        result[0] = Op{}(a[0], b[0]);
    } else if constexpr (L == 2) {
        result[0] = Op{}(a[0], b[0]);
        result[1] = Op{}(a[1], b[1]);
    } else if constexpr (L == 3) {
        result[0] = Op{}(a[0], b[0]);
        result[1] = Op{}(a[1], b[1]);
        result[2] = Op{}(a[2], b[2]);
    } else if constexpr (L == 4) {
        result[0] = Op{}(a[0], b[0]);
        result[1] = Op{}(a[1], b[1]);
        result[2] = Op{}(a[2], b[2]);
        result[3] = Op{}(a[3], b[3]);
    } else {
        for (int i = 0; i < L; ++i) {
            result[i] = Op{}(a[i], b[i]);
        }
    }
    return result;
}

template <typename Op, VectorLike V>
requires UnaryOp<Op, typename V::value_type>
[[nodiscard("element-wise operation: discarding the result is likely a bug")]]
constexpr V apply_unary(const V& a) {
    V result;
    constexpr int L = V::dimension;
    if constexpr (L == 1) {
        result[0] = Op{}(a[0]);
    } else if constexpr (L == 2) {
        result[0] = Op{}(a[0]);
        result[1] = Op{}(a[1]);
    } else if constexpr (L == 3) {
        result[0] = Op{}(a[0]);
        result[1] = Op{}(a[1]);
        result[2] = Op{}(a[2]);
    } else if constexpr (L == 4) {
        result[0] = Op{}(a[0]);
        result[1] = Op{}(a[1]);
        result[2] = Op{}(a[2]);
        result[3] = Op{}(a[3]);
    } else {
        for (int i = 0; i < L; ++i) {
            result[i] = Op{}(a[i]);
        }
    }
    return result;
}

template <typename Op, VectorLike V>
requires BinaryOp<Op, typename V::value_type>
[[nodiscard("element-wise operation: discarding the result is likely a bug")]]
constexpr V apply_scalar_binary_left(typename V::value_type s, const V& v) {
    V result;
    constexpr int L = V::dimension;
    if constexpr (L == 1) {
        result[0] = Op{}(s, v[0]);
    } else if constexpr (L == 2) {
        result[0] = Op{}(s, v[0]);
        result[1] = Op{}(s, v[1]);
    } else if constexpr (L == 3) {
        result[0] = Op{}(s, v[0]);
        result[1] = Op{}(s, v[1]);
        result[2] = Op{}(s, v[2]);
    } else if constexpr (L == 4) {
        result[0] = Op{}(s, v[0]);
        result[1] = Op{}(s, v[1]);
        result[2] = Op{}(s, v[2]);
        result[3] = Op{}(s, v[3]);
    } else {
        for (int i = 0; i < L; ++i) {
            result[i] = Op{}(s, v[i]);
        }
    }
    return result;
}

template <typename Op, VectorLike V>
requires BinaryOp<Op, typename V::value_type>
[[nodiscard("element-wise operation: discarding the result is likely a bug")]]
constexpr V apply_scalar_binary_right(const V& v, typename V::value_type s) {
    V result;
    constexpr int L = V::dimension;
    if constexpr (L == 1) {
        result[0] = Op{}(v[0], s);
    } else if constexpr (L == 2) {
        result[0] = Op{}(v[0], s);
        result[1] = Op{}(v[1], s);
    } else if constexpr (L == 3) {
        result[0] = Op{}(v[0], s);
        result[1] = Op{}(v[1], s);
        result[2] = Op{}(v[2], s);
    } else if constexpr (L == 4) {
        result[0] = Op{}(v[0], s);
        result[1] = Op{}(v[1], s);
        result[2] = Op{}(v[2], s);
        result[3] = Op{}(v[3], s);
    } else {
        for (int i = 0; i < L; ++i) {
            result[i] = Op{}(v[i], s);
        }
    }
    return result;
}

}
