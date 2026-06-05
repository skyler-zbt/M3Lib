// Memory-alignment qualifier for vector and matrix storage.
// Controls the alignas specifier on the underlying storage type:
//   aligned_none   — natural alignment of T (no forced alignment; default)
//   aligned_low    — alignof(std::max_align_t) (platform default)
//   aligned_medium — 16-byte alignment (SSE / AVX friendly)
//   aligned_high   — 32-byte alignment (AVX-512 friendly)
//
// 向量和矩阵存储的内存对齐修饰符。
// 控制底层存储类型的 alignas 说明符：
//   aligned_none   —— T 的自然对齐（不强制对齐；默认）
//   aligned_low    —— alignof(std::max_align_t)（平台默认）
//   aligned_medium —— 16 字节对齐（兼容 SSE / AVX）
//   aligned_high   —— 32 字节对齐（兼容 AVX-512）
export module m3.detail.qualifier;

export namespace m3::detail {
enum class Qualifier { aligned_none, aligned_low, aligned_medium, aligned_high };
}
