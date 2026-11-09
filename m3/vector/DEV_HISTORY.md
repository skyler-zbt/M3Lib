# Vector Development History

## Key changes

### 2026-05-12 · Introduce layered vector types

Introduced layered vector types to accommodate future SIMD extensions. The design was still in development and awaited validation at the time.

### 2026-06-01 to 2026-06-07 · Construction, layout, and bounds checks

- Added a compile-time diagnostic for construction across dimensions and added tuple protocol support. Changed the default qualifier from `aligned_high` to `aligned_none` so the default layout uses the element type’s natural alignment.
- Added explicit bounds checks to protect accesses at runtime when Contracts are disabled or ignored. UBSan builds under GCC 16.1.0 caught an out-of-bounds access at `[[assume]]`. Moving the explicit check ahead of `[[assume]]` makes an invalid access reach `std::abort()` first.

### 2026-06-19 to 2026-07-10 · Move storage and add GLSL-style aliases

Moved `VectorStorage` to `vector` so the vector types and their storage are maintained together. Added GLSL-style floating-point aliases `vec2`, `vec3`, and `vec4` and integer aliases `ivec2`, `ivec3`, and `ivec4`. See the [rationale for moving storage](../detail/DEV_HISTORY.md#2026-06-19--move-storage-to-its-owning-types).

### 2026-11-09 · Direct output and compound assignment

Added `*_into` operations that write directly to the output object's active components to reduce the cost of returning a temporary vector. Compound assignment with two vectors reuses the corresponding output operation. Compound assignment with a scalar on the right reuses the shared scalar helper. See the [arithmetic implementation](operators.cppm) and [direct output helpers](../detail/DEV_HISTORY.md#2026-11-09--direct-output-helpers-and-division-checks).

The maintainer recalls that M3Lib took about 20 times as long as GLM in an early vector operation comparison. The test case and configuration have yet to be recorded. A separate local performance analysis on 2026-10-28 using GCC 16.2 in Release mode found stack temporaries, padding handling, and whole-object copies in the assembly for batched addition of aligned `float` vec3 values. For the same aligned type, writing components directly reduced the time per object in that test. The original analysis report is absent from the current checkout.
