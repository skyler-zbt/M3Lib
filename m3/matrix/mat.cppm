// Mat — primary matrix type for M3Lib v0.2.0.
// Square matrices (C == R) only in this alpha.  C, R ∈ {2, 3, 4}.
// Column-major layout (TD-002), GLSL std430-compatible.
//
// Mat satisfies MatrixLike:
//   - value_type         = T
//   - column_type        = Vec<R, T, Q>
//   - qualifier_type     = Qualifier
//   - columns / rows     = C / R
//   - operator[]         returns Vec<R, T, Q> by value (flat storage)
//
// The by-value column accessor deviates from the TD-002 reference design
// (Vec& reference return).  The deviation is documented in
// m3/detail/storage/matrix_storage.cppm — flat storage was chosen to break
// a module dependency cycle; the by-value form composes correctly with
// column-wise operators (see mat_operators.cppm).
//
// Mat —— M3Lib v0.2.0 的主矩阵类型。
// alpha 阶段仅方阵（C == R）。C, R ∈ {2, 3, 4}。
// 列主序布局（TD-002），兼容 GLSL std430。
//
// Mat 满足 MatrixLike：
//   - value_type         = T
//   - column_type        = Vec<R, T, Q>
//   - qualifier_type     = Qualifier
//   - columns / rows     = C / R
//   - operator[]         按值返回 Vec<R, T, Q>（扁平存储）
//
// 按值列访问器偏离 TD-002 参考设计（Vec& 引用返回）。偏离原因见
// m3/detail/storage/matrix_storage.cppm 文档——扁平存储为打破模块依赖
// 循环；按值形式与按列运算正确组合（见 mat_operators.cppm）。
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

    // Mat is square-only in v0.2.0 alpha; C == R enforced at the type
    // level.  The general rectangular matrix is deferred to v0.4.0.
    //
    // v0.2.0 alpha 仅方阵；C == R 在类型层强制。通用矩形矩阵推迟到 v0.4.0。
    static_assert(C == R, "Mat<C,R,...> is square-only in v0.2.0 alpha; C must equal R. "
                          "Rectangular matrices are deferred to v0.4.0.");

    Mat() = default;

    // Construct from a column-major array of C * R elements.
    // Wraps MatrixBase(const T*) which already does the column-major copy.
    //
    // 从 C * R 个元素的列主序数组构造。包装 MatrixBase(const T*)，
    // 后者已完成列主序拷贝。
    explicit constexpr Mat(const T* arr) noexcept : base_type(arr) {}
};

// Convenience aliases for the square matrices supported in v0.2.0.
//
// v0.2.0 支持的方阵便捷别名。
template <detail::Arithmetic T, detail::Qualifier Q = detail::Qualifier::aligned_none>
using Mat2 = Mat<2, 2, T, Q>;

template <detail::Arithmetic T, detail::Qualifier Q = detail::Qualifier::aligned_none>
using Mat3 = Mat<3, 3, T, Q>;

template <detail::Arithmetic T, detail::Qualifier Q = detail::Qualifier::aligned_none>
using Mat4 = Mat<4, 4, T, Q>;

}  // namespace m3
