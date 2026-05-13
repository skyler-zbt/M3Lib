export module m3.vector.vec;

import std;

import m3.detail;
import m3.vector.base;

export namespace m3 {
    // Primary template: inherits VectorBase, serves as the unified type for operator overloading
    template <int L, detail::Arithmetic T, detail::Qualifier Q = detail::Qualifier::aligned_high>
    requires detail::ValidDimension<L>
    struct Vec : VectorBase<L, T, Q> {
        Vec() = default;
        explicit constexpr Vec(const T& v) : VectorBase<L, T, Q>(v) {}
    };
}