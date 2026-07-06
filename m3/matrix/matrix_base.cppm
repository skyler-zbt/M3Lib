// MatrixBase — independent base class for Mat (no inheritance from VectorBase).
// Provides column access (operator[]), element access (operator()), value_ptr,
// and construction from broadcast scalar or raw pointer.
//
// Per TD-008: Mat does NOT inherit from VectorBase.  The two hierarchies
// are parallel because Mat has different semantics:
//   - Vec::operator[] returns T& (a scalar element)
//   - Mat::operator[] returns Vec<R,T,Q>& (a column reference)
// Treating Mat as a Vector would conflate dimension semantics.
//
// S1.1+v0.2-rc1: storage is NESTED column-major (std::array<Vec<R,T,Q>, C>)
// per TD-002 reference design.  operator[] returns Vec<R,T,Q>& directly
// from storage_.columns_[i] — zero reinterpret_cast, zero temporary.
// This was previously impossible due to the module cycle
//   m3.detail → m3.vector → m3.detail.storage.matrix_storage → m3.detail
// which forced a flat-storage workaround in S1.  Moving matrix_storage
// out of m3.detail.storage into m3.matrix.storage breaks the cycle:
//   m3.matrix.storage → m3.vector.vec   (one direction)
//
// The operator[] follows the same defence-in-depth pattern as VectorBase:
//   1. contract `pre(i < C)` — declares the precondition
//   2. `if consteval { std::abort(); }` — compile-time OOB → hard error
//   3. Runtime guard — debug/CI OOB → SIGABRT
//   4. `[[assume(i < C)]]` — release auto-vectorisation unlock
//
// MatrixBase —— Mat 的独立基类（不继承自 VectorBase）。
// 提供列访问（operator[]）、元素访问（operator()）、value_ptr、
// 以及从广播标量或原始指针的构造。
//
// 按 TD-008：Mat 不继承自 VectorBase。两个层次结构是平行的，
// 因为 Mat 有不同的语义：
//   - Vec::operator[] 返回 T&（标量元素）
//   - Mat::operator[] 返回 Vec<R,T,Q>&（列引用）
// 将 Mat 视为 Vector 会混淆维度语义。
//
// S1.1+v0.2-rc1：存储为嵌套列主序（std::array<Vec<R,T,Q>, C>），
// 按 TD-002 参考设计。operator[] 直接从 storage_.columns_[i] 返回
// Vec<R,T,Q>&——零 reinterpret_cast，零临时对象。
// 这在之前因模块循环
//   m3.detail → m3.vector → m3.detail.storage.matrix_storage → m3.detail
// 不可能实现，强制 S1 用扁平存储绕过。把 matrix_storage 从
// m3.detail.storage 移到 m3.matrix.storage 打破了循环：
//   m3.matrix.storage → m3.vector.vec   （单向）
//
// operator[] 采用与 VectorBase 相同的纵深防御模式：
//   1. 契约 `pre(i < C)` —— 声明前置条件
//   2. `if consteval { std::abort(); }` —— 编译期越界 → 硬错误
//   3. 运行时守卫 —— debug/CI 越界 → SIGABRT
//   4. `[[assume(i < C)]]` —— release 自动向量化解锁
export module m3.matrix.base;

import std;

import m3.detail;
import m3.matrix.storage;
import m3.vector.vec;

export namespace m3::detail {

template <int C, int R, typename T, Qualifier Q>
requires detail::ValidMatrixSize<C, R>
class MatrixBase {
public:
    MatrixBase() = default;
    MatrixBase(const MatrixBase&) = default;

    // Broadcast construction — fills diagonal with scalar (identity-like),
    // off-diagonal with zero.  Matches Mat(T scalar) semantics so that
    // Mat<3,3,float> M{1.0f} yields the 3×3 identity matrix.
    //
    // 广播构造——对角线填 scalar（类单位），非对角线填零。
    // 与 Mat(T scalar) 语义一致：Mat<3,3,float> M{1.0f} 产生 3×3 单位矩阵。
    constexpr explicit MatrixBase(T scalar) noexcept;

