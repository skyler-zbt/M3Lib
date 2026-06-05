// Aligned storage for vector types.
// Template parameters:
//   L — number of elements (dimension), constrained to [1, 4] by ValidDimension
//   T — element type (typename T; the Arithmetic concept is enforced by
//       VectorBase / Vec, so VectorStorage itself stays minimal)
//   Q — alignment qualifier (controls alignas via AlignmentTraits)
//
// 向量类型的对齐存储。
// 模板参数：
//   L —— 元素数量（维度），由 ValidDimension 约束为 [1, 4]
//   T —— 元素类型（typename T；Arithmetic 概念由 VectorBase / Vec 强制，
//        VectorStorage 本身保持最小化）
//   Q —— 对齐修饰符（通过 AlignmentTraits 控制 alignas）
export module m3.detail.storage.vector_storage;

import std;
import m3.detail.qualifier;
import m3.detail.alignment_traits;

export namespace m3::detail {
template <int L, typename T, Qualifier Q>
struct alignas(AlignmentTraits<Q, T>::value) VectorStorage {
    std::array<T, L> data{};
    VectorStorage() = default;
};
}  // namespace m3::detail
