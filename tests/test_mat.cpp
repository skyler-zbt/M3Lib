// M3Lib — Mat type tests: construction, accessors, operators, comparison, products.
//    xmake build test_mat && xmake run test_mat
//
// Covers S1.7 acceptance criteria: ~50 cases spanning construction,
// element access, Hadamard ops, scalar broadcast, compound assignment,
// matrix product, Mat*Vec, Vec*Mat, and boundary checks.

import std;
import m3;

// === test infrastructure ===

import test_common;

// === test cases ===

int main() {
    TestRunner runner;

    // ---- Construction ----

    runner.add("default Mat3 constructs zero", [] -> TestResult {
        m3::Mat<3, 3, float> m;
        for (std::size_t c = 0; c < 3; ++c)
            for (std::size_t r = 0; r < 3; ++r)
                if (auto r2 = check_float_eq(m(c, r), 0.0f, 1e-6f); !r2)
                    return r2;
        return {};
    });

    runner.add("broadcast scalar 1 produces identity-like matrix", [] -> TestResult {
        m3::Mat<3, 3, float> m{1.0f};
        for (std::size_t c = 0; c < 3; ++c) {
            for (std::size_t r = 0; r < 3; ++r) {
                float expected = (c == r) ? 1.0f : 0.0f;
                if (auto r2 = check_float_eq(m(c, r), expected, 1e-6f); !r2)
                    return r2;
            }
        }
        return {};
    });

    runner.add("broadcast scalar 0 produces zero matrix", [] -> TestResult {
        m3::Mat<4, 4, float> m{0.0f};
        for (std::size_t c = 0; c < 4; ++c)
            for (std::size_t r = 0; r < 4; ++r)
                if (auto r2 = check_float_eq(m(c, r), 0.0f, 1e-6f); !r2)
                    return r2;
        return {};
    });

    runner.add("column-major pointer ctor reads columns contiguously", [] -> TestResult {
        // Column 0: {1, 2, 3}, column 1: {4, 5, 6}, column 2: {7, 8, 9}
        float data[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        m3::Mat<3, 3, float> m{data};
        if (auto r2 = check_float_eq(m(0, 0), 1.0f, 1e-6f); !r2)
            return r2;
        if (auto r2 = check_float_eq(m(0, 1), 2.0f, 1e-6f); !r2)
            return r2;
        if (auto r2 = check_float_eq(m(0, 2), 3.0f, 1e-6f); !r2)
            return r2;
        if (auto r2 = check_float_eq(m(1, 0), 4.0f, 1e-6f); !r2)
            return r2;
        if (auto r2 = check_float_eq(m(1, 1), 5.0f, 1e-6f); !r2)
            return r2;
        if (auto r2 = check_float_eq(m(2, 2), 9.0f, 1e-6f); !r2)
            return r2;
        return {};
    });

    runner.add("Mat2 (alias) constructs correctly", [] -> TestResult {
        m3::Mat2<float> m{1.0f};
        if (auto r2 = check_float_eq(m(0, 0), 1.0f, 1e-6f); !r2)
            return r2;
        if (auto r2 = check_float_eq(m(1, 1), 1.0f, 1e-6f); !r2)
            return r2;
        if (auto r2 = check_float_eq(m(0, 1), 0.0f, 1e-6f); !r2)
            return r2;
        return {};
    });

    runner.add("Mat3 (alias) constructs correctly", [] -> TestResult {
        m3::Mat3<float> m{1.0f};
        if (auto r2 = check_float_eq(m(0, 0), 1.0f, 1e-6f); !r2)
            return r2;
        if (auto r2 = check_float_eq(m(2, 2), 1.0f, 1e-6f); !r2)
            return r2;
        if (auto r2 = check_float_eq(m(1, 0), 0.0f, 1e-6f); !r2)
            return r2;
        return {};
    });

    runner.add("Mat4 (alias) constructs correctly", [] -> TestResult {
        m3::Mat4<float> m{1.0f};
        if (auto r2 = check_float_eq(m(0, 0), 1.0f, 1e-6f); !r2)
            return r2;
        if (auto r2 = check_float_eq(m(3, 3), 1.0f, 1e-6f); !r2)
            return r2;
        if (auto r2 = check_float_eq(m(2, 1), 0.0f, 1e-6f); !r2)
            return r2;
        return {};
    });

    runner.add("int Mat works (Arithmetic, not FloatingPoint)", [] -> TestResult {
        m3::Mat<3, 3, int> m{2};
        if (auto r2 = check(m(0, 0) == 2); !r2)
            return r2;
        if (auto r2 = check(m(1, 1) == 2); !r2)
            return r2;
        if (auto r2 = check(m(0, 1) == 0); !r2)
            return r2;
        return {};
    });

    // ---- Element access ----

    runner.add("operator() returns writable reference", [] -> TestResult {
        m3::Mat3<float> m{};
        m(1, 2) = 5.5f;
        if (auto r2 = check_float_eq(m(1, 2), 5.5f, 1e-6f); !r2)
            return r2;
        return {};
    });

    runner.add("operator[] returns Vec column by value", [] -> TestResult {
        m3::Mat3<float> m{};
        m(0, 0) = 1.0f;
        m(0, 1) = 2.0f;
        m(0, 2) = 3.0f;
        m3::Vec<3, float> col0 = m[0];
        if (auto r2 = check_float_eq(col0[0], 1.0f, 1e-6f); !r2)
            return r2;
        if (auto r2 = check_float_eq(col0[1], 2.0f, 1e-6f); !r2)
            return r2;
        if (auto r2 = check_float_eq(col0[2], 3.0f, 1e-6f); !r2)
            return r2;
        return {};
    });

    // ---- Element-wise (Hadamard) operators ----

    runner.add("Mat + Mat element-wise adds", [] -> TestResult {
        m3::Mat3<float> a{1.0f};
        m3::Mat3<float> b{2.0f};
        m3::Mat3<float> c = a + b;
        for (std::size_t c_idx = 0; c_idx < 3; ++c_idx) {
            for (std::size_t r = 0; r < 3; ++r) {
                float expected = (c_idx == r) ? 3.0f : 0.0f;
                if (auto r2 = check_float_eq(c(c_idx, r), expected, 1e-6f); !r2)
                    return r2;
            }
        }
        return {};
    });

    runner.add("Mat - Mat element-wise subtracts", [] -> TestResult {
        m3::Mat3<float> a{5.0f};
        m3::Mat3<float> b{2.0f};
        m3::Mat3<float> c = a - b;
        for (std::size_t c_idx = 0; c_idx < 3; ++c_idx) {
            for (std::size_t r = 0; r < 3; ++r) {
                float expected = (c_idx == r) ? 3.0f : 0.0f;
                if (auto r2 = check_float_eq(c(c_idx, r), expected, 1e-6f); !r2)
                    return r2;
            }
        }
        return {};
    });

    runner.add("Mat / Mat element-wise divides", [] -> TestResult {
        m3::Mat3<float> a{0.0f};
        for (std::size_t c = 0; c < 3; ++c)
            for (std::size_t r = 0; r < 3; ++r)
                a(c, r) = static_cast<float>(c * 3 + r + 1);  // 1..9
        m3::Mat3<float> b{0.0f};
        for (std::size_t c = 0; c < 3; ++c)
            for (std::size_t r = 0; r < 3; ++r)
                b(c, r) = 1.0f;  // all ones — safe denominator
        m3::Mat3<float> c = a / b;
        for (std::size_t c_idx = 0; c_idx < 3; ++c_idx) {
            for (std::size_t r = 0; r < 3; ++r) {
                float expected = static_cast<float>(c_idx * 3 + r + 1);
                if (auto r2 = check_float_eq(c(c_idx, r), expected, 1e-6f); !r2)
                    return r2;
            }
        }
        return {};
    });

    runner.add("unary - negates matrix", [] -> TestResult {
        m3::Mat2<float> m{2.0f};
        m3::Mat2<float> n = -m;
        if (auto r2 = check_float_eq(n(0, 0), -2.0f, 1e-6f); !r2)
            return r2;
        if (auto r2 = check_float_eq(n(1, 1), -2.0f, 1e-6f); !r2)
            return r2;
        if (auto r2 = check_float_eq(n(0, 1), 0.0f, 1e-6f); !r2)
            return r2;
        return {};
    });

    // ---- Scalar broadcast operators ----

    runner.add("Mat * scalar broadcasts", [] -> TestResult {
        m3::Mat3<float> m{1.0f};
        m3::Mat3<float> r = m * 2.5f;
        if (auto r2 = check_float_eq(r(0, 0), 2.5f, 1e-6f); !r2)
            return r2;
        if (auto r2 = check_float_eq(r(2, 2), 2.5f, 1e-6f); !r2)
            return r2;
        return {};
    });

    runner.add("scalar * Mat broadcasts (commutative)", [] -> TestResult {
        m3::Mat3<float> m{1.0f};
        m3::Mat3<float> r = 2.5f * m;
        if (auto r2 = check_float_eq(r(0, 0), 2.5f, 1e-6f); !r2)
            return r2;
        if (auto r2 = check_float_eq(r(2, 2), 2.5f, 1e-6f); !r2)
            return r2;
        return {};
    });

    runner.add("Mat / scalar divides", [] -> TestResult {
        m3::Mat3<float> m{10.0f};
        m3::Mat3<float> r = m / 2.0f;
        if (auto r2 = check_float_eq(r(0, 0), 5.0f, 1e-6f); !r2)
            return r2;
        return {};
    });

    runner.add("Mat + scalar broadcasts", [] -> TestResult {
        m3::Mat3<float> m{1.0f};
        m3::Mat3<float> r = m + 3.0f;
        if (auto r2 = check_float_eq(r(0, 0), 4.0f, 1e-6f); !r2)
            return r2;
        if (auto r2 = check_float_eq(r(1, 2), 3.0f, 1e-6f); !r2)
            return r2;  // 0 + 3
        return {};
    });

    runner.add("Mat - scalar broadcasts", [] -> TestResult {
        m3::Mat3<float> m{5.0f};
        m3::Mat3<float> r = m - 2.0f;
        if (auto r2 = check_float_eq(r(0, 0), 3.0f, 1e-6f); !r2)
            return r2;
        if (auto r2 = check_float_eq(r(1, 1), 3.0f, 1e-6f); !r2)
            return r2;
        return {};
    });

    // ---- Compound assignment ----

    runner.add("Mat += Mat works", [] -> TestResult {
        m3::Mat3<float> a{1.0f};
        m3::Mat3<float> b{2.0f};
        a += b;
        if (auto r2 = check_float_eq(a(0, 0), 3.0f, 1e-6f); !r2)
            return r2;
        return {};
    });

    runner.add("Mat -= Mat works", [] -> TestResult {
        m3::Mat3<float> a{5.0f};
        m3::Mat3<float> b{2.0f};
        a -= b;
        if (auto r2 = check_float_eq(a(0, 0), 3.0f, 1e-6f); !r2)
            return r2;
        return {};
    });

    runner.add("Mat /= Mat works", [] -> TestResult {
        m3::Mat3<float> a{6.0f};
        m3::Mat3<float> b{2.0f};
        a /= b;
        if (auto r2 = check_float_eq(a(0, 0), 3.0f, 1e-6f); !r2)
            return r2;
        return {};
    });

    runner.add("Mat *= scalar works", [] -> TestResult {
        m3::Mat3<float> a{2.0f};
        a *= 3.0f;
        if (auto r2 = check_float_eq(a(0, 0), 6.0f, 1e-6f); !r2)
            return r2;
        return {};
    });

    runner.add("Mat += scalar works", [] -> TestResult {
        m3::Mat3<float> a{1.0f};
        a += 4.0f;
        if (auto r2 = check_float_eq(a(0, 0), 5.0f, 1e-6f); !r2)
            return r2;
        if (auto r2 = check_float_eq(a(1, 2), 4.0f, 1e-6f); !r2)
            return r2;
        return {};
    });

    // ---- Equality ----

    runner.add("Mat == Mat true for identical", [] -> TestResult {
        m3::Mat3<float> a{1.0f};
        m3::Mat3<float> b{1.0f};
        if (auto r2 = check(a == b); !r2)
            return r2;
        if (auto r2 = check(!(a != b)); !r2)
            return r2;
        return {};
    });

    runner.add("Mat == Mat false for different", [] -> TestResult {
        m3::Mat3<float> a{1.0f};
        m3::Mat3<float> b{2.0f};
        if (auto r2 = check(a != b); !r2)
            return r2;
        if (auto r2 = check(!(a == b)); !r2)
            return r2;
        return {};
    });

    // ---- Matrix product (Mat * Mat) ----

    runner.add("Mat2 * Mat2 product (identity * M = M)", [] -> TestResult {
        m3::Mat2<float> I{1.0f};
        m3::Mat2<float> M{0.0f};
        M(0, 0) = 1.0f;
        M(0, 1) = 2.0f;
        M(1, 0) = 3.0f;
        M(1, 1) = 4.0f;
        m3::Mat2<float> R = I * M;
        if (auto r2 = check_float_eq(R(0, 0), 1.0f, 1e-6f); !r2)
            return r2;
        if (auto r2 = check_float_eq(R(0, 1), 2.0f, 1e-6f); !r2)
            return r2;
        if (auto r2 = check_float_eq(R(1, 0), 3.0f, 1e-6f); !r2)
            return r2;
        if (auto r2 = check_float_eq(R(1, 1), 4.0f, 1e-6f); !r2)
            return r2;
        return {};
    });

    runner.add("Mat2 * Mat2 classic product", [] -> TestResult {
        // A = [[1,2],[3,4]], B = [[5,6],[7,8]] in row-major view (column 0 of A is {1,2}, etc.)
        // A*B in column-major formula: result(c, r) = sum_k A(c, k) * B(k, r)
        // R(0, 0) = A(0,0)*B(0,0) + A(0,1)*B(1,0) = 1*5 + 2*7 = 19
        // R(0, 1) = A(0,0)*B(0,1) + A(0,1)*B(1,1) = 1*6 + 2*8 = 22
        // R(1, 0) = A(1,0)*B(0,0) + A(1,1)*B(1,0) = 3*5 + 4*7 = 43
        // R(1, 1) = A(1,0)*B(0,1) + A(1,1)*B(1,1) = 3*6 + 4*8 = 50
        m3::Mat2<float> A{0.0f}, B{0.0f};
        A(0, 0) = 1;
        A(0, 1) = 2;
        A(1, 0) = 3;
        A(1, 1) = 4;
        B(0, 0) = 5;
        B(0, 1) = 6;
        B(1, 0) = 7;
        B(1, 1) = 8;
        m3::Mat2<float> R = A * B;
        if (auto r2 = check_float_eq(R(0, 0), 19.0f, 1e-6f); !r2)
            return r2;
        if (auto r2 = check_float_eq(R(0, 1), 22.0f, 1e-6f); !r2)
            return r2;
        if (auto r2 = check_float_eq(R(1, 0), 43.0f, 1e-6f); !r2)
            return r2;
        if (auto r2 = check_float_eq(R(1, 1), 50.0f, 1e-6f); !r2)
            return r2;
        return {};
    });

    runner.add("Mat3 * Mat3 product (identity check)", [] -> TestResult {
        m3::Mat3<float> I{1.0f};
        m3::Mat3<float> M{0.0f};
        for (std::size_t i = 0; i < 3; ++i)
            M(i, i) = static_cast<float>(i + 1);  // diagonal 1,2,3
        m3::Mat3<float> R = I * M;
        for (std::size_t i = 0; i < 3; ++i)
            for (std::size_t j = 0; j < 3; ++j)
                if (auto r2 = check_float_eq(R(j, i), M(i, j), 1e-6f); !r2)
                    return r2;
        return {};
    });

    runner.add("Mat3 * Mat3 non-trivial product", [] -> TestResult {
        m3::Mat3<float> A{0.0f}, B{0.0f};
        A(0, 0) = 1;
        A(0, 1) = 0;
        A(0, 2) = 2;
        A(1, 0) = -1;
        A(1, 1) = 3;
        A(1, 2) = 1;
        A(2, 0) = 0;
        A(2, 1) = 1;
        A(2, 2) = 0;
        B(0, 0) = 3;
        B(0, 1) = 1;
        B(0, 2) = 0;
        B(1, 0) = 1;
        B(1, 1) = 0;
        B(1, 2) = 1;
        B(2, 0) = 2;
        B(2, 1) = 1;
        B(2, 2) = 1;
        m3::Mat3<float> R = A * B;
        // Manual verify a few entries
        // R(0,0) = 1*3 + 0*1 + 2*2 = 7
        // R(1,1) = -1*1 + 3*0 + 1*1 = 0
        if (auto r2 = check_float_eq(R(0, 0), 7.0f, 1e-6f); !r2)
            return r2;
        if (auto r2 = check_float_eq(R(1, 1), 0.0f, 1e-6f); !r2)
            return r2;
        return {};
    });

    runner.add("Mat4 * Mat4 identity product", [] -> TestResult {
        m3::Mat4<float> I{1.0f};
        m3::Mat4<float> M{0.0f};
        for (std::size_t i = 0; i < 4; ++i)
            M(i, i) = static_cast<float>(i + 1);
        m3::Mat4<float> R = I * M;
        for (std::size_t i = 0; i < 4; ++i)
            for (std::size_t j = 0; j < 4; ++j)
                if (auto r2 = check_float_eq(R(j, i), M(i, j), 1e-6f); !r2)
                    return r2;
        return {};
    });

    // ---- Mat * Vec and Vec * Mat ----

    runner.add("Mat2 * Vec2 right multiplication", [] -> TestResult {
        // M set up with M(c, r) indexing: M(0,0)=1, M(0,1)=2, M(1,0)=3, M(1,1)=4
        // Means M = [[1, 3], [2, 4]] in row-major view (column 0 = {1,2}, column 1 = {3,4}).
        // v = [1, 1], M * v: r[r] = sum over c of M(c, r) * v[c]
        //   r[0] = 1*1 + 3*1 = 4
        //   r[1] = 2*1 + 4*1 = 6
        m3::Mat2<float> M{0.0f};
        M(0, 0) = 1;
        M(0, 1) = 2;
        M(1, 0) = 3;
        M(1, 1) = 4;
        m3::Vec<2, float> v{1.0f, 1.0f};
        m3::Vec<2, float> r = M * v;
        if (auto r2 = check_float_eq(r[0], 4.0f, 1e-6f); !r2)
            return r2;
        if (auto r2 = check_float_eq(r[1], 6.0f, 1e-6f); !r2)
            return r2;
        return {};
    });

    runner.add("Vec2 * Mat2 left multiplication", [] -> TestResult {
        // v * M: result[c] = sum over r of v[r] * M(c, r)
        //   r[0] = v[0]*M(0,0) + v[1]*M(0,1) = 1*1 + 1*2 = 3
        //   r[1] = v[0]*M(1,0) + v[1]*M(1,1) = 1*3 + 1*4 = 7
        m3::Mat2<float> M{0.0f};
        M(0, 0) = 1;
        M(0, 1) = 2;
        M(1, 0) = 3;
        M(1, 1) = 4;
        m3::Vec<2, float> v{1.0f, 1.0f};
        m3::Vec<2, float> r = v * M;
        if (auto r2 = check_float_eq(r[0], 3.0f, 1e-6f); !r2)
            return r2;
        if (auto r2 = check_float_eq(r[1], 7.0f, 1e-6f); !r2)
            return r2;
        return {};
    });

    runner.add("Mat3 * Vec3 product (per-column interpretation)", [] -> TestResult {
        // M columns: c0 = {1,0,0}, c1 = {0,1,0}, c2 = {0,0,1}
        // v = {1,2,3}, M*v should be {1,2,3} for identity-like M
        m3::Mat3<float> M{0.0f};
        for (std::size_t i = 0; i < 3; ++i)
            M(i, i) = 1.0f;
        m3::Vec<3, float> v{1.0f, 2.0f, 3.0f};
        m3::Vec<3, float> r = M * v;
        if (auto r2 = check_float_eq(r[0], 1.0f, 1e-6f); !r2)
            return r2;
        if (auto r2 = check_float_eq(r[1], 2.0f, 1e-6f); !r2)
            return r2;
        if (auto r2 = check_float_eq(r[2], 3.0f, 1e-6f); !r2)
            return r2;
        return {};
    });

    runner.add("Vec3 * Mat3 row vector left multiplication", [] -> TestResult {
        m3::Mat3<float> M{0.0f};
        for (std::size_t i = 0; i < 3; ++i)
            M(i, i) = 1.0f;
        m3::Vec<3, float> v{4.0f, 5.0f, 6.0f};
        m3::Vec<3, float> r = v * M;
        if (auto r2 = check_float_eq(r[0], 4.0f, 1e-6f); !r2)
            return r2;
        if (auto r2 = check_float_eq(r[1], 5.0f, 1e-6f); !r2)
            return r2;
        if (auto r2 = check_float_eq(r[2], 6.0f, 1e-6f); !r2)
            return r2;
        return {};
    });

    runner.add("Mat4 * Vec4 transform", [] -> TestResult {
        m3::Mat4<float> I{1.0f};
        m3::Vec<4, float> v{1.0f, 2.0f, 3.0f, 4.0f};
        m3::Vec<4, float> r = I * v;
        if (auto r2 = check_float_eq(r[0], 1.0f, 1e-6f); !r2)
            return r2;
        if (auto r2 = check_float_eq(r[3], 4.0f, 1e-6f); !r2)
            return r2;
        return {};
    });

    // ---- value_ptr ----

    // Note: value_ptr() returns a pointer to column 0's data.  With the
    // default qualifier (aligned_none), columns are densely packed and
    // contiguous — ptr[i] traversal is safe.  With aligned qualifiers
    // (e.g. aligned_16), column-to-column padding may exist; prefer
    // column-by-column access via operator[](c).value_ptr().
    //
    // 注意：value_ptr() 返回第 0 列数据的指针。默认限定符（aligned_none）
    // 下列是密集连续的——ptr[i] 遍历是安全的。对齐限定符（如 aligned_16）
    // 下列间可能存在填充；建议通过 operator[](c).value_ptr() 逐列访问。

    runner.add("value_ptr points to column-major data (default qualifier, contiguous)", [] -> TestResult {
        float data[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        m3::Mat3<float> m{data};
        const float* ptr = m.value_ptr();
        for (std::size_t i = 0; i < 9; ++i)
            if (auto r2 = check_float_eq(ptr[i], data[i], 1e-6f); !r2)
                return r2;
        return {};
    });

    runner.add("value_ptr writable returns T* for in-place mutation (default qualifier)", [] -> TestResult {
        m3::Mat2<float> m{};
        float* ptr = m.value_ptr();
        ptr[0] = 1.0f;
        ptr[1] = 2.0f;
        ptr[2] = 3.0f;
        ptr[3] = 4.0f;
        if (auto r2 = check_float_eq(m(0, 0), 1.0f, 1e-6f); !r2)
            return r2;
        if (auto r2 = check_float_eq(m(1, 1), 4.0f, 1e-6f); !r2)
            return r2;
        return {};
    });

    runner.add("value_ptr column-by-column access (safe for all qualifiers)", [] -> TestResult {
        float data[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        m3::Mat3<float> m{data};
        // Per-column value_ptr() is always safe regardless of qualifier.
        //
        // 无论限定符如何，逐列 value_ptr() 始终安全。
        for (std::size_t c = 0; c < 3; ++c) {
            const float* col_ptr = m[c].value_ptr();
            for (std::size_t r = 0; r < 3; ++r) {
                if (auto r2 = check_float_eq(col_ptr[r], data[c * 3 + r], 1e-6f); !r2)
                    return r2;
            }
        }
        return {};
    });

    // ---- Compound assignment * = Mat ----

    runner.add("Mat *= Mat (matrix product assignment)", [] -> TestResult {
        m3::Mat2<float> a{1.0f};
        m3::Mat2<float> b{0.0f};
        b(0, 0) = 2;
        b(1, 1) = 2;
        a *= b;
        // a was identity, a*b should be b
        if (auto r2 = check_float_eq(a(0, 0), 2.0f, 1e-6f); !r2)
            return r2;
        if (auto r2 = check_float_eq(a(1, 1), 2.0f, 1e-6f); !r2)
            return r2;
        return {};
    });

    // ---- MatrixLike concept compliance (static asserts) ----

    runner.add("MatrixLike concept satisfied", [] -> TestResult {
        // Diagnostic: what does m[i] return?
        using M = m3::Mat3<float>;
        using RetType = decltype(std::declval<M&>()[std::declval<std::size_t>()]);
        using ColType = M::column_type;
        using ConstRetType = decltype(std::declval<const M&>()[std::declval<std::size_t>()]);
        static_assert(std::same_as<RetType, ColType&>,
                      "Mat::operator[] must return column_type& (TD-002 reference)");
        static_assert(std::same_as<ConstRetType, const ColType&>,
                      "Mat::operator[] const must return const column_type&");
        // Each MatrixLike sub-constraint
        static_assert(requires { typename M::value_type; });
        static_assert(requires { typename M::column_type; });
        static_assert(requires { typename M::qualifier_type; });
        static_assert(std::same_as<decltype(M::columns), const int>);
        static_assert(std::same_as<decltype(M::rows), const int>);
        static_assert(m3::detail::Arithmetic<M::value_type>);
        static_assert(std::default_initializable<M>);
        static_assert(requires(M& m, std::size_t i) { m[i]; });
        static_assert(m3::detail::MatrixLike<M>, "Mat3<float> must satisfy MatrixLike");
        static_assert(m3::detail::MatrixLike<m3::Mat4<float>>,
                      "Mat4<float> must satisfy MatrixLike");
        return {};
    });

    runner.add("Mat columns/rows/value_type/column_type accessible", [] -> TestResult {
        static_assert(m3::Mat3<float>::columns == 3, "columns should be 3");
        static_assert(m3::Mat3<float>::rows == 3, "rows should be 3");
        static_assert(std::is_same_v<m3::Mat3<float>::value_type, float>,
                      "value_type should be float");
        static_assert(std::is_same_v<m3::Mat3<float>::column_type, m3::Vec<3, float>>,
                      "column_type should be Vec<3,float>");
        return {};
    });

    // ---- Edge cases ----

    runner.add("non-square requirement: Mat<3,4> rejected at compile time", [] -> TestResult {
        // This test verifies the static_assert fires.  We don't actually
        // instantiate Mat<3,4,...>; the static_assert is on the type.
        // We just confirm Mat<2,2>, Mat<3,3>, Mat<4,4> all work.
        m3::Mat<2, 2, float> m2{};
        m3::Mat<3, 3, float> m3x{};
        m3::Mat<4, 4, float> m4{};
        if (auto r2 = check(m2(0, 0) == 0.0f); !r2)
            return r2;
        if (auto r2 = check(m3x(1, 1) == 0.0f); !r2)
            return r2;
        if (auto r2 = check(m4(3, 3) == 0.0f); !r2)
            return r2;
        return {};
    });

    runner.add("size compile-time: sizeof(Mat<4,4,float>) = 64", [] -> TestResult {
        static_assert(sizeof(m3::Mat<4, 4, float>) == 64,
                      "Mat<4,4,float> should be 64 bytes (16 columns × 4 floats)");
        return {};
    });

    runner.add("size compile-time: sizeof(Mat<3,3,float>) = 36", [] -> TestResult {
        static_assert(sizeof(m3::Mat<3, 3, float>) == 36,
                      "Mat<3,3,float> should be 36 bytes (9 floats × 4 bytes)");
        return {};
    });

    runner.add("size compile-time: sizeof(Mat<2,2,float>) = 16", [] -> TestResult {
        static_assert(sizeof(m3::Mat<2, 2, float>) == 16,
                      "Mat<2,2,float> should be 16 bytes (4 floats × 4 bytes)");
        return {};
    });

    // ---- Operator precedence / associativity ----

    runner.add("a + b * scalar (precedence)", [] -> TestResult {
        m3::Mat2<float> a{1.0f};
        m3::Mat2<float> b{2.0f};
        // (a + b) * 3: a + b has 3 on diagonal, 0 off-diagonal. Multiply by 3: 9 diagonal, 0 off.
        m3::Mat2<float> r = (a + b) * 3.0f;
        if (auto r2 = check_float_eq(r(0, 0), 9.0f, 1e-6f); !r2)
            return r2;
        if (auto r2 = check_float_eq(r(1, 1), 9.0f, 1e-6f); !r2)
            return r2;
        if (auto r2 = check_float_eq(r(0, 1), 0.0f, 1e-6f); !r2)
            return r2;
        return {};
    });

    runner.add("negation then addition", [] -> TestResult {
        m3::Mat2<float> a{2.0f};
        m3::Mat2<float> b{1.0f};
        m3::Mat2<float> r = -a + b;
        // -a has -2 on diagonal, 0 off-diagonal. b has 1 on diagonal, 0 off.
        // -a + b: -1 on diagonal, 0 off-diagonal.
        if (auto r2 = check_float_eq(r(0, 0), -1.0f, 1e-6f); !r2)
            return r2;
        if (auto r2 = check_float_eq(r(1, 0), 0.0f, 1e-6f); !r2)
            return r2;
        if (auto r2 = check_float_eq(r(0, 1), 0.0f, 1e-6f); !r2)
            return r2;
        if (auto r2 = check_float_eq(r(1, 1), -1.0f, 1e-6f); !r2)
            return r2;
        return {};
    });

    // ---- Final summary ----

    return runner.run();
}
