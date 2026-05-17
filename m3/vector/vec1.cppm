export module m3.vector.vec1;

import m3.detail;
import m3.vector.base;
import m3.vector.vec;

export namespace m3 {
    template<detail::Arithmetic T, detail::Qualifier Q>
    class Vec<1, T, Q> : VectorBase<1, T, Q> {

        using base_type = VectorBase<1,T,Q>::VectorBase;
        using base_type::base_type;

        Vec() = default;
        explicit constexpr Vec(const T& v) : VectorBase<1, T, Q>(v) {}

        constexpr T& x();
        [[nodiscard]]
        constexpr const T& x() const;

        constexpr T& r();
        [[nodiscard]]
        constexpr const T& r() const;

        constexpr T& s();
        [[nodiscard]]
        constexpr const T& s() const;
    };

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr T & Vec<1, T, Q>::x() {
        return this->storage_[0];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr const T & Vec<1, T, Q>::x() const {
        return this->storage_[0];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr T & Vec<1, T, Q>::r() {
        return this->storage_[0];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr const T & Vec<1, T, Q>::r() const {
        return this->storage_[0];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr T & Vec<1, T, Q>::s() {
        return this->storage_[0];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr const T & Vec<1, T, Q>::s() const {
        return this->storage_[0];
    }
} // namespace m3
