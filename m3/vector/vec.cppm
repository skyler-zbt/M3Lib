export module m3.vector.vec;

import std;

import m3.detail;
import m3.vector.base;

export namespace m3 {

    template <int L, detail::Arithmetic T, detail::Qualifier Q = detail::Qualifier::aligned_none>
    requires detail::ValidVecDimension<L>
    class Vec : public m3::detail::VectorBase<L, T, Q> {
        using base_type = m3::detail::VectorBase<L, T, Q>::VectorBase;
        using base_type::base_type;
    public:
        static constexpr int dimension = L;
        using value_type = T;
        using qualifier_type = detail::Qualifier;

        Vec() = default;

        explicit constexpr Vec(const T* arr) noexcept : m3::detail::VectorBase<L, T, Q>() {
            for (int i = 0; i < L; ++i)
                (*this)[i] = arr[i];
        }
    };

    template <detail::Arithmetic T, detail::Qualifier Q>
    class Vec<1, T, Q> : public m3::detail::VectorBase<1, T, Q> {
    public:
        static constexpr int dimension = 1;
        using value_type = T;
        using qualifier_type = detail::Qualifier;

        Vec() = default;

        explicit constexpr Vec(const T& v) noexcept : m3::detail::VectorBase<1, T, Q>(v) {}

        explicit constexpr Vec(const T* arr) noexcept : m3::detail::VectorBase<1, T, Q>() {
            (*this)[0] = arr[0];
        }

        constexpr T& x() {
            return (*this)[0];
        }
        [[nodiscard]] constexpr const T& x() const {
            return (*this)[0];
        }
        constexpr T& r() {
            return (*this)[0];
        }
        [[nodiscard]] constexpr const T& r() const {
            return (*this)[0];
        }
        constexpr T& s() {
            return (*this)[0];
        }
        [[nodiscard]] constexpr const T& s() const {
            return (*this)[0];
        }
    };

    template <detail::Arithmetic T, detail::Qualifier Q>
    class Vec<2, T, Q> : public m3::detail::VectorBase<2, T, Q> {
    public:
        static constexpr int dimension = 2;
        using value_type = T;
        using qualifier_type = detail::Qualifier;

        Vec() = default;

        explicit constexpr Vec(const T& v) noexcept : m3::detail::VectorBase<2, T, Q>(v) {}

        constexpr Vec(const T& x, const T& y) noexcept : m3::detail::VectorBase<2, T, Q>() {
            (*this)[0] = x;
            (*this)[1] = y;
        }

        explicit constexpr Vec(const T* arr) noexcept : m3::detail::VectorBase<2, T, Q>() {
            (*this)[0] = arr[0];
            (*this)[1] = arr[1];
        }

        constexpr T& x() {
            return (*this)[0];
        }
        [[nodiscard]] constexpr const T& x() const {
            return (*this)[0];
        }
        constexpr T& y() {
            return (*this)[1];
        }
        [[nodiscard]] constexpr const T& y() const {
            return (*this)[1];
        }

        constexpr T& r() {
            return (*this)[0];
        }
        [[nodiscard]] constexpr const T& r() const {
            return (*this)[0];
        }
        constexpr T& g() {
            return (*this)[1];
        }
        [[nodiscard]] constexpr const T& g() const {
            return (*this)[1];
        }

        constexpr T& s() {
            return (*this)[0];
        }
        [[nodiscard]] constexpr const T& s() const {
            return (*this)[0];
        }
        constexpr T& t() {
            return (*this)[1];
        }
        [[nodiscard]] constexpr const T& t() const {
            return (*this)[1];
        }
    };

    template <detail::Arithmetic T, detail::Qualifier Q>
    class Vec<3, T, Q> : public m3::detail::VectorBase<3, T, Q> {
    public:
        static constexpr int dimension = 3;
        using value_type = T;
        using qualifier_type = detail::Qualifier;

        Vec() = default;

        explicit constexpr Vec(const T& v) noexcept : m3::detail::VectorBase<3, T, Q>(v) {}

        constexpr Vec(const T& x, const T& y, const T& z) noexcept : m3::detail::VectorBase<3, T, Q>() {
            (*this)[0] = x;
            (*this)[1] = y;
            (*this)[2] = z;
        }

        explicit constexpr Vec(const T* arr) noexcept : m3::detail::VectorBase<3, T, Q>() {
            (*this)[0] = arr[0];
            (*this)[1] = arr[1];
            (*this)[2] = arr[2];
        }

        constexpr T& x() {
            return (*this)[0];
        }
        [[nodiscard]] constexpr const T& x() const {
            return (*this)[0];
        }
        constexpr T& y() {
            return (*this)[1];
        }
        [[nodiscard]] constexpr const T& y() const {
            return (*this)[1];
        }
        constexpr T& z() {
            return (*this)[2];
        }
        [[nodiscard]] constexpr const T& z() const {
            return (*this)[2];
        }

        constexpr T& r() {
            return (*this)[0];
        }
        [[nodiscard]] constexpr const T& r() const {
            return (*this)[0];
        }
        constexpr T& g() {
            return (*this)[1];
        }
        [[nodiscard]] constexpr const T& g() const {
            return (*this)[1];
        }
        constexpr T& b() {
            return (*this)[2];
        }
        [[nodiscard]] constexpr const T& b() const {
            return (*this)[2];
        }

        constexpr T& s() {
            return (*this)[0];
        }
        [[nodiscard]] constexpr const T& s() const {
            return (*this)[0];
        }
        constexpr T& t() {
            return (*this)[1];
        }
        [[nodiscard]] constexpr const T& t() const {
            return (*this)[1];
        }
        constexpr T& p() {
            return (*this)[2];
        }
        [[nodiscard]] constexpr const T& p() const {
            return (*this)[2];
        }
    };

