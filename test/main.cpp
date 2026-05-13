import std;
import m3.vector;
import m3.detail;

int tests_total  = 0;
int tests_passed = 0;
int tests_failed = 0;

void test_result(bool ok, const char* name) {
    ++tests_total;
    if (ok) {
        ++tests_passed;
        std::println("  PASS  {}", name);
    } else {
        ++tests_failed;
        std::println(std::cerr, "  FAIL  {}", name);
    }
}

// ============================================================================
// Vec<1> storage[] bounds
// ============================================================================
void test_vec1_storage() {
    std::println("=== Vec<1>.storage[] ===");

    // Valid: index 0
    {
        m3::Vec<1, int> v(42);
        bool ok = (v.storage[0] == 42);
        test_result(ok, "Vec<1>{42}.storage[0] == 42");
    }

    // Write through operator[]
    {
        m3::Vec<1, int> v(0);
        v.storage[0] = 99;
        bool ok = (v.storage[0] == 99);
        test_result(ok, "write storage[0] = 99");
    }

    // const overload
    {
        const m3::Vec<1, float> v(3.14f);
        bool ok = (v.storage[0] == 3.14f);
        test_result(ok, "const Vec<1>.storage[0] == 3.14f");
    }

    // Default-constructed
    {
        m3::Vec<1, int> v;
        [[maybe_unused]] auto val = v.storage[0];
        test_result(true, "default-constructed Vec<1>.storage[0] accessible");
    }
}

// ============================================================================
// Vec<1> x() / r() / s() swizzle
// ============================================================================
void test_vec1_swizzle() {
    std::println("=== Vec<1> x() / r() / s() ===");

    // All three return same value
    {
        m3::Vec<1, float> v(2.718f);
        bool ok = (v.x() == 2.718f) && (v.r() == 2.718f) && (v.s() == 2.718f);
        test_result(ok, "x(), r(), s() all return 2.718f");
    }

    // Write through x(), read through all three
    {
        m3::Vec<1, int> v(0);
        v.x() = 10;
        bool ok = (v.x() == 10) && (v.r() == 10) && (v.s() == 10);
        test_result(ok, "write x()=10, r() and s() see 10");
    }

    // Write through r()
    {
        m3::Vec<1, int> v(0);
        v.r() = 20;
        bool ok = (v.x() == 20);
        test_result(ok, "write r()=20, x() sees 20");
    }

    // Write through s()
    {
        m3::Vec<1, int> v(0);
        v.s() = 30;
        bool ok = (v.x() == 30);
        test_result(ok, "write s()=30, x() sees 30");
    }

    // Const overloads
    {
        const m3::Vec<1, double> v(1.414);
        bool ok = (v.x() == 1.414) && (v.r() == 1.414) && (v.s() == 1.414);
        test_result(ok, "const x(), r(), s() all return 1.414");
    }

    // Aliasing: &x() == value_ptr()
    {
        m3::Vec<1, int> v(99);
        bool ok = (&v.x() == v.value_ptr());
        test_result(ok, "&x() == value_ptr()");
    }
}

// ============================================================================
// Vec<1> value_ptr()
// ============================================================================
void test_vec1_value_ptr() {
    std::println("=== Vec<1> value_ptr() ===");

    // Non-null
    {
        m3::Vec<1, int> v;
        bool ok = (v.value_ptr() != nullptr);
        test_result(ok, "default Vec<1> value_ptr() != nullptr");
    }

    // Points to value
    {
        m3::Vec<1, float> v(2.718f);
        bool ok = (*v.value_ptr() == 2.718f);
        test_result(ok, "Vec<1>{2.718f} *value_ptr() == 2.718f");
    }

    // Mutable write-through
    {
        m3::Vec<1, int> v(0);
        *v.value_ptr() = 77;
        bool ok = (v.storage[0] == 77);
        test_result(ok, "write *value_ptr()=77, storage[0] == 77");
    }

    // Pointer stability
    {
        m3::Vec<1, int> v(0);
        bool ok = (v.value_ptr() == v.value_ptr());
        test_result(ok, "value_ptr() returns stable address");
    }

    // Const overload
    {
        const m3::Vec<1, double> v(0.5);
        bool ok = (*v.value_ptr() == 0.5);
        test_result(ok, "const value_ptr() const, *p == 0.5");
    }
}

// ============================================================================
int main() {
    std::println("=== Vec<1> Tests ===\n");

    test_vec1_storage();
    test_vec1_swizzle();
    test_vec1_value_ptr();

    std::println("\n=== {}/{} passed, {} failed ===",
        tests_passed, tests_total, tests_failed);

    return tests_failed > 0 ? 1 : 0;
}
