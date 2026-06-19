// Aligned storage for matrix types.
// Template parameters:
//   C — number of columns (constrained to [2, 4] by ValidMatrixSize)
//   R — number of rows (constrained to [2, 4] by ValidMatrixSize)
//   T — element type (Arithmetic concept enforced by Mat, not by storage)
//   Q — alignment qualifier (controls alignas via AlignmentTraits)
//
// Layout: NESTED column-major, std::array<Vec<R, T, Q>, C>.  Each entry
// columns_[i] is a full Vec<R, T, Q> — zero reinterpret_cast for
// Mat::operator[], which returns Vec<R,T,Q>& directly from columns_[i]
// (TD-002 reference design).
//
// The Vec inner alignment padding (per Q) is preserved per-column.
// Future v0.5.0 may evaluate flat layout for SIMD register views; for
// v0.2.0 the nested layout is the simpler and faster choice on hot paths
// (compiler can elide the Vec wrapping in release builds).
//
// Lives in m3.matrix (not m3.detail.storage) to break the cycle that
// would otherwise form:
//   m3.detail → m3.vector → m3.detail.storage.matrix_storage → m3.detail
// Co-locating storage with its owning type (m3.matrix) means the import
// direction is m3.matrix.storage → m3.vector.vec (one-way), no cycle.
//
// 矩阵类型的对齐存储。
// 模板参数：
//   C —— 列数（由 ValidMatrixSize 约束为 [2, 4]）
//   R —— 行数（由 ValidMatrixSize 约束为 [2, 4]）
//   T —— 元素类型（Arithmetic 概念由 Mat 强制，存储本身保持最小化）
//   Q —— 对齐修饰符（通过 AlignmentTraits 控制 alignas）
//
// 布局：嵌套列主序，std::array<Vec<R, T, Q>, C>。每项 columns_[i] 是
// 完整的 Vec<R, T, Q>——Mat::operator[] 直接从 columns_[i] 返回
// Vec<R,T,Q>& 零 reinterpret_cast（TD-002 参考设计）。
//
// Vec 内部的对齐填充（按 Q）保留在每列内。
// 未来 v0.5.0 可能评估扁平布局以支持 SIMD 寄存器视图；v0.2.0 嵌套
// 布局是热路径上更简单更快的选择（编译器在 release 构建中可消除
// Vec 包装开销）。
//
// 位于 m3.matrix（而非 m3.detail.storage）以打破循环：
//   m3.detail → m3.vector → m3.detail.storage.matrix_storage → m3.detail
// 将 storage 与其所属类型（m3.matrix）同目录使导入方向为单向
// m3.matrix.storage → m3.vector.vec，无循环。
export module m3.matrix.storage;

import std;

import m3.detail.qualifier;
import m3.detail.alignment_traits;
import m3.vector.vec;

export namespace m3::detail {

template <int C, int R, typename T, Qualifier Q>
struct alignas(AlignmentTraits<Q, T>::value) MatrixStorage {
    std::array<Vec<R, T, Q>, C> columns_{};
    MatrixStorage() = default;
};

}  // namespace m3::detail