    // Raw pointer construction — copies R*C elements column-major from ptr.
    // For interop with C APIs / glUniformMatrix*.
    //
    // 原始指针构造——从 ptr 以列主序拷贝 R*C 个元素。
    // 用于 C API / glUniformMatrix* 互操作。
    explicit MatrixBase(const T* ptr) noexcept;

    // Column access (primary).  Returns Vec<R,T,Q>& — zero reinterpret_cast,
    // zero temporary, zero per-access cost.  Storage is nested
    // (std::array<Vec<R,T,Q>, C>) so the column is a true reference into
    // storage_.columns_[i], not a value copy.
    //
    // 列访问（主要）。返回 Vec<R,T,Q>&——零 reinterpret_cast，零临时，
    // 零每次访问开销。存储是嵌套的（std::array<Vec<R,T,Q>, C>），
    // 所以列是对 storage_.columns_[i] 的真正引用，不是值拷贝。
    constexpr Vec<R, T, Q>& operator[](std::size_t i) noexcept pre(i < static_cast<std::size_t>(C));
    constexpr const Vec<R, T, Q>& operator[](std::size_t i) const noexcept
        pre(i < static_cast<std::size_t>(C));

    // Element access — two-parameter function-call syntax to avoid
    // ambiguity with the column accessor.  Accesses element at column c,
    // row r.  Returns T& for direct element manipulation.
    //
    // 元素访问——双参数函数调用语法以避免与列访问器歧义。
    // 访问列 c、行 r 处的元素。返回 T& 以便直接操作元素。
    constexpr T& operator()(std::size_t c, std::size_t r) noexcept
        pre(c < static_cast<std::size_t>(C) && r < static_cast<std::size_t>(R));
    constexpr const T& operator()(std::size_t c, std::size_t r) const noexcept
        pre(c < static_cast<std::size_t>(C) && r < static_cast<std::size_t>(R));

