export module m3.math:matrix;

import std;
import m3.detail;
import m3.matrix;

export namespace m3 {

// The input must be finite and invertible, with a finite representable inverse.
// Zero pivots and non-finite arithmetic terminate, just like checked division.
// Partial pivoting does not guarantee accuracy for ill-conditioned matrices.
template <int N, detail::FloatingPoint T, detail::Qualifier Q>
[[nodiscard("pure function: discarding an inverse is likely a bug")]]
constexpr Mat<N, N, T, Q> inverse(const Mat<N, N, T, Q> &matrix) noexcept {
  constexpr std::size_t size = static_cast<std::size_t>(N);
  std::array<std::array<T, 2 * size>, size> augmented{};

  // Mat indexes (column, row); elimination uses row-major scratch storage.
  for (std::size_t row = 0; row < size; ++row) {
    for (std::size_t column = 0; column < size; ++column) {
      const T value = matrix(column, row);
      if (!std::isfinite(value)) [[unlikely]] {
        std::abort();
      }
      augmented[row][column] = value;
    }
    augmented[row][size + row] = T{1};
  }

  for (std::size_t column = 0; column < size; ++column) {
    std::size_t pivot_row = column;
    T pivot_magnitude = std::abs(augmented[column][column]);
    for (std::size_t row = column + 1; row < size; ++row) {
      const T magnitude = std::abs(augmented[row][column]);
      if (magnitude > pivot_magnitude) {
        pivot_magnitude = magnitude;
        pivot_row = row;
      }
    }
    if (pivot_magnitude == T{0}) [[unlikely]] {
      std::abort();
    }
    if (pivot_row != column) {
      std::swap(augmented[column], augmented[pivot_row]);
    }

    const T pivot = augmented[column][column];
    for (std::size_t entry = column + 1; entry < 2 * size; ++entry) {
      augmented[column][entry] /= pivot;
      if (!std::isfinite(augmented[column][entry])) [[unlikely]] {
        std::abort();
      }
    }
    augmented[column][column] = T{1};

    for (std::size_t row = 0; row < size; ++row) {
      if (row == column) {
        continue;
      }
      const T factor = augmented[row][column];
      augmented[row][column] = T{0};
      if (factor == T{0}) {
        continue;
      }
      for (std::size_t entry = column + 1; entry < 2 * size; ++entry) {
        const T product = factor * augmented[column][entry];
        if (!std::isfinite(product)) [[unlikely]] {
          std::abort();
        }
        augmented[row][entry] -= product;
        if (!std::isfinite(augmented[row][entry])) [[unlikely]] {
          std::abort();
        }
      }
    }
  }

  Mat<N, N, T, Q> result;
  for (std::size_t row = 0; row < size; ++row) {
    for (std::size_t column = 0; column < size; ++column) {
      result(column, row) = augmented[row][size + column];
    }
  }
  return result;
}

} // namespace m3
