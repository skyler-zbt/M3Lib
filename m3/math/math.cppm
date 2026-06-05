// m3.math — GLSL-style mathematical functions.
// Partitions:
//   :vector — dot, cross, normalize, length, distance, reflect, refract
//   :common — mix, clamp, lerp
//
// Future: :matrix (transpose, inverse, determinant, …),
//         :quaternion (slerp, nlerp, …)
//
// m3.math —— GLSL 风格的数学函数。
// 分区：
//   :vector —— dot、cross、normalize、length、distance、reflect、refract
//   :common —— mix、clamp、lerp
//
// 待实现：:matrix（transpose、inverse、determinant 等）、
//         :quaternion（slerp、nlerp 等）
export module m3.math;

export import :vector;
export import :common;
