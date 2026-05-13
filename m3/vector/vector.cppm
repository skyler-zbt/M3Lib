export module m3.vector;

export import m3.vector.base;
export import m3.vector.vec;
export import m3.vector.vec1;

import m3.detail;
import m3.detail.operations.ops;
import m3.detail.operations.apply;

export namespace m3 {

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    constexpr Vec<L, T, Q> operator+(const Vec<L, T, Q>& a, const Vec<L, T, Q>& b) {
        return detail::apply_binary<detail::add>(a, b);
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    constexpr Vec<L, T, Q> operator-(const Vec<L, T, Q>& a, const Vec<L, T, Q>& b) {
        return detail::apply_binary<detail::sub>(a, b);
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    constexpr Vec<L, T, Q> operator*(const Vec<L, T, Q>& a, const Vec<L, T, Q>& b) {
        return detail::apply_binary<detail::mul>(a, b);
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    constexpr Vec<L, T, Q> operator/(const Vec<L, T, Q>& a, const Vec<L, T, Q>& b) {
        return detail::apply_binary<detail::div>(a, b);
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    constexpr Vec<L, T, Q> operator-(const Vec<L, T, Q>& a) {
        return detail::apply_unary<detail::neg>(a);
    }

}
