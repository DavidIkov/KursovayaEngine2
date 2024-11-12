#pragma once
template<typename Type>
class ArrayView {
    template<typename>
    friend class ArrayView;
    Type* Data; unsigned int Len;
public:
    ArrayView(Type* data, unsigned int len) :Data(data), Len(len) {};
    template<unsigned int L>
    ArrayView(Type (&&arr)[L]) :Data(arr), Len(L) {};
	template<unsigned int L>
    ArrayView(Type (&arr)[L]) :Data(arr), Len(L) {};

    unsigned int gLen() const { return Len; };
    Type& operator[](unsigned int ind) { return Data[ind]; };
    Type const& operator[](unsigned int ind) const { return Data[ind]; };
	Type* gDataPtr() { return Data; }
    Type const* gDataPtr() const { return Data; }
};
template<>
class ArrayView<void> {
    void* Data; unsigned int LenInBytes;
public:
    template<typename T>
    ArrayView(ArrayView<T> data) :Data(data.Data), LenInBytes(data.Len * sizeof(T)) {};
    ArrayView(void* data, unsigned int lenInBytes) :Data(data), LenInBytes(lenInBytes) {};
    unsigned int gLenInBytes() const { return LenInBytes; };
    void* operator[](unsigned int byteOff) { return (unsigned char*)Data + byteOff; };
    void const* operator[](unsigned int byteOff) const { return (unsigned char*)Data + byteOff; };
    void* gDataPtr() { return Data; }
    void const* gDataPtr() const { return Data; }
};
