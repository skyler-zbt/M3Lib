export module m3.vector.base;

import std;

import m3.detail;
import m3.vector.storage;

export namespace m3::detail {

    template <int L, detail::Arithmetic T, detail::Qualifier Q>
    requires detail::ValidVecDimension<L>
    class VectorBase {
    public:
        VectorBase() = default;
        VectorBase(const VectorBase&) = default;

        template <int M>
        requires(M != L)
        explicit VectorBase(const VectorBase<M, T, Q>&) =
            delete ("cannot construct Vec from a vector of different dimension; "
                    "use explicit element-wise conversion instead");

        constexpr explicit VectorBase(T scalar) noexcept;

        [[nodiscard("raw pointer to underlying data; intended for C API / SIMD interop")]]
        constexpr T* value_ptr() noexcept;
        [[nodiscard("raw pointer to underlying data; intended for C API / SIMD interop")]]
        constexpr const T* value_ptr() const noexcept;

        constexpr T& operator[](std::size_t i) noexcept pre(i < static_cast<std::size_t>(L));
        constexpr const T& operator[](std::size_t i) const noexcept pre(i < static_cast<std::size_t>(L));
    protected:
        detail::VectorStorage<L, T, Q> storage_;
    };

    template <int L, detail::Arithmetic T, detail::Qualifier Q>
    requires detail::ValidVecDimension<L>
    constexpr VectorBase<L, T, Q>::VectorBase(T scalar) noexcept {
        storage_.data.fill(scalar);
    }

    template <int L, detail::Arithmetic T, detail::Qualifier Q>
    requires detail::ValidVecDimension<L>
    constexpr T* VectorBase<L, T, Q>::value_ptr() noexcept {
        return storage_.data.data();
    }

    template <int L, detail::Arithmetic T, detail::Qualifier Q>
    requires detail::ValidVecDimension<L>
    constexpr const T* VectorBase<L, T, Q>::value_ptr() const noexcept {
        return storage_.data.data();
    }

    template <int L, detail::Arithmetic T, detail::Qualifier Q>
    requires detail::ValidVecDimension<L>
    constexpr T& VectorBase<L, T, Q>::operator[](std::size_t i) noexcept {

        if consteval {
            if (i >= static_cast<std::size_t>(L)) [[unlikely]] {
                std::abort();
            }
        }

        if (!std::is_constant_evaluated()) {
            if (i >= static_cast<std::size_t>(L)) [[unlikely]] {
                std::abort();
            }
        }
        [[assume(i < static_cast<std::size_t>(L))]];
        return storage_.data[i];
    }

    template <int L, detail::Arithmetic T, detail::Qualifier Q>
    requires detail::ValidVecDimension<L>
    constexpr const T& VectorBase<L, T, Q>::operator[](std::size_t i) const noexcept {

        if consteval {
            if (i >= static_cast<std::size_t>(L)) [[unlikely]] {
                std::abort();
            }
        }
        if (!std::is_constant_evaluated()) {
            if (i >= static_cast<std::size_t>(L)) [[unlikely]] {
                std::abort();
            }
        }
        [[assume(i < static_cast<std::size_t>(L))]];
        return storage_.data[i];
    }
}
