export module m3.vector.vec3;

import m3.detail;
import m3.vector.base;
import m3.vector.vec;

export namespace m3 {
    template<detail::Arithmetic T, detail::Qualifier Q>
    class Vec<3, T, Q> : VectorBase<3, T, Q> {

        using base_type = VectorBase<3,T,Q>::VectorBase;
        using base_type::base_type;

        Vec() = default;
        explicit constexpr Vec(const T& v) : VectorBase<3, T, Q>(v) {}

        //xyz
        constexpr T& x();
        [[nodiscard]]
        constexpr const T& x() const;
        constexpr T& y();
        [[nodiscard]]
        constexpr const T& y() const;
        constexpr T& z();
        [[nodiscard]]
        constexpr const T& z() const;

        //rgb
        constexpr T& r();
        [[nodiscard]]
        constexpr const T& r() const;
        constexpr T& g();
        [[nodiscard]]
        constexpr const T& g() const;
        constexpr T& b();
        [[nodiscard]]
        constexpr const T& b() const;

        //stp
        constexpr T& s();
        [[nodiscard]]
        constexpr const T& s() const;
        constexpr T& t();
        [[nodiscard]]
        constexpr const T& t() const;
        constexpr T& p();
        [[nodiscard]]
        constexpr const T& p() const;
    };

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr T & Vec<3, T, Q>::x() {
        return this->storage_[0];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr const T & Vec<3, T, Q>::x() const {
        return this->storage_[0];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr T & Vec<3, T, Q>::y() {
        return this->storage_[1];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr const T & Vec<3, T, Q>::y() const {
        return this->storage_[1];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr T & Vec<3, T, Q>::z() {
        return this->storage_[2];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr const T & Vec<3, T, Q>::z() const {
        return this->storage_[2];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr T & Vec<3, T, Q>::r() {
        return this->storage_[0];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr const T & Vec<3, T, Q>::r() const {
        return this->storage_[0];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr T & Vec<3, T, Q>::g() {
        return this->storage_[1];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr const T & Vec<3, T, Q>::g() const {
        return this->storage_[1];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr T & Vec<3, T, Q>::b() {
        return this->storage_[2];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr const T & Vec<3, T, Q>::b() const {
        return this->storage_[2];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr T & Vec<3, T, Q>::s() {
        return this->storage_[0];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr const T & Vec<3, T, Q>::s() const {
        return this->storage_[0];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr T & Vec<3, T, Q>::t() {
        return this->storage_[1];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr const T & Vec<3, T, Q>::t() const {
        return this->storage_[1];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr T & Vec<3, T, Q>::p() {
        return this->storage_[2];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr const T & Vec<3, T, Q>::p() const {
        return this->storage_[2];
    }
} // namespace m3
