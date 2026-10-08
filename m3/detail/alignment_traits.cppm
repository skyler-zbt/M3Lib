
export module m3.detail.alignment_traits;

import std;

import m3.detail.qualifier;

export namespace m3::detail {
template <Qualifier Q, typename T>
struct AlignmentTraits;

template <typename T>
struct AlignmentTraits<Qualifier::aligned_none, T> {
    static constexpr std::size_t value = alignof(T);
};

template <typename T>
struct AlignmentTraits<Qualifier::aligned_low, T> {
    static constexpr std::size_t value = alignof(std::max_align_t);
};

template <typename T>
struct AlignmentTraits<Qualifier::aligned_medium, T> {
    static constexpr std::size_t value = 16;
};

template <typename T>
struct AlignmentTraits<Qualifier::aligned_high, T> {
    static constexpr std::size_t value = 32;
};

}
