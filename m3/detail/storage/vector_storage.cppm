// Aligned storage for vector types.
// Template parameters:
//   L — number of elements (dimension), constrained to [1, 4] by ValidDimension
//   T — element type (typename T; the Arithmetic concept is enforced by
//       VectorBase / Vec, so VectorStorage itself stays minimal)
//   Q — alignment qualifier (controls alignas via AlignmentTraits)
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
} // namespace m3::detail