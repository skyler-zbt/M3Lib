# Matrix Development History

## Key changes

### 2026-06-19 · Introduce square matrices

Introduced `Mat` with column-major storage, accessors, and basic arithmetic. The initial implementation covered square matrices and deferred rectangular support. The maintainer still needs to confirm the scope of future support. See the [rationale for moving storage](../detail/DEV_HISTORY.md#2026-06-19--move-storage-to-its-owning-types).

### 2026-11-09 · Correct matrix multiplication indexing

Result element `(c, r)` is the dot product of row `r` of the left matrix and column `c` of the right matrix. The implementation sums `a(k, r) * b(c, k)`, and `*=` reuses the matrix product. The `Vec * Mat` signature now uses the column count as the result dimension. Regression tests cover noncommuting products, vector products on both sides, and `A *= A`. See the [multiplication implementation](mat_operators.cppm) and [matrix tests](../../tests/test_matrix.cpp).

### 2026-11-09 · Separate linear algebra and component-wise operations

Retained matrix products `*` and `*=`, as well as matrix division by a scalar. The former matrix `/` and `/=` operators performed component-wise division. Their replacements are `component_divide(A, B)` and `A = component_divide(A, B)`, respectively. `component_divide` checks all denominators before calculating the result and terminates the process on either positive or negative zero.

A review of other math libraries and graphics APIs led to keeping the linear algebra multiplication semantics commonly used in graphics. Matrix products compose graphics transformations, and inverse matrices undo invertible transformations. `component_multiply()` and `component_divide()` provide the separate element-wise operations.

For square matrices, GLM evaluates `A / B` as `A * inverse(B)`, which hides the cost of inversion at the call site. Following DirectXMath, M3Lib exposes inversion as a separate function. Callers choose when to compute the inverse and whether to use `A * inverse(B)` or `inverse(B) * A`. [Inversion implementation history](../math/DEV_HISTORY.md#2026-11-09--floating-point-square-matrix-inversion) · [GLM square matrix division](https://github.com/g-truc/glm/blob/6f14f4792a0cde5d0cf2c910506724d61cb95834/glm/detail/type_mat4x4.inl#L247-L252) · [DirectXMath `XMMatrixInverse`](https://learn.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmmatrixinverse)
