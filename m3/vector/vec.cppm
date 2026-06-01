// Vec — the primary vector type with GLSL-style swizzle accessors.
// Specializations for dimensions 1–4 provide .x/.y/.z/.w, .r/.g/.b/.a, .s/.t/.p/.q.
// Vec<5+, T, Q> is supported via the primary template (no swizzle accessors).
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

        // Construct from a C array of L elements.  The caller must ensure
        // arr points to at least L valid, initialized values.
        explicit constexpr Vec(const T* arr) noexcept : VectorBase<L, T, Q>() {
            for (int i = 0; i < L; ++i) (*this)[i] = arr[i];
        }
    };

    // Vec<1,T,Q> — wraps a single scalar with x/r/s accessors
    template<detail::Arithmetic T, detail::Qualifier Q>
    class Vec<1, T, Q> : public VectorBase<1, T, Q> {
    public:
        Vec() = default;

        // Broadcast: fills the single element with v
        explicit constexpr Vec(const T& v) noexcept : VectorBase<1, T, Q>(v) {}

        // Pointer: reads one element from arr
        explicit constexpr Vec(const T* arr) noexcept : VectorBase<1, T, Q>() {
            (*this)[0] = arr[0];
        }

        // Spatial, colour, and texture-coordinate aliases for element 0
        constexpr T& x()               { return (*this)[0]; }
        [[nodiscard]] constexpr const T& x() const { return (*this)[0]; }
        constexpr T& r()               { return (*this)[0]; }
        [[nodiscard]] constexpr const T& r() const { return (*this)[0]; }
        constexpr T& s()               { return (*this)[0]; }
        [[nodiscard]] constexpr const T& s() const { return (*this)[0]; }
    };

    // Vec<2,T,Q> — two-component vector with x/y, r/g, s/t accessors
    template<detail::Arithmetic T, detail::Qualifier Q>
    class Vec<2, T, Q> : public VectorBase<2, T, Q> {
    public:
        Vec() = default;

        // Broadcast: fills all 2 elements with v
        explicit constexpr Vec(const T& v) noexcept : VectorBase<2, T, Q>(v) {}

        // Component-wise: Vec<2, float>{1.5f, 2.0f}
        constexpr Vec(const T& x, const T& y) noexcept : VectorBase<2, T, Q>() {
            (*this)[0] = x; (*this)[1] = y;
        }

        // Pointer: reads 2 elements from arr
        explicit constexpr Vec(const T* arr) noexcept : VectorBase<2, T, Q>() {
            (*this)[0] = arr[0]; (*this)[1] = arr[1];
        }

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

    // Vec<3,T,Q> — three-component vector with x/y/z, r/g/b, s/t/p accessors
    template<detail::Arithmetic T, detail::Qualifier Q>
    class Vec<3, T, Q> : public VectorBase<3, T, Q> {
    public:
        Vec() = default;

        // Broadcast: fills all 3 elements with v
        explicit constexpr Vec(const T& v) noexcept : VectorBase<3, T, Q>(v) {}

        // Component-wise: Vec<3, float>{1.0f, 0.0f, 0.5f}
        constexpr Vec(const T& x, const T& y, const T& z) noexcept : VectorBase<3, T, Q>() {
            (*this)[0] = x; (*this)[1] = y; (*this)[2] = z;
        }

        // Pointer: reads 3 elements from arr
        explicit constexpr Vec(const T* arr) noexcept : VectorBase<3, T, Q>() {
            (*this)[0] = arr[0]; (*this)[1] = arr[1]; (*this)[2] = arr[2];
        }

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

    // Vec<4,T,Q> — four-component vector with x/y/z/w, r/g/b/a, s/t/p/q accessors
    template<detail::Arithmetic T, detail::Qualifier Q>
    class Vec<4, T, Q> : public VectorBase<4, T, Q> {
    public:
        Vec() = default;

        // Broadcast: fills all 4 elements with v
        explicit constexpr Vec(const T& v) noexcept : VectorBase<4, T, Q>(v) {}

        // Component-wise: Vec<4, float>{1.0f, 0.0f, 0.0f, 1.0f}
        constexpr Vec(const T& x, const T& y, const T& z, const T& w) noexcept : VectorBase<4, T, Q>() {
            (*this)[0] = x; (*this)[1] = y; (*this)[2] = z; (*this)[3] = w;
        }

        // Pointer: reads 4 elements from arr
        explicit constexpr Vec(const T* arr) noexcept : VectorBase<4, T, Q>() {
            (*this)[0] = arr[0]; (*this)[1] = arr[1];
            (*this)[2] = arr[2]; (*this)[3] = arr[3];
        }

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

// ---- C++26 constexpr structured bindings (P2686R4) ----
// Provide tuple-like access for decomposing Vec into its components:
//   constexpr auto [x, y, z] = Vec<3, float>{1, 2, 3};
//   auto [r, g, b, a] = some_color;

namespace std {

    template<int L, m3::detail::Arithmetic T, m3::detail::Qualifier Q>
    struct tuple_size<m3::Vec<L, T, Q>>
        : integral_constant<std::size_t, static_cast<std::size_t>(L)> {};

    template<std::size_t I, int L, m3::detail::Arithmetic T, m3::detail::Qualifier Q>
    requires (I < static_cast<std::size_t>(L))
    struct tuple_element<I, m3::Vec<L, T, Q>> {
        using type = T;
    };

} // namespace std

// ---- std::formatter for Vec (debug / display) ----

namespace std {

    template<int L, m3::detail::Arithmetic T, m3::detail::Qualifier Q>
    struct formatter<m3::Vec<L, T, Q>> {
        constexpr auto parse(format_parse_context& ctx) {
            // Accept only default format specifier for now.
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

} // namespace std

export namespace m3 {

    template<std::size_t I, int L, detail::Arithmetic T, detail::Qualifier Q>
    requires (I < static_cast<std::size_t>(L))
    [[nodiscard("tuple accessor: discarding a retrieved element is likely a bug")]]
    constexpr T& get(Vec<L, T, Q>& v) noexcept {
        return v[static_cast<std::size_t>(I)];
    }

    template<std::size_t I, int L, detail::Arithmetic T, detail::Qualifier Q>
    requires (I < static_cast<std::size_t>(L))
    [[nodiscard("tuple accessor: discarding a retrieved element is likely a bug")]]
    constexpr const T& get(const Vec<L, T, Q>& v) noexcept {
        return v[static_cast<std::size_t>(I)];
    }

    template<std::size_t I, int L, detail::Arithmetic T, detail::Qualifier Q>
    requires (I < static_cast<std::size_t>(L))
    [[nodiscard("tuple accessor: discarding a retrieved element is likely a bug")]]
    constexpr T&& get(Vec<L, T, Q>&& v) noexcept {
        return std::move(v[static_cast<std::size_t>(I)]);
    }

    template<std::size_t I, int L, detail::Arithmetic T, detail::Qualifier Q>
    requires (I < static_cast<std::size_t>(L))
    [[nodiscard("tuple accessor: discarding a retrieved element is likely a bug")]]
    constexpr const T&& get(const Vec<L, T, Q>&& v) noexcept {
        return std::move(v[static_cast<std::size_t>(I)]);
    }

} // namespace m3
