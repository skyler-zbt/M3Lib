export module m3.detail.storage.vector_storage;

import std;
import m3.detail.qualifier;
import m3.detail.alignment_traits;

export namespace m3 {
    // storage 
    template<int L, typename T, Qualifier Q>
    struct alignas(AlignmentTraits<Q>::value) VectorStorage
    {
        std::array<T, L> data;

        VectorStorage() = default;

        explicit constexpr VectorStorage(const T& v);

        constexpr T& operator[](std::size_t i);
        constexpr const T& operator[](std::size_t i) const;

        static constexpr int length() noexcept;
    };


    template<int L, typename T, Qualifier Q>
    constexpr VectorStorage<L, T, Q>::VectorStorage(const T &v) {
        for (auto& elem : data) {
            elem = v;
        }
    }

    template<int L, typename T, Qualifier Q>
    constexpr T & VectorStorage<L, T, Q>::operator[](std::size_t i) {
        return this->data[i];
    }

    template<int L, typename T, Qualifier Q>
    constexpr const T & VectorStorage<L, T, Q>::operator[](std::size_t i)const{
        return this->data[i];
    }

    template<int L, typename T, Qualifier Q>
    constexpr int VectorStorage<L, T, Q>::length() noexcept {
        return L;
    }
} // namespace m3