<!--
  ⚠️ Read this first! / 请先阅读！

  This template applies to ALL pull requests.
  本模板适用于所有 PR。

  Branch model / 分支模型

  M3Lib uses a single-branch model: `master` is both the development and
  release branch.  Open PRs against `master`.
  M3Lib 采用单分支模型：`master` 同时作为开发与发布分支。
  PR 直接向 `master` 发起即可。
-->

## What / 改了什么

<!-- Brief description. 一句话描述。 -->

## Why / 为什么

<!-- Motivation, issue link, or design rationale. 动机、issue 链接、设计依据。 -->

## How / 怎么做的

<!-- Implementation notes, tradeoffs, follow-ups. 实现要点、权衡、后续。 -->

## Testing / 测试

- [ ] All existing tests pass / 全部已有测试通过 (`xmake build tests`)
- [ ] Added tests for new behavior / 新行为已加测试
- [ ] Boundary / OOB cases covered / 边界与越界已覆盖

## Checklist / 检查清单

- [ ] Code follows `clang-format` (runs in CI) / 代码遵循 `clang-format`（CI 自动检查）
- [ ] Comments use bilingual block format (English first, then Chinese) per [CONTRIBUTING.md — Code Style](./CONTRIBUTING.md#code-style) / 注释采用双语块分隔格式（先完整英文段落，再完整中文段落），参见 [CONTRIBUTING.md — 代码风格](./CONTRIBUTING.md#code-style)
- [ ] Public API has no `[[nodiscard]]` violations / 公共 API 无 `[[nodiscard]]` 违规
- [ ] No new `try-catch` (use `std::expected` / `std::optional`) / 不新增 `try-catch`（使用 `std::expected` / `std::optional`）
- [ ] No new C-macros / `#include` (use `import std;` / modules) / 不新增 C 宏或 `#include`（使用 `import std;` / modules）