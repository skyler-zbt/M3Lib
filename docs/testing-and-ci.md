# Testing & CI notes (deferred from ROADMAP)

> Separate from the main roadmap. The `tests/` tree is expected to be rewritten;
> this file records what that rewrite and CI/perf work should cover when picked up.

## Goals

- Make correctness checks trustworthy against GLSL 4.60 semantics (and documented intentional deltas).
- Keep CI commands identical to what README documents.
- Optionally track performance trends without blocking merges on noise.

## Test rewrite

- Replace the current `tests/` suite end-to-end; do not try to salvage cases that encode the old (wrong) matrix-product index order.
- Prefer a **function × argument shape × scalar type** coverage matrix for shipped GLSL built-ins (trig, exp, common, geometric, relational, matrix once they exist).
- Use ULP or relative-error compares for floating-point; avoid brittle exact equality on transcendental results.
- Property / differential checks vs a reference (e.g. GLM) for non-commutative products: `A * B != B * A`, `(A * B) * v == A * (B * v)`.
- Contract / bounds coverage for both `Vec` and `Mat`; `sizeof` / `alignof` per `Qualifier`.
- Edge-case policy tests aligned with [ROADMAP.md](../ROADMAP.md) intentional deltas and [glsl-deferred.md](glsl-deferred.md): `normalize(0)`, `mod(x, 0)`, `abs(INT_MIN)`, `transform_point` with `w == 0`, etc.

## Build / CI alignment

- Either add an aggregate `tests` xmake target or change README/CI to the real per-target commands — commands in docs and CI must match verbatim.
- Run correctness under sanitizers and across qualifiers, not only a single optimised configuration.

## Benchmarks & compile cost (optional, non-blocking)

- In-repo microbenchmarks as a CI trend monitor (non-blocking): Vec3/Vec4 hot paths, matrix product, a real MVP pipeline that precomputes `P * V * M` outside the vertex loop vs a pure `mat_mat` scenario.
- Optionally keep `-S` / instruction-count snapshots for hot paths.
- If comparing M3 vs GLM module cold-start cost: repeat clean builds, report median/dispersion, and attribute cost across scan / BMI / archive / link before deciding to merge partitions.

## Suggested order of work

1. Fix production bugs listed under ROADMAP v0.3 (especially `mat_matmul`).
2. Rewrite tests against the corrected semantics.
3. Wire CI to the documented build/run commands.
4. Add ULP helpers and coverage tables.
5. Add benchmarks only after correctness is stable.

---

# 测试与 CI 说明（从 ROADMAP 拆出）

> 独立于主路线图。`tests/` 目录预期整目录重写；本文记录日后动手时，重写与 CI/性能工作应覆盖的内容。

## 目标

- 正确性检查可信，对齐 GLSL 4.60 语义（及已文档化的故意语义差）。
- CI 命令与 README 文档完全一致。
- 可选跟踪性能趋势，但不因噪声阻断合并。

## 测试重写

- 端到端替换现有 `tests/` 套件；不要抢救那些按旧的（错误的）矩阵积索引顺序编写的用例。
- 优先建立已交付 GLSL 内建的 **函数 × 参数形状 × 标量类型** 覆盖矩阵（三角、指数、通用、几何、关系；矩阵函数落地后一并纳入）。
- 浮点比较用 ULP 或相对误差；避免对超越函数结果做脆弱的精确相等断言。
- 相对参考实现（如 GLM）做性质 / 差分检查，覆盖非交换积：`A * B != B * A`、`(A * B) * v == A * (B * v)`。
- `Vec` 与 `Mat` 均覆盖契约 / 边界；按 `Qualifier` 检查 `sizeof` / `alignof`。
- 边界策略用例与 [ROADMAP.md](../ROADMAP.md) 故意语义差及 [glsl-deferred.md](glsl-deferred.md) 对齐：`normalize(0)`、`mod(x, 0)`、`abs(INT_MIN)`、`w == 0` 的 `transform_point` 等。

## 构建 / CI 对齐

- 要么增加聚合 `tests` xmake target，要么把 README/CI 改成实际按 target 构建的命令——文档与 CI 中的命令必须逐字一致。
- 正确性测试应在 sanitizers 与多种 qualifier 下运行，而不只跑单一优化配置。

## 基准与编译成本（可选，非阻断）

- 仓库内 microbenchmark 作为 CI 趋势监控（非阻断）：Vec3/Vec4 热路径、矩阵积、真实 MVP 管线（顶点循环外预计算 `P * V * M`）对比纯 `mat_mat` 场景。
- 可选保留热路径的 `-S` / 指令数快照。
- 若比较 M3 与 GLM 模块冷启动成本：重复 clean 构建，报告 median/离散度，并归因到扫描 / BMI / 归档 / 链接各阶段，再决定是否合并过细分区。

## 建议工作顺序

1. 先修 ROADMAP v0.3 所列生产缺陷（尤其是 `mat_matmul`）。
2. 按修正后的语义重写测试。
3. 把 CI 接到文档中的构建/运行命令。
4. 补 ULP 辅助与覆盖表。
5. 正确性稳定后再加基准。
