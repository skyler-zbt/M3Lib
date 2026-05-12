export module m3.vector.vec;

import std;

import m3.detail;
import m3.vector.base;

export namespace m3 {
    // Primary template: inherits VectorBase, serves as the unified type for operator overloading
    template <int L, Arithmetic T, Qualifier Q = Qualifier::aligned_high>
    requires ValidDimension<L>
    struct Vec : VectorBase<L, T, Q> {
        Vec() = default;
        explicit constexpr Vec(const T& v) : VectorBase<L, T, Q>(v) {}
    };
}