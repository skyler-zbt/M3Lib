export module m3.detail.storage.vector_storage;

import std;
import m3.detail.qualifier;
import m3.detail.alignment_traits;

export namespace m3::detail {
    template<int L, typename T, Qualifier Q>
    struct alignas(AlignmentTraits<Q>::value) VectorStorage
    {
        std::array<T, L> data{};
        VectorStorage() = default;
    };
} // namespace m3