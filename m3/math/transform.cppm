// GLSL-style homogeneous transform helpers for Mat4 × Vec3.
// transform_point: treats Vec3 as a position (w=1), applies perspective divide.
// transform_direction: treats Vec3 as a direction (w=0), no perspective divide.
// These complement the existing operator*(Mat4, Vec4) which provides full
// homogeneous multiplication.
//
// GLSL 风格的 Mat4 × Vec3 齐次变换辅助函数。
// transform_point：将 Vec3 视为位置（w=1），执行透视除法。
// transform_direction：将 Vec3 视为方向（w=0），不执行透视除法。
// 这些函数补充现有的 operator*(Mat4, Vec4) 完整齐次乘法。
export module m3.math:transform;

import std;
import m3.detail;
import m3.matrix;
import m3.vector;

export namespace m3 {

// transform_point — treats Vec3 as a point (w=1), applies full homogeneous
// transform including perspective divide.
// Returns Vec3: the xyz components after dividing by w.
//
// transform_point —— 将 Vec3 视为点（w=1），应用完整齐次变换含透视除法。
// 返回 Vec3：透视除法后的 xyz 分量。
template <detail::FloatingPoint T, detail::Qualifier Q>
[[nodiscard("pure function: discarding a transformed point is likely a bug")]]
constexpr Vec<3, T, Q> transform_point(const Mat<4, 4, T, Q>& m, const Vec<3, T, Q>& p) noexcept {
    Vec<4, T, Q> homogeneous{p.x(), p.y(), p.z(), static_cast<T>(1)};
    Vec<4, T, Q> result = m * homogeneous;
    // Perspective divide: if w is 0 (degenerate), return xyz directly to avoid NaN.
    //
    // 透视除法：若 w 为 0（退化情况），直接返回 xyz 以避免 NaN。
    if (result.w() == static_cast<T>(0)) {
        return Vec<3, T, Q>{result.x(), result.y(), result.z()};
    }
    return Vec<3, T, Q>{result.x() / result.w(), result.y() / result.w(), result.z() / result.w()};
}

// transform_direction — treats Vec3 as a direction (w=0), applies transform
// without translation or perspective divide.
// Returns Vec3: the xyz components directly.
//
// transform_direction —— 将 Vec3 视为方向（w=0），应用变换但不含平移和透视除法。
// 返回 Vec3：直接取 xyz 分量。
template <detail::FloatingPoint T, detail::Qualifier Q>
[[nodiscard("pure function: discarding a transformed direction is likely a bug")]]
constexpr Vec<3, T, Q> transform_direction(const Mat<4, 4, T, Q>& m,
                                           const Vec<3, T, Q>& d) noexcept {
    Vec<4, T, Q> homogeneous{d.x(), d.y(), d.z(), static_cast<T>(0)};
    Vec<4, T, Q> result = m * homogeneous;
    return Vec<3, T, Q>{result.x(), result.y(), result.z()};
}

}  // namespace m3
