export module m3.vector.base;

import std;

import m3.detail;

export namespace m3{

    // Shared base: storage + fundamental operations for all Vec specializations
    template <int L, detail::Arithmetic T, detail::Qualifier Q>
    requires detail::ValidDimension<L>
    struct VectorBase {
        detail::VectorStorage<L, T, Q> storage;

        VectorBase() = default;

        explicit constexpr VectorBase(const T& v) : storage(v) {}

        constexpr T* value_ptr() noexcept;
        constexpr const T* value_ptr() const noexcept;
    };

    // VectorBase out-of-line definitions
    template<int L, detail::Arithmetic T, detail::Qualifier Q> requires detail::ValidDimension<L>
    constexpr T* VectorBase<L, T, Q>::value_ptr() noexcept {
        return storage.data.data();
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q> requires detail::ValidDimension<L>
    constexpr const T* VectorBase<L, T, Q>::value_ptr() const noexcept {
        return storage.data.data();
    }
}
