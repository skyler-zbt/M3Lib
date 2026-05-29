// Compile-time alignment values mapped from Qualifier.
// Used as alignas(AlignmentTraits<Q>::value) in storage types.
export module m3.detail.alignment_traits;

import std;

import m3.detail.qualifier;

export namespace m3::detail {
    template<Qualifier Q>
    struct AlignmentTraits;

    // Platform-default alignment (e.g. 8 or 16 bytes depending on ABI)
    template<>
    struct AlignmentTraits<Qualifier::aligned_low> {
        static constexpr std::size_t value = alignof(std::max_align_t);
    };

    // 16-byte — SSE / AVX 128-bit friendly
    template<>
    struct AlignmentTraits<Qualifier::aligned_medium> {
        static constexpr std::size_t value = 16;
    };

    // 32-byte — AVX-512 256-bit friendly
    template<>
    struct AlignmentTraits<Qualifier::aligned_high> {
        static constexpr std::size_t value = 32;
    };

}