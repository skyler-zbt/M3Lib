// m3.matrix — matrix type, base class, and operator aggregation hub.
// Re-exports base, mat, and the :operators partition so that consumers
// can import m3.matrix to get the complete Mat API including arithmetic
// and matrix products.
//
// m3.matrix —— 矩阵类型、基类和运算符的聚合枢纽。
// 重导出 base、mat 和 :operators 分区，使用者只需 import m3.matrix
// 即可获得完整的 Mat API（含算术运算和矩阵乘法）。
export module m3.matrix;

export import m3.matrix.base;
export import m3.matrix.mat;
export import :operators;
