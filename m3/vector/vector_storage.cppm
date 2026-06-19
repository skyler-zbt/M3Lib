// Aligned storage for vector types.
// Template parameters:
//   L — number of elements (dimension), constrained to [1, 4] by ValidDimension
//   T — element type (typename T; the Arithmetic concept is enforced by
//       VectorBase / Vec, so VectorStorage itself stays minimal)
//   Q — alignment qualifier (controls alignas via AlignmentTraits)
//
// Lives in m3.vector (not m3.detail.storage) to avoid the module
// dependency cycle that would otherwise form: m3.detail.concepts requires
// matrix_storage to expose column_type = Vec<R, T, Q>, which in turn
// requires importing m3.vector.vec, which depends on m3.detail.* via the
// m3.vector → m3.vector.base → m3.detail chain.  Co-locating storage
// with its owning type breaks the cycle at the storage layer.
//
// 向量类型的对齐存储。
// 模板参数：
//   L —— 元素数量（维度），由 ValidDimension 约束为 [1, 4]
//   T —— 元素类型（typename T；Arithmetic 概念由 VectorBase / Vec 强制，
//        VectorStorage 本身保持最小化）
//   Q —— 对齐修饰符（通过 AlignmentTraits 控制 alignas）
//
// 位于 m3.vector（而非 m3.detail.storage）以避免模块依赖循环：
// m3.detail.concepts 要求 matrix_storage 暴露 column_type = Vec<R,T,Q>，
// 后者需要 import m3.vector.vec，而 m3.vector.vec 通过
// m3.vector → m3.vector.base → m3.detail 链依赖 m3.detail.*。
// 将 storage 与其所属类型同目录可在存储层打破循环。
export module m3.vector.storage;

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
