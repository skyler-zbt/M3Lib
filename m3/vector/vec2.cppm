export module m3.vector.vec2;

import m3.detail;
import m3.vector.base;
import m3.vector.vec;

export namespace m3 {
    template<detail::Arithmetic T, detail::Qualifier Q>
    class Vec<2, T, Q> : VectorBase<2, T, Q> {

        using base_type = VectorBase<2,T,Q>::VectorBase;
        using base_type::base_type;

        Vec() = default;
        explicit constexpr Vec(const T& v) : VectorBase<2, T, Q>(v) {}

        constexpr T& x();
        [[nodiscard]]
        constexpr const T& x() const;
        constexpr T& y();
        [[nodiscard]]
        constexpr const T& y() const;

        constexpr T& r();
        [[nodiscard]]
        constexpr const T& r() const;
        constexpr T& g();
        [[nodiscard]]
        constexpr const T& g() const;

        constexpr T& s();
        [[nodiscard]]
        constexpr const T& s() const;
        constexpr T& t();
        [[nodiscard]]
        constexpr const T& t() const;
    };

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr T & Vec<2, T, Q>::x() {
        return this->storage_[0];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr const T & Vec<2, T, Q>::x() const {
        return this->storage_[0];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr T & Vec<2, T, Q>::y() {
        return this->storage_[1];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr const T & Vec<2, T, Q>::y() const {
        return this->storage_[1];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr T & Vec<2, T, Q>::r() {
        return this->storage_[0];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr const T & Vec<2, T, Q>::r() const {
        return this->storage_[0];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr T & Vec<2, T, Q>::g() {
        return this->storage_[1];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr const T & Vec<2, T, Q>::g() const {
        return this->storage_[1];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr T & Vec<2, T, Q>::s() {
        return this->storage_[0];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr const T & Vec<2, T, Q>::s() const {
        return this->storage_[0];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr T & Vec<2, T, Q>::t() {
        return this->storage_[1];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr const T & Vec<2, T, Q>::t() const {
        return this->storage_[1];
    }
} // namespace m3
