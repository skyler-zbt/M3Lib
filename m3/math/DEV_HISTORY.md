# Math Development History

## Key changes

### 2026-06-01 to 2026-07-10 · Split and expand math functions

- Split the single-file math module into `vector` and `common` partitions and added vector geometry and common math functions.
- Expanded the GLSL-style common functions. `mod` computes the remainder using a quotient rounded toward negative infinity. Added trigonometric, exponential, point transformation, and direction transformation functions. The vector overloads reuse the existing component-wise helpers.

### 2026-11-09 · Correct intermediate calculations for extreme inputs

- `length` converts integer components to the floating-point result type before squaring. The floating-point path uses `hypot` when a direct sum of squares is unsuitable, reducing precision loss from intermediate overflow or underflow. For integer inputs, `distance` computes the unsigned magnitude of each difference before converting to floating point. This preserves the difference between adjacent large integers that would round to the same value if converted first. `normalize` scales components before computing the normalized result on the extreme-value path. See the [geometry implementation](vector.cppm) and [extreme-value tests](../../tests/test_geometry.cpp).
- `smoothstep` handles interval endpoints first. When the interval is too wide, it halves the values before subtracting. Scalar and vector `mod` share the same implementation for extreme values. See the [common math implementation](common.cppm) and [extreme-value tests](../../tests/test_common_math.cpp).

### 2026-11-09 · Floating-point square matrix inversion

`inverse` uses Gauss–Jordan elimination with partial pivoting and fixed-size scratch storage. It terminates on a zero pivot or a detected non-finite computation. Tests compare against GLM, check residuals against the identity for both multiplication orders, exercise row swaps during pivoting and compile-time calls, and verify termination in a separate process. The maintainer still needs to confirm the rationale for the algorithm and whether termination should be a long-term API guarantee. See the [matrix interface changes](../matrix/DEV_HISTORY.md#2026-11-09--separate-linear-algebra-and-component-wise-operations).
