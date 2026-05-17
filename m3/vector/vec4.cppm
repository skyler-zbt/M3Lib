export module m3.vector.vec4;

import m3.detail;
import m3.vector.base;
import m3.vector.vec;

export namespace m3 {
    template<detail::Arithmetic T, detail::Qualifier Q>
    class Vec<4, T, Q> : VectorBase<4, T, Q> {

        using base_type = VectorBase<4,T,Q>::VectorBase;
        using base_type::base_type;

        Vec() = default;
        explicit constexpr Vec(const T& v) : VectorBase<4, T, Q>(v) {}

        //xyzw
        constexpr T& x();
        [[nodiscard]]
        constexpr const T& x() const;
        constexpr T& y();
        [[nodiscard]]
        constexpr const T& y() const;
        constexpr T& z();
        [[nodiscard]]
        constexpr const T& z() const;
        constexpr T& w();
        [[nodiscard]]
        constexpr const T& w() const;

        //rgba
        constexpr T& r();
        [[nodiscard]]
        constexpr const T& r() const;
        constexpr T& g();
        [[nodiscard]]
        constexpr const T& g() const;
        constexpr T& b();
        [[nodiscard]]
        constexpr const T& b() const;
        constexpr T& a();
        [[nodiscard]]
        constexpr const T& a() const;

        //stpq
        constexpr T& s();
        [[nodiscard]]
        constexpr const T& s() const;
        constexpr T& t();
        [[nodiscard]]
        constexpr const T& t() const;
        constexpr T& p();
        [[nodiscard]]
        constexpr const T& p() const;
        constexpr T& q();
        [[nodiscard]]
        constexpr const T& q() const;
    };

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr T & Vec<4, T, Q>::x() {
        return this->storage_[0];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr const T & Vec<4, T, Q>::x() const {
        return this->storage_[0];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr T & Vec<4, T, Q>::y() {
        return this->storage_[1];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr const T & Vec<4, T, Q>::y() const {
        return this->storage_[1];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr T & Vec<4, T, Q>::z() {
        return this->storage_[2];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr const T & Vec<4, T, Q>::z() const {
        return this->storage_[2];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr T & Vec<4, T, Q>::w() {
        return this->storage_[3];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr const T & Vec<4, T, Q>::w() const {
        return this->storage_[3];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr T & Vec<4, T, Q>::r() {
        return this->storage_[0];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr const T & Vec<4, T, Q>::r() const {
        return this->storage_[0];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr T & Vec<4, T, Q>::g() {
        return this->storage_[1];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr const T & Vec<4, T, Q>::g() const {
        return this->storage_[1];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr T & Vec<4, T, Q>::b() {
        return this->storage_[2];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr const T & Vec<4, T, Q>::b() const {
        return this->storage_[2];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr T & Vec<4, T, Q>::a() {
        return this->storage_[3];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr const T & Vec<4, T, Q>::a() const {
        return this->storage_[3];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr T & Vec<4, T, Q>::s() {
        return this->storage_[0];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr const T & Vec<4, T, Q>::s() const {
        return this->storage_[0];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr T & Vec<4, T, Q>::t() {
        return this->storage_[1];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr const T & Vec<4, T, Q>::t() const {
        return this->storage_[1];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr T & Vec<4, T, Q>::p() {
        return this->storage_[2];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr const T & Vec<4, T, Q>::p() const {
        return this->storage_[2];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr T & Vec<4, T, Q>::q() {
        return this->storage_[3];
    }

    template<detail::Arithmetic T, detail::Qualifier Q>
    constexpr const T & Vec<4, T, Q>::q() const {
        return this->storage_[3];
    }
} // namespace m3
