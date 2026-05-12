export module m3.vector.vec1;

import m3.detail;
import m3.vector.base;

export namespace m3 {
    template<Arithmetic T, Qualifier Q>
    struct Vec<1, T, Q> : VectorBase<1, T, Q>{

        using base_type = VectorBase<1, T, Q>;
        using base_type::storage;

        constexpr T& x();
        constexpr const T& x() const;

        constexpr T& r();
        constexpr const T& r() const;

        constexpr T& s();
        constexpr const T& s() const;

    };
} // namespace m3
