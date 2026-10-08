
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

    constexpr explicit MatrixBase(T scalar) noexcept;

    explicit MatrixBase(const T* ptr) noexcept;

    constexpr Vec<R, T, Q>& operator[](std::size_t i) noexcept pre(i < static_cast<std::size_t>(C));
    constexpr const Vec<R, T, Q>& operator[](std::size_t i) const noexcept
        pre(i < static_cast<std::size_t>(C));

    constexpr T& operator()(std::size_t c, std::size_t r) noexcept
        pre(c < static_cast<std::size_t>(C) && r < static_cast<std::size_t>(R));
    constexpr const T& operator()(std::size_t c, std::size_t r) const noexcept
        pre(c < static_cast<std::size_t>(C) && r < static_cast<std::size_t>(R));

    [[nodiscard("raw pointer to underlying data; intended for C API / SIMD interop")]]
    constexpr T* value_ptr() noexcept;
    [[nodiscard("raw pointer to underlying data; intended for C API / SIMD interop")]]
    constexpr const T* value_ptr() const noexcept;
protected:
    detail::MatrixStorage<C, R, T, Q> storage_;
};

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

    for (std::size_t c = 0; c < static_cast<std::size_t>(C); ++c) {
        for (std::size_t r = 0; r < static_cast<std::size_t>(R); ++r) {
            storage_.columns_[c][r] = ptr[c * static_cast<std::size_t>(R) + r];
        }
    }
}

template <int C, int R, typename T, Qualifier Q>
requires detail::ValidMatrixSize<C, R>
constexpr Vec<R, T, Q>& MatrixBase<C, R, T, Q>::operator[](std::size_t i) noexcept {

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

}