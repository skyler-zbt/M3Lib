# M3Lib API guide

The matrix and vector operations on this page are available through `import m3;`. See the [project README](../README.md) for build instructions and a complete program.

## Matrix products and component-wise operations

`Mat<C, R, T, Q>` takes the column count first and the row count second. The current public matrix types are 2×2, 3×3, and 4×4 square matrices. `m[c]` returns column `c`, and `m(c, r)` accesses column `c`, row `r`.

For two matrices of the same type, `a * b` is the linear algebra product and `a *= b` assigns that product to `a`. `m * v` treats `v` as a column vector, and `v * m` treats it as a row vector. In both expressions, the vector's dimension, element type, and alignment qualifier match the matrix.

Use `m3::component_multiply(a, b)` and `m3::component_divide(a, b)` for element-by-element arithmetic on matrices of the same type. `component_divide` checks every denominator and terminates the process if any is zero, including floating-point positive and negative zero. Matrix `/` and `/=` take a scalar of the matrix's element type as the right operand and terminate on a zero divisor.

| Former matrix expression | Equivalent component-wise expression |
| --- | --- |
| `a / b` | `m3::component_divide(a, b)` |
| `a /= b` | `a = m3::component_divide(a, b)` |

See the [matrix development history](../m3/matrix/DEV_HISTORY.md) for the API change and its rationale.

## Explicit matrix inverse

`m3::inverse(b)` is `constexpr noexcept` and returns a matrix with the same size, floating-point element type, and alignment qualifier as `b`. Pass a finite, invertible 2×2, 3×3, or 4×4 matrix whose inverse is representable in its element type. The current implementation terminates the process if it detects a non-finite input or intermediate value, or a zero pivot. Near-singular matrices can produce inaccurate results.

Choose the multiplication order explicitly. Given matrices `a` and `b` of the same type, with `b` suitable for inversion, these expressions apply the inverse on opposite sides of `a`.

```cpp
const auto inv_b = m3::inverse(b);
const auto right = a * inv_b;
const auto left = inv_b * a;
```

See the [math development history](../m3/math/DEV_HISTORY.md) for implementation details and [test documentation](../tests/README.md) for how numerical behavior is checked.

## Vector output operations

`m3::add_into(out, a, b)`, `m3::subtract_into(out, a, b)`, `m3::multiply_into(out, a, b)`, and `m3::divide_into(out, a, b)` operate component by component on three vectors of the same type. They write to `out` and return it by reference. The same vector object can be passed as `out` and either input. `divide_into` checks all denominators before writing and terminates the process on a zero denominator, including either sign of floating-point zero.

```cpp
m3::vec3 a{1.0f, 2.0f, 3.0f};
const m3::vec3 b{4.0f, 5.0f, 6.0f};
auto& out = m3::add_into(a, a, b); // out refers to a, which now contains {5, 7, 9}.
```

See the [vector development history](../m3/vector/DEV_HISTORY.md) for the implementation and performance rationale.
