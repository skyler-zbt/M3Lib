<!--
  ⚠️ Read this first! / 请先阅读！

  This template applies to ALL pull requests.
  本模板适用于所有 PR。

  ⚠️ PR target / 发起目标 / Base branch

       feat/* or fix/* ──→ dev ──→ master (on release)

  DO NOT open a PR directly against `master`.
  Open against `dev` instead.  Merging to master is reserved for
  release-time integration of stable dev commits.

  不要直接向 master 发起 PR。
  改向 dev 发起。合 master 保留给发布时从稳定 dev 集成。
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

- [ ] Code follows `clang-format` (runs in CI)
- [ ] Comments use bilingual block format (English first, then Chinese) per PREFERENCE §二
- [ ] Public API has no `[[nodiscard]]` violations
- [ ] No new `try-catch` (use `std::expected` / `std::optional`)
- [ ] No new C-macros / `#include` (use `import std;` / modules)
- [ ] CHANGELOG.md updated under "Unreleased" / 已更新 CHANGELOG.md
