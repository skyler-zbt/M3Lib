export module m3.detail.alignment_traits;

import m3.detail.qualifier;

export namespace m3 {
    template<Qualifier Q>
    struct AlignmentTraits;

    template<>
    struct AlignmentTraits<Qualifier::aligned_lowp> {
        static constexpr std::size_t value = alignof(std::max_align_t);
    };

    template<>
    struct AlignmentTraits<Qualifier::aligned_mediump> {
        static constexpr std::size_t value = 16;
    };

    template<>
    struct AlignmentTraits<Qualifier::aligned_highp> {
        static constexpr std::size_t value = 32;
    };

}