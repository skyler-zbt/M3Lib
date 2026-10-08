
export module m3.detail.concepts;

import std;

export namespace m3::detail {
template <typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

template <typename T>
concept FloatingPoint = std::floating_point<T>;

template <int L>
concept ValidVecDimension = (L >= 1 && L <= 4);

template <typename V>
concept VectorLike = requires {
    typename V::value_type;
    typename V::qualifier_type;
    { V::dimension } -> std::convertible_to<int>;
    requires std::default_initializable<V>;
    requires requires(V& v, const V& cv, std::size_t i) {
        { v[i] } -> std::same_as<typename V::value_type&>;
        { cv[i] } -> std::same_as<const typename V::value_type&>;
    };
};

template <typename M>
concept MatrixLike = requires {
    typename M::value_type;
    typename M::column_type;
    requires std::same_as<typename M::column_type::value_type, typename M::value_type>;
    typename M::qualifier_type;
    requires std::same_as<decltype(M::columns), const int>;
    requires std::same_as<decltype(M::rows), const int>;
    requires std::default_initializable<M>;
    requires Arithmetic<typename M::value_type>;
    requires requires(M& m, const M& cm, std::size_t i) {
        { m[i] } -> std::same_as<typename M::column_type&>;
        { cm[i] } -> std::same_as<const typename M::column_type&>;
    };
};

template <int C, int R>
concept ValidMatrixSize = (C >= 2 && C <= 4) && (R >= 2 && R <= 4);

template <typename V>
using element_ref_t = std::remove_reference_t<decltype(std::declval<V&>()[std::size_t{}])>;

template <typename Op, typename T>
concept BinaryOp =
    std::regular_invocable<Op, T, T> && std::convertible_to<std::invoke_result_t<Op, T, T>, T>;

template <typename Op, typename T>
concept UnaryOp =
    std::regular_invocable<Op, T> && std::convertible_to<std::invoke_result_t<Op, T>, T>;
}
