// Element-wise arithmetic functors.
// Each is a stateless callable used by apply_binary / apply_unary.
// Functors (rather than lambdas) are used because they can be passed as
// non-type template parameters — lambdas are not allowed in that context
// before C++20, and even in C++20+ each lambda has a unique type,
// complicating template argument deduction.
//
// 逐元素算术函数对象。
// 每个都是无状态可调用对象，供 apply_binary / apply_unary 使用。
// 使用函数对象而非 lambda，因为它们可以作为非类型模板参数传递——
// lambda 在 C++20 之前不允许用于该场景，且即使在 C++20+ 中，
// 每个 lambda 都有唯一类型，会使模板实参推导复杂化。
export module m3.detail.operations.ops;

import std;

export namespace m3::detail {

struct Add {
    template <typename T>
    constexpr T operator()(T a, T b) const noexcept {
        return a + b;
    }
};
struct Sub {
    template <typename T>
    constexpr T operator()(T a, T b) const noexcept {
        return a - b;
    }
};
struct Mul {
    template <typename T>
    constexpr T operator()(T a, T b) const noexcept {
        return a * b;
    }
};
struct Div {
    template <typename T>
    constexpr T operator()(T a, T b) const noexcept pre(b != T{0}) {
        return a / b;
    }
};

struct Neg {
    template <typename T>
    constexpr T operator()(T a) const noexcept {
        // Negating the minimum value of a signed integer type is undefined
        // behaviour (signed overflow).  Unsigned and floating-point types
        // are well-defined under negation, so the check is type-conditional.
        //
        // 对有符号整数类型的最小值取负是未定义行为（有符号溢出）。
        // 无符号和浮点类型的取负是良定义的，因此检查按类型条件化。
        if constexpr (std::is_integral_v<T> && std::is_signed_v<T>) {
            if (a == std::numeric_limits<T>::lowest()) [[unlikely]] {
                std::abort();
            }
        }
        return -a;
    }
};

}  // namespace m3::detail