    template <detail::Arithmetic T, detail::Qualifier Q>
    class Vec<4, T, Q> : public m3::detail::VectorBase<4, T, Q> {
    public:
        static constexpr int dimension = 4;
        using value_type = T;
        using qualifier_type = detail::Qualifier;

        Vec() = default;

        explicit constexpr Vec(const T& v) noexcept : m3::detail::VectorBase<4, T, Q>(v) {}

        constexpr Vec(const T& x, const T& y, const T& z, const T& w) noexcept
            : m3::detail::VectorBase<4, T, Q>() {
            (*this)[0] = x;
            (*this)[1] = y;
            (*this)[2] = z;
            (*this)[3] = w;
        }

        explicit constexpr Vec(const T* arr) noexcept : m3::detail::VectorBase<4, T, Q>() {
            (*this)[0] = arr[0];
            (*this)[1] = arr[1];
            (*this)[2] = arr[2];
            (*this)[3] = arr[3];
        }

        constexpr T& x() {
            return (*this)[0];
        }
        [[nodiscard]] constexpr const T& x() const {
            return (*this)[0];
        }
        constexpr T& y() {
            return (*this)[1];
        }
        [[nodiscard]] constexpr const T& y() const {
            return (*this)[1];
        }
        constexpr T& z() {
            return (*this)[2];
        }
        [[nodiscard]] constexpr const T& z() const {
            return (*this)[2];
        }
        constexpr T& w() {
            return (*this)[3];
        }
        [[nodiscard]] constexpr const T& w() const {
            return (*this)[3];
        }

        constexpr T& r() {
            return (*this)[0];
        }
        [[nodiscard]] constexpr const T& r() const {
            return (*this)[0];
        }
        constexpr T& g() {
            return (*this)[1];
        }
        [[nodiscard]] constexpr const T& g() const {
            return (*this)[1];
        }
        constexpr T& b() {
            return (*this)[2];
        }
        [[nodiscard]] constexpr const T& b() const {
            return (*this)[2];
        }
        constexpr T& a() {
            return (*this)[3];
        }
        [[nodiscard]] constexpr const T& a() const {
            return (*this)[3];
        }

        constexpr T& s() {
            return (*this)[0];
        }
        [[nodiscard]] constexpr const T& s() const {
            return (*this)[0];
        }
        constexpr T& t() {
            return (*this)[1];
        }
        [[nodiscard]] constexpr const T& t() const {
            return (*this)[1];
        }
        constexpr T& p() {
            return (*this)[2];
        }
        [[nodiscard]] constexpr const T& p() const {
            return (*this)[2];
        }
        constexpr T& q() {
            return (*this)[3];
        }
        [[nodiscard]] constexpr const T& q() const {
            return (*this)[3];
        }
    };

    using vec2 = Vec<2, float>;
    using vec3 = Vec<3, float>;
    using vec4 = Vec<4, float>;

    using ivec2 = Vec<2, int>;
    using ivec3 = Vec<3, int>;
    using ivec4 = Vec<4, int>;

}

namespace std {

    template <int L, m3::detail::Arithmetic T, m3::detail::Qualifier Q>
    struct tuple_size<m3::Vec<L, T, Q>> : integral_constant<std::size_t, static_cast<std::size_t>(L)> {
    };

    template <std::size_t I, int L, m3::detail::Arithmetic T, m3::detail::Qualifier Q>
    requires(I < static_cast<std::size_t>(L))
    struct tuple_element<I, m3::Vec<L, T, Q>> {
        using type = T;
    };

}

namespace std {

template <int L, m3::detail::Arithmetic T, m3::detail::Qualifier Q>
struct formatter<m3::Vec<L, T, Q>> {
    constexpr auto parse(format_parse_context& ctx) {

        auto it = ctx.begin();
        if (it != ctx.end() && *it != '}') {
            throw format_error("Vec formatter only supports {} (default)");
        }
        return it;
    }

    auto format(const m3::Vec<L, T, Q>& v, format_context& ctx) const {
        auto out = ctx.out();
        *out++ = '[';
        for (int i = 0; i < L; ++i) {
            if (i > 0) {
                *out++ = ',';
                *out++ = ' ';
            }
            out = std::format_to(out, "{}", v[i]);
        }
        *out++ = ']';
        return out;
    }
};

}

export namespace m3 {

template <std::size_t I, int L, detail::Arithmetic T, detail::Qualifier Q>
requires(I < static_cast<std::size_t>(L))
[[nodiscard("tuple accessor: discarding a retrieved element is likely a bug")]]
constexpr T& get(Vec<L, T, Q>& v) noexcept {
    return v[static_cast<std::size_t>(I)];
}

template <std::size_t I, int L, detail::Arithmetic T, detail::Qualifier Q>
requires(I < static_cast<std::size_t>(L))
[[nodiscard("tuple accessor: discarding a retrieved element is likely a bug")]]
constexpr const T& get(const Vec<L, T, Q>& v) noexcept {
    return v[static_cast<std::size_t>(I)];
}

template <std::size_t I, int L, detail::Arithmetic T, detail::Qualifier Q>
requires(I < static_cast<std::size_t>(L))
[[nodiscard("tuple accessor: discarding a retrieved element is likely a bug")]]
constexpr T&& get(Vec<L, T, Q>&& v) noexcept {
    return std::move(v[static_cast<std::size_t>(I)]);
}

template <std::size_t I, int L, detail::Arithmetic T, detail::Qualifier Q>
requires(I < static_cast<std::size_t>(L))
[[nodiscard("tuple accessor: discarding a retrieved element is likely a bug")]]
constexpr const T&& get(const Vec<L, T, Q>&& v) noexcept {
    return std::move(v[static_cast<std::size_t>(I)]);
}

}
