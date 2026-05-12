export module m3.vector.basic;

import std;
import m3.detail;

export namespace m3{

    // TODO: move concept to detail modules and create concept submodule
    template <typename T>
    concept Arithmetic = std::is_arithmetic_v<T>;

    template <int L>
    concept ValidDimension = (L >= 1 && L<=4);

    // basic vector template
    template <int L, Arithmetic T, qualifier Q = qualifier::aligned_high>
    requires ValidDimension<L>
    struct vec;
}