// VectorBase — CRTP-free base class for vector types.
// Provides aligned storage, element access (operator[], value_ptr),
// and scalar broadcast construction.
export module m3.vector.base;

import std;

import m3.detail;

export namespace m3 {

    template <int L, detail::Arithmetic T, detail::Qualifier Q>
    requires detail::ValidDimension<L>
    class VectorBase {
    public:
        VectorBase() = default;
        VectorBase(const VectorBase&) = default;

        // Broadcast construction — fills all L elements with scalar
        constexpr explicit VectorBase(T scalar) noexcept;

        // Raw pointer to underlying data (for interop with C APIs / SIMD)
        [[nodiscard]] constexpr T*       value_ptr()       noexcept;
        [[nodiscard]] constexpr const T* value_ptr() const noexcept;

        // Unchecked element access (matches GLSL / std::array behavior)
        constexpr T&       operator[](std::size_t i);
        constexpr const T& operator[](std::size_t i) const;
    protected:
        detail::VectorStorage<L, T, Q> storage_;
    };

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    requires detail::ValidDimension<L>
    constexpr VectorBase<L, T, Q>::VectorBase(T scalar) noexcept {
        storage_.data.fill(scalar);
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    requires detail::ValidDimension<L>
    constexpr T* VectorBase<L, T, Q>::value_ptr() noexcept {
        return storage_.data.data();
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    requires detail::ValidDimension<L>
    constexpr const T* VectorBase<L, T, Q>::value_ptr() const noexcept {
        return storage_.data.data();
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    requires detail::ValidDimension<L>
    constexpr T& VectorBase<L, T, Q>::operator[](std::size_t i) {
        return storage_.data[i];
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    requires detail::ValidDimension<L>
    constexpr const T& VectorBase<L, T, Q>::operator[](std::size_t i) const {
        return storage_.data[i];
    }

} // namespace m3
