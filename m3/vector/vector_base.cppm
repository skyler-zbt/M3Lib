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
    // The contract pre(i < L) is the primary safety net.  This
    // if-consteval guard provides a secondary gate for constant-
    // evaluation contexts where contracts may not be enabled:
    // std::abort() is not a constant expression, so a compile-time
    // out-of-bounds access is diagnosed as a hard error.
    //
    // At runtime:
    // 1. [[assume(i < L)]] tells the compiler the index is always
    //    in bounds.  In optimised builds this allows elimination of
    //    the runtime guard below as dead code, unlocking auto-
    //    vectorisation.
    // 2. The runtime if-abort guard is kept for debug / CI builds
    //    where contracts may not terminate correctly (e.g. UBSan
    //    interference) and optimisations are off.
    //
    // 编译期纵深防御。
    // 契约 pre(i < L) 是主要安全网。此 if-consteval 守卫为
    // 常量求值上下文提供二级保护：std::abort() 不是常量表达式，
    // 编译期越界访问会被诊断为硬错误。
    //
    // 运行期：
    // 1. [[assume(i < L)]] 告诉编译器索引始终在边界内。
    //    优化构建中编译器可将下方运行时守卫作为死代码消除，
    //    解锁自动向量化。
    // 2. 运行时 if-abort 守卫保留用于 debug/CI 构建，以应对
    //    契约未正确终止（如 UBSan 干扰）且优化未启用的场景。
    if consteval {
        if (i >= static_cast<std::size_t>(L)) [[unlikely]] {
            std::abort();
        }
    }
    [[assume(i < static_cast<std::size_t>(L))]];
    if (i >= static_cast<std::size_t>(L)) [[unlikely]] {
        std::abort();
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
    if (i >= static_cast<std::size_t>(L)) [[unlikely]] {
        std::abort();
    }
    return storage_.data[i];
}

}  // namespace m3::detail
