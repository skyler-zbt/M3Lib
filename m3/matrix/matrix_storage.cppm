
export module m3.matrix.storage;

import std;

import m3.detail.qualifier;
import m3.detail.alignment_traits;
import m3.vector.vec;

export namespace m3::detail {

template <int C, int R, typename T, Qualifier Q>
struct alignas(AlignmentTraits<Q, T>::value) MatrixStorage {
    std::array<Vec<R, T, Q>, C> columns_{};
    MatrixStorage() = default;
};

}