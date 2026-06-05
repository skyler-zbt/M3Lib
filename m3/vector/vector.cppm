// m3.vector — vector type, base class, and operator aggregation hub.
// Re-exports base, vec, and the :operators partition so that consumers can
// import m3.vector to get the complete Vec API including arithmetic.
//
// m3.vector —— 向量类型、基类和运算符的聚合枢纽。
// 重导出 base、vec 和 :operators 分区，使用者只需 import m3.vector
// 即可获得完整的 Vec API（含算术运算）。
export module m3.vector;

export import m3.vector.base;
export import m3.vector.vec;
export import :operators;
