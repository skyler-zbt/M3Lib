// m3.detail — re-exports all detail submodules (qualifier, concepts, operations).
//
// Note: storage submodules have been co-located with their owning types
// to break the cycle that otherwise forms when storage references its
// element type.  See m3.vector.storage and m3.matrix.storage directly.
//
// m3.detail —— 重导出所有 detail 子模块（qualifier、concepts、operations）。
//
// 注意：存储子模块已与所属类型同目录，以打破存储引用元素类型时形成
// 的模块循环。直接使用 m3.vector.storage 和 m3.matrix.storage。
export module m3.detail;

export import m3.detail.qualifier;
export import m3.detail.concepts;
export import m3.detail.operations.ops;
export import m3.detail.operations.apply;
