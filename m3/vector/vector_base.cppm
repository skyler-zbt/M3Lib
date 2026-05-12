export module m3.vector.base;

import std;

import m3.detail;

export namespace m3{
    // basic vector template
    template <int L, Arithmetic T, Qualifier Q = Qualifier::aligned_highp>
    requires ValidDimension<L>
    struct VectorBase{
        VectorStorage<L, T> storage;

        constexpr std::array<T, L>& data() noexcept;
        constexpr const std::array<T, L>& data() const noexcept;

        constexpr T& operator[](std::size_t i);
        constexpr const T& operator[](std::size_t i) const;

        static constexpr int length() noexcept;
    };

    constexpr std::array<T, L>& VectorBase<L,T,Q>::data() noexcept{
        return storage.data;
    }

    constexpr const std::array<T, L>& VectorBase<L,T,Q>::data() const noexcept{
        return storage.data;
    }

    constexpr T& VectorBase<L,T,Q>::operator[](std::size_t i){
        return storage.data[i];
    }

    constexpr const T& VectorBase<L,T,Q>::operator[](std::size_t i) const{
        return storage.data[i];
    }

    static constexpr int VectorBase<L,T,Q>::length() noexcept{
        return L;
    }
}