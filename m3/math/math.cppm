// m3.math — GLSL-style mathematical functions.
// Partitions:
//   :vector    — dot, cross, normalize, length, distance, reflect, refract
//   :common    — mix, clamp, lerp, abs, sign, floor, ceil, fract, mod,
//                step, smoothstep, min, max
//   :trig      — sin, cos, tan, asin, acos, atan, atan2, radians, degrees
//   :exp       — pow, exp, log, exp2, log2, sqrt, inversesqrt
//   :transform — transform_point, transform_direction (Mat4 × Vec3)
//
// Future: :matrix (transpose, inverse, determinant, …),
//         :quaternion (slerp, nlerp, …)
//
// m3.math —— GLSL 风格的数学函数。
// 分区：
//   :vector    —— dot、cross、normalize、length、distance、reflect、refract
//   :common    —— mix、clamp、lerp、abs、sign、floor、ceil、fract、mod、
//                 step、smoothstep、min、max
//   :trig      —— sin、cos、tan、asin、acos、atan、atan2、radians、degrees
//   :exp       —— pow、exp、log、exp2、log2、sqrt、inversesqrt
//   :transform —— transform_point、transform_direction（Mat4 × Vec3）
//
// 待实现：:matrix（transpose、inverse、determinant 等）、
//         :quaternion（slerp、nlerp 等）
export module m3.math;

export import :vector;
export import :common;
export import :trig;
export import :exp;
export import :transform;
