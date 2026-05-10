export module m3.vector.specialized;

import std;
import m3.detail;
import m3.vector.basic;

export namespace m3 {
    // specialized  vec1
    template <Arithmetic T, qualifier Q>
    struct vec<1,T,Q> {
        T x;
    };
}