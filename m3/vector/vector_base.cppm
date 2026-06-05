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
        template<int M>
        requires (M != L)
        VectorBase(const VectorBase<M, T, Q>&) = delete(
            "cannot construct Vec from a vector of different dimension; "
            "use explicit element-wise conversion instead");

        // Broadcast construction — fills all L elements with scalar
        //
        // 广播构造——用标量填充全部 L 个元素
        constexpr explicit VectorBase(T scalar) noexcept;

        // Raw pointer to underlying data (for interop with C APIs / SIMD)
        //
        // 指向底层数据的原始指针（用于与 C API / SIMD 互操作）
        [[nodiscard("raw pointer to underlying data; intended for C API / SIMD interop")]]
        constexpr T*       value_ptr()       noexcept;
        [[nodiscard("raw pointer to underlying data; intended for C API / SIMD interop")]]
        constexpr const T* value_ptr() const noexcept;

        // Element access with bounds checking via contracts (P2900R14)
        // In observe/enforce mode, out-of-bounds index triggers a contract violation.
        // In ignore mode, behaviour is identical to std::array::operator[].
        //
        // 通过契约（P2900R14）进行带边界检查的元素访问。
        // 在 observe/enforce 模式下，越界索引触发契约违规。
        // 在 ignore 模式下，行为与 std::array::operator[] 相同。
        constexpr T&       operator[](std::size_t i) noexcept
            pre(i < static_cast<std::size_t>(L));
        constexpr const T& operator[](std::size_t i) const noexcept
            pre(i < static_cast<std::size_t>(L));
    protected:
        detail::VectorStorage<L, T, Q> storage_;
    };

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    requires detail::ValidDimension<L>
    constexpr VectorBase<L, T, Q>::VectorBase(T scalar) noexcept {
        storage_.data.fill(scalar);
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    requires detail::ValidDimension<L>
    constexpr T* VectorBase<L, T, Q>::value_ptr() noexcept {
        return storage_.data.data();
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    requires detail::ValidDimension<L>
    constexpr const T* VectorBase<L, T, Q>::value_ptr() const noexcept {
        return storage_.data.data();
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    requires detail::ValidDimension<L>
    constexpr T& VectorBase<L, T, Q>::operator[](std::size_t i) noexcept
    {
        return storage_.data[i];
    }

    template<int L, detail::Arithmetic T, detail::Qualifier Q>
    requires detail::ValidDimension<L>
    constexpr const T& VectorBase<L, T, Q>::operator[](std::size_t i) const noexcept
    {
        return storage_.data[i];
    }

} // namespace m3::detail
