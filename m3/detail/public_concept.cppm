export module m3.detail.public_concepts;

import std;

export namespace m3 {
    template <typename T>
    concept Arithmetic = std::is_arithmetic_v<T>;

    template <int L>
    concept ValidDimension = (L >= 1 && L<=4);
}