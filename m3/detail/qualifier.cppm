// Memory-alignment qualifier for vector and matrix storage.
// Controls the alignas specifier on the underlying storage type:
//   aligned_high   — 32-byte alignment (AVX-512 friendly)
//   aligned_medium — 16-byte alignment (SSE / AVX friendly)
//   aligned_low    — alignof(std::max_align_t) (platform default)
export module m3.detail.qualifier;

export namespace m3::detail {
    enum class Qualifier {
        aligned_high,
        aligned_medium,
        aligned_low
    };
}