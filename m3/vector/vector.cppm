export module m3.vector;

export import m3.vector.base;
export import m3.vector.vec;

import m3.detail;
import m3.detail.operations.ops;
import m3.detail.operations.apply;

export namespace m3 {

    // component-wise binary operators (delegate to detail::apply_binary)

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    [[nodiscard]] constexpr Vec<L, T, Q> operator+(const Vec<L, T, Q>& a, const Vec<L, T, Q>& b) {
        return detail::apply_binary<detail::add>(a, b);
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    [[nodiscard]] constexpr Vec<L, T, Q> operator-(const Vec<L, T, Q>& a, const Vec<L, T, Q>& b) {
        return detail::apply_binary<detail::sub>(a, b);
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    [[nodiscard]] constexpr Vec<L, T, Q> operator*(const Vec<L, T, Q>& a, const Vec<L, T, Q>& b) {
        return detail::apply_binary<detail::mul>(a, b);
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    [[nodiscard]] constexpr Vec<L, T, Q> operator/(const Vec<L, T, Q>& a, const Vec<L, T, Q>& b) {
        return detail::apply_binary<detail::div>(a, b);
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    [[nodiscard]] constexpr Vec<L, T, Q> operator-(const Vec<L, T, Q>& a) {
        return detail::apply_unary<detail::neg>(a);
    }

    // scalar operators — unrolled for L ∈ {1,2,3,4}, generic loop otherwise

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    [[nodiscard]] constexpr Vec<L, T, Q> operator*(const Vec<L, T, Q>& a, T scalar) {
        Vec<L, T, Q> result;
        if constexpr (L == 1) {
            result[0] = a[0] * scalar;
        }
        else if constexpr (L == 2) {
            result[0] = a[0] * scalar;
            result[1] = a[1] * scalar;
        }
        else if constexpr (L == 3) {
            result[0] = a[0] * scalar;
            result[1] = a[1] * scalar;
            result[2] = a[2] * scalar;
        }
        else if constexpr (L == 4) {
            result[0] = a[0] * scalar;
            result[1] = a[1] * scalar;
            result[2] = a[2] * scalar;
            result[3] = a[3] * scalar;
        }
        else {
            for (int i = 0; i < L; ++i) {
                result[i] = a[i] * scalar;
            }
        }
        return result;
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    [[nodiscard]] constexpr Vec<L, T, Q> operator*(T scalar, const Vec<L, T, Q>& a) {
        return a * scalar;
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    [[nodiscard]] constexpr Vec<L, T, Q> operator/(const Vec<L, T, Q>& a, T scalar) {
        Vec<L, T, Q> result;
        if constexpr (L == 1) {
            result[0] = a[0] / scalar;
        }
        else if constexpr (L == 2) {
            result[0] = a[0] / scalar;
            result[1] = a[1] / scalar;
        }
        else if constexpr (L == 3) {
            result[0] = a[0] / scalar;
            result[1] = a[1] / scalar;
            result[2] = a[2] / scalar;
        }
        else if constexpr (L == 4) {
            result[0] = a[0] / scalar;
            result[1] = a[1] / scalar;
            result[2] = a[2] / scalar;
            result[3] = a[3] / scalar;
        }
        else {
            for (int i = 0; i < L; ++i) {
                result[i] = a[i] / scalar;
            }
        }
        return result;
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    [[nodiscard]] constexpr Vec<L, T, Q> operator+(const Vec<L, T, Q>& a, T scalar) {
        Vec<L, T, Q> result;
        if constexpr (L == 1) {
            result[0] = a[0] + scalar;
        }
        else if constexpr (L == 2) {
            result[0] = a[0] + scalar;
            result[1] = a[1] + scalar;
        }
        else if constexpr (L == 3) {
            result[0] = a[0] + scalar;
            result[1] = a[1] + scalar;
            result[2] = a[2] + scalar;
        }
        else if constexpr (L == 4) {
            result[0] = a[0] + scalar;
            result[1] = a[1] + scalar;
            result[2] = a[2] + scalar;
            result[3] = a[3] + scalar;
        }
        else {
            for (int i = 0; i < L; ++i) {
                result[i] = a[i] + scalar;
            }
        }
        return result;
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    [[nodiscard]] constexpr Vec<L, T, Q> operator+(T scalar, const Vec<L, T, Q>& a) {
        return a + scalar;
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    [[nodiscard]] constexpr Vec<L, T, Q> operator-(const Vec<L, T, Q>& a, T scalar) {
        Vec<L, T, Q> result;
        if constexpr (L == 1) {
            result[0] = a[0] - scalar;
        }
        else if constexpr (L == 2) {
            result[0] = a[0] - scalar;
            result[1] = a[1] - scalar;
        }
        else if constexpr (L == 3) {
            result[0] = a[0] - scalar;
            result[1] = a[1] - scalar;
            result[2] = a[2] - scalar;
        }
        else if constexpr (L == 4) {
            result[0] = a[0] - scalar;
            result[1] = a[1] - scalar;
            result[2] = a[2] - scalar;
            result[3] = a[3] - scalar;
        }
        else {
            for (int i = 0; i < L; ++i) {
                result[i] = a[i] - scalar;
            }
        }
        return result;
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    [[nodiscard]] constexpr Vec<L, T, Q> operator-(T scalar, const Vec<L, T, Q>& a) {
        Vec<L, T, Q> result;
        if constexpr (L == 1) {
            result[0] = scalar - a[0];
        }
        else if constexpr (L == 2) {
            result[0] = scalar - a[0];
            result[1] = scalar - a[1];
        }
        else if constexpr (L == 3) {
            result[0] = scalar - a[0];
            result[1] = scalar - a[1];
            result[2] = scalar - a[2];
        }
        else if constexpr (L == 4) {
            result[0] = scalar - a[0];
            result[1] = scalar - a[1];
            result[2] = scalar - a[2];
            result[3] = scalar - a[3];
        }
        else {
            for (int i = 0; i < L; ++i) {
                result[i] = scalar - a[i];
            }
        }
        return result;
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    [[nodiscard]] constexpr bool operator==(const Vec<L, T, Q>& a, const Vec<L, T, Q>& b) {
        for (int i = 0; i < L; ++i) {
            if (a[i] != b[i]) return false;
        }
        return true;
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    [[nodiscard]] constexpr bool operator!=(const Vec<L, T, Q>& a, const Vec<L, T, Q>& b) {
        return !(a == b);
    }

} // namespace m3
