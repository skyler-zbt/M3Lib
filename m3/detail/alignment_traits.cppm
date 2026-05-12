export module m3.detail.alignment_traits;

import std;

import m3.detail.qualifier;

export namespace m3 {
    template<Qualifier Q>
    struct AlignmentTraits;

    template<>
    struct AlignmentTraits<Qualifier::aligned_low> {
        static constexpr std::size_t value = alignof(std::max_align_t);
    };

    template<>
    struct AlignmentTraits<Qualifier::aligned_medium> {
        static constexpr std::size_t value = 16;
    };

    template<>
    struct AlignmentTraits<Qualifier::aligned_high> {
        static constexpr std::size_t value = 32;
    };

}