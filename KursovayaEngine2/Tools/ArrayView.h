#pragma once
#include<vector>

template<typename T>
class ArrayView {
    template<typename>
    friend class ArrayView;
    T* Data; size_t Len;
public:
    constexpr ArrayView(T* data, size_t len) noexcept:Data(data), Len(len) {};
    ArrayView(const std::vector<T>& vec) noexcept:Data((T*)&* vec.begin()), Len(vec.size()) {}
    template<size_t L> constexpr ArrayView(T (&&arr)[L]) noexcept:Data(arr), Len(L) {};
	template<size_t L> constexpr ArrayView(T (&arr)[L]) noexcept:Data(arr), Len(L) {};

    size_t gLen() const noexcept { return Len; };
    T& operator[](size_t ind) noexcept { return Data[ind]; };
    T const& operator[](size_t ind) const noexcept { return Data[ind]; };
	T* gDataPtr() noexcept { return Data; }
    T const* gDataPtr() const noexcept { return Data; }
    operator std::vector<T>() const && noexcept = delete;
    operator std::vector<T>() const & { return std::vector<T>(Data, Data + Len); }


};
template<>
class ArrayView<void> {
    void* Data; size_t LenInBytes;
public:
    template<typename T> ArrayView(const std::vector<T>& vec) :Data((void*)&*vec.begin()), LenInBytes(vec.size() * sizeof(T)) {}
    template<typename T> ArrayView(ArrayView<T> data) : Data(data.Data), LenInBytes(data.Len * sizeof(T)) {};
    ArrayView(void* data, size_t lenInBytes) :Data(data), LenInBytes(lenInBytes) {};
    size_t gLenInBytes() const { return LenInBytes; };
    void* operator[](size_t byteOff) { return (unsigned char*)Data + byteOff; };
    void const* operator[](size_t byteOff) const { return (unsigned char*)Data + byteOff; };
    void* gDataPtr() { return Data; }
    void const* gDataPtr() const { return Data; }
};
