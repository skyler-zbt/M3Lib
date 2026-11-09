# Detail Development History

## Key changes

### 2026-05-13 to 2026-06-05 · Establish the shared implementation layer

Introduced `detail` to isolate implementation details. Generalized `apply` to work with `VectorLike` and removed its direct import of the concrete `Vec` module. Made `AlignmentTraits` take the element type so it can express natural alignment.

### 2026-06-06 · Check division by zero explicitly

Compiler support for propagating Contracts in function object templates was limited at the time. Added an explicit denominator check in `Div` that calls `std::abort()` on zero.

### 2026-06-19 · Move storage to its owning types

Moved vector storage to `vector`, defined matrix storage in `matrix`, and removed the re-export of the old storage module from `detail`. This broke the import cycle between storage, the concrete types, and the shared layer. The shared layer continued to provide alignment and concepts.

### 2026-11-09 · Direct output helpers and division checks

Added `apply_binary_into` and `apply_scalar_binary_right_into`. The division path checks denominators before applying `/` and writing the output. Vector division that returns a value also uses these helpers. The tests cover aliasing, very small denominators, and termination on a zero denominator. See the [vector development history](../vector/DEV_HISTORY.md#2026-11-09--direct-output-and-compound-assignment) and the [division output tests](../../tests/test_division_output.cpp).