    // Raw pointer to underlying data of the first column (column 0).
    // Use case: C-API interop when the consumer knows the column-major
    // layout and processes C * R contiguous elements.  Note: column
    // padding (per Q alignment) is NOT included — the returned pointer
    // covers exactly the elements of column 0.
    //
    // 指向第一列底层数据的原始指针。
    // 用途：消费者已知列主序布局并处理 C * R 个连续元素时的 C API 互操作。
    // 注意：不包含列填充（按 Q 对齐）——返回的指针恰好覆盖第 0 列的元素。
    [[nodiscard("raw pointer to underlying data; intended for C API / SIMD interop")]]
    constexpr T* value_ptr() noexcept;
    [[nodiscard("raw pointer to underlying data; intended for C API / SIMD interop")]]
    constexpr const T* value_ptr() const noexcept;
protected:
    detail::MatrixStorage<C, R, T, Q> storage_;
};

// --- Out-of-class member definitions ---

template <int C, int R, typename T, Qualifier Q>
requires detail::ValidMatrixSize<C, R>
constexpr MatrixBase<C, R, T, Q>::MatrixBase(T scalar) noexcept {
    for (std::size_t c = 0; c < static_cast<std::size_t>(C); ++c) {
        for (std::size_t r = 0; r < static_cast<std::size_t>(R); ++r) {
            storage_.columns_[c][r] = (c == r) ? scalar : T{0};
        }
    }
}

template <int C, int R, typename T, Qualifier Q>
requires detail::ValidMatrixSize<C, R>
MatrixBase<C, R, T, Q>::MatrixBase(const T* ptr) noexcept {
    // Column-major copy: ptr[0..R-1] is column 0, ptr[R..2R-1] is column 1, etc.
    //
    // 列主序拷贝：ptr[0..R-1] 是第 0 列，ptr[R..2R-1] 是第 1 列，依此类推。
    for (std::size_t c = 0; c < static_cast<std::size_t>(C); ++c) {
        for (std::size_t r = 0; r < static_cast<std::size_t>(R); ++r) {
            storage_.columns_[c][r] = ptr[c * static_cast<std::size_t>(R) + r];
        }
    }
}

template <int C, int R, typename T, Qualifier Q>
requires detail::ValidMatrixSize<C, R>
constexpr Vec<R, T, Q>& MatrixBase<C, R, T, Q>::operator[](std::size_t i) noexcept {
    // Compile-time defence-in-depth.
    //
    // 编译期纵深防御。
    if consteval {
        if (i >= static_cast<std::size_t>(C)) [[unlikely]] {
            std::abort();
        }
    }
    // Runtime defence-in-depth (see VectorBase::operator[] rationale).
    //
    // 运行时纵深防御（理由见 VectorBase::operator[]）。
    if (!std::is_constant_evaluated()) {
        if (i >= static_cast<std::size_t>(C)) [[unlikely]] {
            std::abort();
        }
    }
    [[assume(i < static_cast<std::size_t>(C))]];
    return storage_.columns_[i];
}

template <int C, int R, typename T, Qualifier Q>
requires detail::ValidMatrixSize<C, R>
constexpr const Vec<R, T, Q>& MatrixBase<C, R, T, Q>::operator[](std::size_t i) const noexcept {
    if consteval {
        if (i >= static_cast<std::size_t>(C)) [[unlikely]] {
            std::abort();
        }
    }
    if (!std::is_constant_evaluated()) {
        if (i >= static_cast<std::size_t>(C)) [[unlikely]] {
            std::abort();
        }
    }
    [[assume(i < static_cast<std::size_t>(C))]];
    return storage_.columns_[i];
}

template <int C, int R, typename T, Qualifier Q>
requires detail::ValidMatrixSize<C, R>
constexpr T* MatrixBase<C, R, T, Q>::value_ptr() noexcept {
    // Return a pointer to the first column's data.  Note: with nested
    // storage this is NOT contiguous across all C columns — column 1 may
    // have Q-driven padding.  C API consumers must process column by
    // column (call value_ptr() on each column's Vec directly when needed).
    //
    // 返回指向第 0 列数据的指针。注意：嵌套存储下，所有 C 列之间
    // 不一定连续——第 1 列可能有 Q 驱动的填充。C API 消费者必须
    // 逐列处理（需要时直接对每列的 Vec 调用 value_ptr()）。
    return storage_.columns_[0].value_ptr();
}

template <int C, int R, typename T, Qualifier Q>
requires detail::ValidMatrixSize<C, R>
constexpr const T* MatrixBase<C, R, T, Q>::value_ptr() const noexcept {
    return storage_.columns_[0].value_ptr();
}

template <int C, int R, typename T, Qualifier Q>
requires detail::ValidMatrixSize<C, R>
constexpr T& MatrixBase<C, R, T, Q>::operator()(std::size_t c, std::size_t r) noexcept {
    if consteval {
        if (c >= static_cast<std::size_t>(C) || r >= static_cast<std::size_t>(R)) [[unlikely]] {
            std::abort();
        }
    }
    if (!std::is_constant_evaluated()) {
        if (c >= static_cast<std::size_t>(C) || r >= static_cast<std::size_t>(R)) [[unlikely]] {
            std::abort();
        }
    }
    [[assume(c < static_cast<std::size_t>(C) && r < static_cast<std::size_t>(R))]];
    return storage_.columns_[c][r];
}

template <int C, int R, typename T, Qualifier Q>
requires detail::ValidMatrixSize<C, R>
constexpr const T& MatrixBase<C, R, T, Q>::operator()(std::size_t c, std::size_t r) const noexcept {
    if consteval {
        if (c >= static_cast<std::size_t>(C) || r >= static_cast<std::size_t>(R)) [[unlikely]] {
            std::abort();
        }
    }
    if (!std::is_constant_evaluated()) {
        if (c >= static_cast<std::size_t>(C) || r >= static_cast<std::size_t>(R)) [[unlikely]] {
            std::abort();
        }
    }
    [[assume(c < static_cast<std::size_t>(C) && r < static_cast<std::size_t>(R))]];
    return storage_.columns_[c][r];
}

}  // namespace m3::detail