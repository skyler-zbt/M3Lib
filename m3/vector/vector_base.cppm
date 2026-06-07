// VectorBase — CRTP-free base class for vector types.
// Provides aligned storage, element access (operator[], value_ptr),
// and scalar broadcast construction.
//
// VectorBase —— 无 CRTP 的向量类型基类。
// 提供对齐存储、元素访问（operator[]、value_ptr）和标量广播构造。
export module m3.vector.base;

import std;

import m3.detail;

export namespace m3::detail {

template <int L, detail::Arithmetic T, detail::Qualifier Q>
requires detail::ValidDimension<L>
class VectorBase {
public:
    VectorBase() = default;
    VectorBase(const VectorBase&) = default;

    // Prevent implicit copy-construction from vectors of different dimension
    //
    // 禁止从不同维度的向量隐式拷贝构造
    template <int M>
    requires(M != L)
    VectorBase(const VectorBase<M, T, Q>&) =
        delete ("cannot construct Vec from a vector of different dimension; "
                "use explicit element-wise conversion instead");

    // Broadcast construction — fills all L elements with scalar
    //
    // 广播构造——用标量填充全部 L 个元素
    constexpr explicit VectorBase(T scalar) noexcept;

    // Raw pointer to underlying data (for interop with C APIs / SIMD)
    //
    // 指向底层数据的原始指针（用于与 C API / SIMD 互操作）
    [[nodiscard("raw pointer to underlying data; intended for C API / SIMD interop")]]
    constexpr T* value_ptr() noexcept;
    [[nodiscard("raw pointer to underlying data; intended for C API / SIMD interop")]]
    constexpr const T* value_ptr() const noexcept;

    // Element access with bounds checking via contracts (P2900R14).
    // At compile time (if consteval), an out-of-bounds index is caught via
    // std::abort() which is not a constant expression → hard compile error.
    // At runtime, [[assume(i < L)]] tells the compiler the index is always
    // valid, unlocking auto-vectorisation in hot loops.
    //
    // 通过契约（P2900R14）进行带边界检查的元素访问。
    // 编译期（if consteval）越界索引通过 std::abort() 捕获——非 constexpr
    // 调用产生硬编译错误。
    // 运行期 [[assume(i < L)]] 告诉编译器索引始终有效，解锁热循环中的
    // 自动向量化。
    constexpr T& operator[](std::size_t i) noexcept pre(i < static_cast<std::size_t>(L));
    constexpr const T& operator[](std::size_t i) const noexcept
        pre(i < static_cast<std::size_t>(L));
protected:
    detail::VectorStorage<L, T, Q> storage_;
};

template <int L, detail::Arithmetic T, detail::Qualifier Q>
requires detail::ValidDimension<L>
constexpr VectorBase<L, T, Q>::VectorBase(T scalar) noexcept {
    storage_.data.fill(scalar);
}

template <int L, detail::Arithmetic T, detail::Qualifier Q>
requires detail::ValidDimension<L>
constexpr T* VectorBase<L, T, Q>::value_ptr() noexcept {
    return storage_.data.data();
}

template <int L, detail::Arithmetic T, detail::Qualifier Q>
requires detail::ValidDimension<L>
constexpr const T* VectorBase<L, T, Q>::value_ptr() const noexcept {
    return storage_.data.data();
}

template <int L, detail::Arithmetic T, detail::Qualifier Q>
requires detail::ValidDimension<L>
constexpr T& VectorBase<L, T, Q>::operator[](std::size_t i) noexcept {
    // Compile-time defense-in-depth.
    // std::abort() is not a constant expression, so a compile-time
    // out-of-bounds access is diagnosed as a hard error.
    //
    // At runtime:
    // 1. [[assume(i < L)]] tells the compiler the index is always
    //    in bounds, unlocking auto-vectorisation in hot loops.
    // 2. The volatile sink prevents the compiler from eliminating
    //    the runtime guard as dead code after [[assume]], keeping
    //    the OOB → abort path alive in debug and CI builds.
    //    In release builds the compare-and-branch is still
    //    optimised away because [[assume]] dominates.
    //
    // 编译期：std::abort() 不是常量表达式，越界触发硬编译错误。
    //
    // 运行期：
    // 1. [[assume(i < L)]] 告诉编译器索引始终在边界内，
    //    解锁热循环的自动向量化。
    // 2. volatile 屏障阻止编译器在 [[assume]] 后将运行时守卫
    //    当作死代码消除，保证 debug/CI 中 OOB → abort 路径存活。
    //    release 构建中 [[assume]] 仍主导，比较分支被优化消除。
    if consteval {
        if (i >= static_cast<std::size_t>(L)) [[unlikely]] {
            std::abort();
        }
    }
    [[assume(i < static_cast<std::size_t>(L))]];

    // Volatile sink: prevents the compiler from eliminating the
    // runtime guard as dead code after [[assume]].  Guarded by
    // !std::is_constant_evaluated() because volatile writes are
    // not permitted in constexpr functions.
    // At compile time the if-consteval block above handles OOB.
    //
    // volatile 屏障：阻止编译器将运行时守卫作为死代码消除。
    // 由 !std::is_constant_evaluated() 保护，因为 volatile 写
    // 在 constexpr 函数中不允许。
    // 编译期 OOB 由上方的 if-consteval 块处理。
    if (!std::is_constant_evaluated()) {
        volatile std::size_t sink = i;
        (void)sink;

        if (i >= static_cast<std::size_t>(L)) [[unlikely]] {
            std::abort();
        }
    }
    return storage_.data[i];
}

template <int L, detail::Arithmetic T, detail::Qualifier Q>
requires detail::ValidDimension<L>
constexpr const T& VectorBase<L, T, Q>::operator[](std::size_t i) const noexcept {
    // Const overload of the if-consteval + [[assume]] pattern.
    // At compile time, out-of-bounds access is diagnosed as a hard
    // error via std::abort() (not a constant expression).
    // At runtime, [[assume(i < L)]] enables the compiler to eliminate
    // the runtime guard below in optimised builds, while the guard
    // is retained for debug/CI where contracts or UBSan may interfere.
    // The contract pre(i < L) is the primary safety net.
    //
    // if-consteval + [[assume]] 模式的 const 重载。
    // 编译期越界通过 std::abort()（非常量表达式）诊断。
    // 运行期 [[assume(i < L)]] 使编译器在优化构建中消除下方
    // 运行时守卫，而 debug/CI 中保留守卫应对契约或 UBSan 干扰。
    // 契约 pre(i < L) 是主要安全网。
    if consteval {
        if (i >= static_cast<std::size_t>(L)) [[unlikely]] {
            std::abort();
        }
    }
    [[assume(i < static_cast<std::size_t>(L))]];

    // Volatile sink (see mutable overload for rationale).
    // Guarded by !std::is_constant_evaluated().
    //
    // volatile 屏障（原理见可变重载）。
    // 由 !std::is_constant_evaluated() 保护。
    if (!std::is_constant_evaluated()) {
        volatile std::size_t sink = i;
        (void)sink;

        if (i >= static_cast<std::size_t>(L)) [[unlikely]] {
            std::abort();
        }
    }
    return storage_.data[i];
}

}  // namespace m3::detail
