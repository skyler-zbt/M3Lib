
export module m3.matrix.mat;

import std;

import m3.detail;
import m3.matrix.base;
import m3.vector.vec;

export namespace m3 {

template <int C, int R, detail::Arithmetic T, detail::Qualifier Q = detail::Qualifier::aligned_none>
requires detail::ValidMatrixSize<C, R>
class Mat : public detail::MatrixBase<C, R, T, Q> {
    using base_type = detail::MatrixBase<C, R, T, Q>;
    using base_type::base_type;
public:
    static constexpr int columns = C;
    static constexpr int rows = R;
    using value_type = T;
    using column_type = Vec<R, T, Q>;
    using qualifier_type = detail::Qualifier;

    static_assert(C == R, "Mat<C,R,...> is square-only in v0.2 alpha; C must equal R. "
                          "Rectangular matrices are deferred to v0.5.");

    Mat() = default;

    explicit constexpr Mat(const T* arr) noexcept : base_type(arr) {}
};

template <detail::Arithmetic T, detail::Qualifier Q = detail::Qualifier::aligned_none>
using Mat2 = Mat<2, 2, T, Q>;

template <detail::Arithmetic T, detail::Qualifier Q = detail::Qualifier::aligned_none>
using Mat3 = Mat<3, 3, T, Q>;

template <detail::Arithmetic T, detail::Qualifier Q = detail::Qualifier::aligned_none>
using Mat4 = Mat<4, 4, T, Q>;

}
