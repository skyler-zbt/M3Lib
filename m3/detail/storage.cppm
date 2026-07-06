// m3.detail.storage — DEPRECATED stub.
//
// Storage modules have been co-located with their owning types to break
// the module dependency cycle that otherwise forms when storage references
// its element type:
//   - m3.vector.storage  (formerly m3.detail.storage.vector_storage)
//   - m3.matrix.storage  (formerly m3.detail.storage.matrix_storage)
//
// See ROADMAP.md (storage co-location rationale).  This file
// is kept as a placeholder so existing `import m3.detail.storage;` lines
// (none expected in user code, but possibly in intermediate branches) do
// not break the build with a hard module-not-found error.
//
// m3.detail.storage —— 已弃用的存根。
//
// 存储模块已与所属类型同目录，以打破存储引用元素类型时形成的模块循环：
//   - m3.vector.storage  （原 m3.detail.storage.vector_storage）
//   - m3.matrix.storage  （原 m3.detail.storage.matrix_storage）
//
// 详见 ROADMAP.md（存储同目录说明）。本文件保留为占位符，避免现有
// `import m3.detail.storage;`（用户代码无此用法，但中间分支可能有）
// 导致"模块未找到"的硬错误。
export module m3.detail.storage;