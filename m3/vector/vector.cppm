export module m3.vector;

export import m3.vector.base;
export import m3.vector.vec;
export import m3.vector.vec1;

import m3.detail;
import m3.detail.operations.ops;
import m3.detail.operations.apply;

export namespace m3 {

    template<int L, Arithmetic T, Qualifier Q>
    constexpr Vec<L, T, Q> operator+(const Vec<L, T, Q>& a, const Vec<L, T, Q>& b) {
        return detail::apply_binary<ops::add>(a, b);
    }

    template<int L, Arithmetic T, Qualifier Q>
    constexpr Vec<L, T, Q> operator-(const Vec<L, T, Q>& a, const Vec<L, T, Q>& b) {
        return detail::apply_binary<ops::sub>(a, b);
    }

    template<int L, Arithmetic T, Qualifier Q>
    constexpr Vec<L, T, Q> operator*(const Vec<L, T, Q>& a, const Vec<L, T, Q>& b) {
        return detail::apply_binary<ops::mul>(a, b);
    }

    template<int L, Arithmetic T, Qualifier Q>
    constexpr Vec<L, T, Q> operator/(const Vec<L, T, Q>& a, const Vec<L, T, Q>& b) {
        return detail::apply_binary<ops::div>(a, b);
    }

    template<int L, Arithmetic T, Qualifier Q>
    constexpr Vec<L, T, Q> operator-(const Vec<L, T, Q>& a) {
        return detail::apply_unary<ops::neg>(a);
    }

}
