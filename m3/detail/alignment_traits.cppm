// Compile-time alignment values mapped from Qualifier and element type.
// Used as alignas(AlignmentTraits<Q, T>::value) in storage types.
// aligned_none uses natural alignment of T (no forced alignas overhead).
//
// 编译期对齐值，由 Qualifier 和元素类型映射得出。
// 用作 alignas(AlignmentTraits<Q, T>::value) 修饰存储类型。
// aligned_none 使用 T 的自然对齐，不强制 alignas 开销。
export module m3.detail.alignment_traits;

import std;

import m3.detail.qualifier;

export namespace m3::detail {
template <Qualifier Q, typename T>
struct AlignmentTraits;

// Natural alignment of the element type — no forced padding.
// Vec3<float> = 12 B, Vec4<float> = 16 B, matching GPU buffer expectations.
//
// 元素类型的自然对齐——不强制填充。
// Vec3<float> = 12 B，Vec4<float> = 16 B，与 GPU 缓冲区期望一致。
template <typename T>
struct AlignmentTraits<Qualifier::aligned_none, T> {
    static constexpr std::size_t value = alignof(T);
};

// Platform-default alignment (e.g. 8 or 16 bytes depending on ABI)
//
// 平台默认对齐（如 8 或 16 字节，取决于 ABI）
template <typename T>
struct AlignmentTraits<Qualifier::aligned_low, T> {
    static constexpr std::size_t value = alignof(std::max_align_t);
};

// 16-byte — SSE / AVX 128-bit friendly
//
// 16 字节对齐——兼容 SSE / AVX 128 位
template <typename T>
struct AlignmentTraits<Qualifier::aligned_medium, T> {
    static constexpr std::size_t value = 16;
};

// 32-byte — AVX-512 256-bit friendly
//
// 32 字节对齐——兼容 AVX-512 256 位
template <typename T>
struct AlignmentTraits<Qualifier::aligned_high, T> {
    static constexpr std::size_t value = 32;
};

}  // namespace m3::detail
