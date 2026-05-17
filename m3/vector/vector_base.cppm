export module m3.vector.base;

import std;

import m3.detail;

export namespace m3{

    // Shared base: storage + fundamental operations for all Vec specializations
    template <int L, detail::Arithmetic T, detail::Qualifier Q>
    requires detail::ValidDimension<L>
    class VectorBase {
    public:
        VectorBase() = default;
        constexpr VectorBase(const VectorBase&) = default;
        constexpr explicit VectorBase(T scalar) noexcept;

        constexpr T* value_ptr() noexcept;
        constexpr const T* value_ptr() const noexcept;

        constexpr T& operator[](std::size_t i);
        constexpr const T& operator[](std::size_t i) const;
    protected:
        detail::VectorStorage<L, T, Q> storage_;
    };

    template<int L, detail::Arithmetic T, detail::Qualifier Q> requires detail::ValidDimension<L>
    constexpr VectorBase<L, T, Q>::VectorBase(T scalar) noexcept {
        storage_.data.fill(scalar);
    }

    // VectorBase out-of-line definitions
    template<int L, detail::Arithmetic T, detail::Qualifier Q> requires detail::ValidDimension<L>
    constexpr T* VectorBase<L, T, Q>::value_ptr() noexcept {
        return storage_.data.data();
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q> requires detail::ValidDimension<L>
    constexpr const T* VectorBase<L, T, Q>::value_ptr() const noexcept {
        return storage_.data.data();
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q> requires detail::ValidDimension<L>
    constexpr T & VectorBase<L, T, Q>::operator[](std::size_t i) {
        return storage_.data[i];
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q> requires detail::ValidDimension<L>
    constexpr const T & VectorBase<L, T, Q>::operator[](std::size_t i) const {
        return storage_.data[i];
    }
}
