
import std;
import m3;

import test_common;

static void fire_contract_violation() {

    contract_assert(false);
}

void handle_contract_violation(const std::contracts::contract_violation& v) {
    auto loc = v.location();
    std::print(std::cerr, "[contract violation] {} at {}:{}:{}\n", v.comment(), loc.file_name(),
               loc.line(), loc.column());
    if (v.is_terminating()) {
        std::abort();
    }
}

int main() {
    TestRunner runner;

    runner.add("operator[] lower bound (index 0)", [] -> TestResult {
        m3::Vec<4, int> v{0};
        v[0] = 42;
        if (auto r = check(v[0] == 42); !r)
            return r;
        return {};
    });

    runner.add("operator[] upper bound (index L-1)", [] -> TestResult {
        m3::Vec<4, int> v{0};
        v[3] = 99;
        if (auto r = check(v[3] == 99); !r)
            return r;
        return {};
    });

    runner.add("operator[] const valid access passes contract", [] -> TestResult {
        const m3::Vec<3, float> v{1.0f, 2.0f, 3.0f};
        if (auto r = check_float_eq(v[0], 1.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(v[1], 2.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(v[2], 3.0f, 1e-6f); !r)
            return r;
        return {};
    });

    runner.add("contract_assert violation survives observe", [] -> TestResult {
        fire_contract_violation();
        return {};
    });

    runner.add("multiple contract_assert violations survive", [] -> TestResult {
        fire_contract_violation();
        fire_contract_violation();
        fire_contract_violation();
        return {};
    });

    runner.add("vec usable after contract_assert violation", [] -> TestResult {
        m3::Vec<3, float> v{1.0f, 2.0f, 3.0f};

        fire_contract_violation();

        if (auto r = check_float_eq(v[0], 1.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(v[1], 2.0f, 1e-6f); !r)
            return r;
        if (auto r = check_float_eq(v[2], 3.0f, 1e-6f); !r)
            return r;
        auto sum = v + v;
        if (auto r = check_float_eq(sum[0], 2.0f, 1e-6f); !r)
            return r;
        return {};
    });

    return runner.run();
}
