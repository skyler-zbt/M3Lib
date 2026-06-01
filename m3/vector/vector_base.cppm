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

        // Prevent implicit copy-construction from vectors of different dimension
        template<int M>
        requires (M != L)
        VectorBase(const VectorBase<M, T, Q>&) = delete(
            "cannot construct VectorBase from a vector of different dimension; "
            "use explicit element-wise conversion instead");

        // Broadcast construction — fills all L elements with scalar
        constexpr explicit VectorBase(T scalar) noexcept;

        // Raw pointer to underlying data (for interop with C APIs / SIMD)
        [[nodiscard("raw pointer to underlying data; intended for C API / SIMD interop")]]
        constexpr T*       value_ptr()       noexcept;
        [[nodiscard("raw pointer to underlying data; intended for C API / SIMD interop")]]
        constexpr const T* value_ptr() const noexcept;

        // Element access with bounds checking via contracts (P2900R14)
        // In observe/enforce mode, out-of-bounds index triggers a contract violation.
        // In ignore mode, behaviour is identical to std::array::operator[].
        constexpr T&       operator[](std::size_t i)
            pre(i < static_cast<std::size_t>(L));
        constexpr const T& operator[](std::size_t i) const
            pre(i < static_cast<std::size_t>(L));
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
    constexpr T& VectorBase<L, T, Q>::operator[](std::size_t i)
    {
        return storage_.data[i];
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    requires detail::ValidDimension<L>
    constexpr const T& VectorBase<L, T, Q>::operator[](std::size_t i) const
    {
        return storage_.data[i];
    }

} // namespace m3
