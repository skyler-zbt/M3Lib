
export module m3.math:transform;

import std;
import m3.detail;
import m3.matrix;
import m3.vector;

export namespace m3 {

template <detail::FloatingPoint T, detail::Qualifier Q>
[[nodiscard("pure function: discarding a transformed point is likely a bug")]]
constexpr Vec<3, T, Q> transform_point(const Mat<4, 4, T, Q>& m, const Vec<3, T, Q>& p) noexcept {
    Vec<4, T, Q> homogeneous{p.x(), p.y(), p.z(), static_cast<T>(1)};
    Vec<4, T, Q> result = m * homogeneous;

    if (result.w() == static_cast<T>(0)) {
        return Vec<3, T, Q>{result.x(), result.y(), result.z()};
    }
    return Vec<3, T, Q>{result.x() / result.w(), result.y() / result.w(), result.z() / result.w()};
}

template <detail::FloatingPoint T, detail::Qualifier Q>
[[nodiscard("pure function: discarding a transformed direction is likely a bug")]]
constexpr Vec<3, T, Q> transform_direction(const Mat<4, 4, T, Q>& m,
                                           const Vec<3, T, Q>& d) noexcept {
    Vec<4, T, Q> homogeneous{d.x(), d.y(), d.z(), static_cast<T>(0)};
    Vec<4, T, Q> result = m * homogeneous;
    return Vec<3, T, Q>{result.x(), result.y(), result.z()};
}

}
