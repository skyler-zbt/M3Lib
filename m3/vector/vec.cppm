export module m3.vector.vec;

import std;

import m3.detail;
import m3.vector.base;

export namespace m3 {

    // Primary template — parameterized on dimension L, element type T, and alignment Q
    template <int L, detail::Arithmetic T, detail::Qualifier Q = detail::Qualifier::aligned_high>
    requires detail::ValidDimension<L>
    class Vec : public VectorBase<L, T, Q> {
        using base_type = VectorBase<L, T, Q>::VectorBase;
        using base_type::base_type;

    public:
        Vec() = default;
    };

    // Vec<1,T,Q> — scalar alias
    template<detail::Arithmetic T, detail::Qualifier Q>
    class Vec<1, T, Q> : public VectorBase<1, T, Q> {
        using base_type = VectorBase<1, T, Q>::VectorBase;

    public:
        Vec() = default;
        explicit constexpr Vec(const T& v) : VectorBase<1, T, Q>(v) {}

        constexpr T& x()               { return (*this)[0]; }
        [[nodiscard]] constexpr const T& x() const { return (*this)[0]; }
        constexpr T& r()               { return (*this)[0]; }
        [[nodiscard]] constexpr const T& r() const { return (*this)[0]; }
        constexpr T& s()               { return (*this)[0]; }
        [[nodiscard]] constexpr const T& s() const { return (*this)[0]; }
    };

    // Vec<2,T,Q>
    template<detail::Arithmetic T, detail::Qualifier Q>
    class Vec<2, T, Q> : public VectorBase<2, T, Q> {
        using base_type = VectorBase<2, T, Q>::VectorBase;

    public:
        Vec() = default;
        explicit constexpr Vec(const T& v) : VectorBase<2, T, Q>(v) {}

        constexpr T& x()               { return (*this)[0]; }
        [[nodiscard]] constexpr const T& x() const { return (*this)[0]; }
        constexpr T& y()               { return (*this)[1]; }
        [[nodiscard]] constexpr const T& y() const { return (*this)[1]; }

        constexpr T& r()               { return (*this)[0]; }
        [[nodiscard]] constexpr const T& r() const { return (*this)[0]; }
        constexpr T& g()               { return (*this)[1]; }
        [[nodiscard]] constexpr const T& g() const { return (*this)[1]; }

        constexpr T& s()               { return (*this)[0]; }
        [[nodiscard]] constexpr const T& s() const { return (*this)[0]; }
        constexpr T& t()               { return (*this)[1]; }
        [[nodiscard]] constexpr const T& t() const { return (*this)[1]; }
    };

    // Vec<3,T,Q>
    template<detail::Arithmetic T, detail::Qualifier Q>
    class Vec<3, T, Q> : public VectorBase<3, T, Q> {
        using base_type = VectorBase<3, T, Q>::VectorBase;

    public:
        Vec() = default;
        explicit constexpr Vec(const T& v) : VectorBase<3, T, Q>(v) {}

        constexpr T& x()               { return (*this)[0]; }
        [[nodiscard]] constexpr const T& x() const { return (*this)[0]; }
        constexpr T& y()               { return (*this)[1]; }
        [[nodiscard]] constexpr const T& y() const { return (*this)[1]; }
        constexpr T& z()               { return (*this)[2]; }
        [[nodiscard]] constexpr const T& z() const { return (*this)[2]; }

        constexpr T& r()               { return (*this)[0]; }
        [[nodiscard]] constexpr const T& r() const { return (*this)[0]; }
        constexpr T& g()               { return (*this)[1]; }
        [[nodiscard]] constexpr const T& g() const { return (*this)[1]; }
        constexpr T& b()               { return (*this)[2]; }
        [[nodiscard]] constexpr const T& b() const { return (*this)[2]; }

        constexpr T& s()               { return (*this)[0]; }
        [[nodiscard]] constexpr const T& s() const { return (*this)[0]; }
        constexpr T& t()               { return (*this)[1]; }
        [[nodiscard]] constexpr const T& t() const { return (*this)[1]; }
        constexpr T& p()               { return (*this)[2]; }
        [[nodiscard]] constexpr const T& p() const { return (*this)[2]; }
    };

    // Vec<4,T,Q>
    template<detail::Arithmetic T, detail::Qualifier Q>
    class Vec<4, T, Q> : public VectorBase<4, T, Q> {
        using base_type = VectorBase<4, T, Q>::VectorBase;

    public:
        Vec() = default;
        explicit constexpr Vec(const T& v) : VectorBase<4, T, Q>(v) {}

        constexpr T& x()               { return (*this)[0]; }
        [[nodiscard]] constexpr const T& x() const { return (*this)[0]; }
        constexpr T& y()               { return (*this)[1]; }
        [[nodiscard]] constexpr const T& y() const { return (*this)[1]; }
        constexpr T& z()               { return (*this)[2]; }
        [[nodiscard]] constexpr const T& z() const { return (*this)[2]; }
        constexpr T& w()               { return (*this)[3]; }
        [[nodiscard]] constexpr const T& w() const { return (*this)[3]; }

        constexpr T& r()               { return (*this)[0]; }
        [[nodiscard]] constexpr const T& r() const { return (*this)[0]; }
        constexpr T& g()               { return (*this)[1]; }
        [[nodiscard]] constexpr const T& g() const { return (*this)[1]; }
        constexpr T& b()               { return (*this)[2]; }
        [[nodiscard]] constexpr const T& b() const { return (*this)[2]; }
        constexpr T& a()               { return (*this)[3]; }
        [[nodiscard]] constexpr const T& a() const { return (*this)[3]; }

        constexpr T& s()               { return (*this)[0]; }
        [[nodiscard]] constexpr const T& s() const { return (*this)[0]; }
        constexpr T& t()               { return (*this)[1]; }
        [[nodiscard]] constexpr const T& t() const { return (*this)[1]; }
        constexpr T& p()               { return (*this)[2]; }
        [[nodiscard]] constexpr const T& p() const { return (*this)[2]; }
        constexpr T& q()               { return (*this)[3]; }
        [[nodiscard]] constexpr const T& q() const { return (*this)[3]; }
    };

} // namespace m3
