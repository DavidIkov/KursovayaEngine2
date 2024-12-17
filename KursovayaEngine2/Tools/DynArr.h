#pragma once
#include"ArrayView.h"
#include"ErrorsSystem.h"

//by default after moving some object, destructor will get called for safety, if you dont want this behaviour you can disable it
template<typename T, size_t CapacityExpansionSize = 10, bool CallDestructorAfterMove = true>
class DynArr {
protected:
    using DT = unsigned char[sizeof(T)];
    size_t Len = 0; size_t Capacity = 0; 
    T* Arr = nullptr;

public:
    struct IndexWentOutOfBoundsError:KE2::ErrorsSystemNamespace::ErrorBase{};
protected:
    void _SafetyCheckForIndex(size_t ind, size_t max) const {
        if (ind >= max)
            KE2::ErrorsSystemNamespace::SendError << "Index went out of bounds" >> IndexWentOutOfBoundsError();
    }
public:

    DynArr() {}
    DynArr(size_t capacity) : Capacity(capacity), Arr((T*)new DT[capacity]) {}
	DynArr(size_t len, const T& inst) :Len(len), Capacity(len), Arr((T*)new DT[len]) { for (size_t i = 0; i < len; i++) new(Arr + i) T(inst); }
    DynArr(size_t len, T const* arr) :Len(len), Capacity(len), Arr((T*)new DT[len]) { for (size_t i = 0; i < len; i++) new(Arr + i) T(arr[i]); }
	DynArr(size_t len, T *&& arr) :Len(len), Capacity(len), Arr(arr) {}
	DynArr(const ArrayView<T>& arrView) : Len(arrView.gLen()), Capacity(arrView.gLen()), Arr((T*)new DT[Len]) { for (size_t i = 0; i < Len; i++) new(Arr + i) T(arrView[i]); }
    DynArr(ArrayView<T>&& arrView) : Len(arrView.gLen()), Capacity(arrView.gLen()), Arr(arrView.gDataPtr()) {}
    DynArr(const ArrayView<void>& arrView) : Len(arrView.gLenInBytes() / sizeof(T)), Capacity(Len), Arr((T*)new DT[Len]) { for (size_t i = 0; i < Len; i++) new(Arr + i) T(arrView[i * sizeof(T)]); }
    DynArr(ArrayView<void>&& arrView) : Len(arrView.gLenInBytes()/sizeof(T)), Capacity(Len), Arr((T*)arrView.gDataPtr()) {}
    DynArr(DynArr<T>&& toCopy) : Len(toCopy.Len), Capacity(toCopy.Capacity), Arr(toCopy.Arr) { toCopy.Arr = nullptr; toCopy.Len = 0; toCopy.Capacity = 0; }
    DynArr(const DynArr<T>& toCopy, bool copyCapacity = true) :Len(toCopy.Len), Capacity(copyCapacity ? toCopy.Capacity : toCopy.Len), Arr((T*)new DT[Capacity]) { 
        for (size_t i = 0; i < Len; i++) new(Arr + i) T(toCopy.Arr[i]); }
    virtual ~DynArr() { if (Arr != nullptr) { for (size_t i = 0; i < Len; i++) Arr[i].~T();
            delete[](void*)Arr; Arr = nullptr; Len = 0; Capacity = 0; } }

	operator ArrayView<T> () const { return ArrayView<T>(Arr, Len); }
	operator const void* () const { return Arr; }
	operator void* () { return Arr; }
	operator const T* () const { return Arr; }
	operator T* () { return Arr; }
    T* operator*() { return Arr; }
    T const* operator*() const { return Arr; }


    virtual DynArr<T>& operator=(DynArr<T>&& toCopy) { this->~DynArr(); new(this) DynArr(std::move(toCopy)); return *this; }
    DynArr<T>& operator=(const DynArr<T>& toCopy) { this->~DynArr(); new(this) DynArr(toCopy); return *this; }

    T& operator[](size_t ind) noexcept { _SafetyCheckForIndex(ind, Len); return Arr[ind]; }
    const T& operator[](size_t ind) const noexcept { _SafetyCheckForIndex(ind, Len); return Arr[ind]; }

    size_t gLen() const noexcept { return Len; }
    size_t gCapacity() const noexcept { return Capacity; }

    template<typename...ConstrTypes>
    void InsertAtIndex(size_t ind, ConstrTypes&&...constrParams) {

        _SafetyCheckForIndex(ind, Len + 1);

        if (Len == Capacity) {
            Capacity += CapacityExpansionSize;
            T* oldArr = Arr;
            Arr = (T*)new DT[Capacity];
            for (size_t i = 0; i < ind; i++) { new(Arr + i) T(std::move(oldArr[i])); if constexpr (CallDestructorAfterMove) oldArr[i].~T(); }
            new(Arr + ind) T(std::forward<ConstrTypes>(constrParams)...);
            for (size_t i = ind; i < Len; i++) { new(Arr + i + 1) T(std::move(oldArr[i])); if constexpr (CallDestructorAfterMove) oldArr[i].~T(); }
            Len++;
            if (oldArr != nullptr) delete[](void*)oldArr;
        }
        else {
            if (ind != Len) {
                new(Arr + Len) T(std::move(Arr[Len - 1]));
                for (size_t i = Len - 1;; i--) { 
                    if constexpr (CallDestructorAfterMove) Arr[i].~T();
                    if (i == ind) break;
                    new(Arr + i) T(std::move(Arr[i - 1])); 
                }
            }
            new(Arr + ind) T(std::forward<ConstrTypes>(constrParams)...);
            Len++;
        }

    }

	template<typename...ConstrTypes>
    void InsertAtEnd(ConstrTypes&&...constrParams) { InsertAtIndex(Len, std::forward<ConstrTypes>(constrParams)...); }

    void ChangeCapacity(size_t newCapacity) {
        if (newCapacity == Capacity) return;
        else if (newCapacity == 0) this->~DynArr();
        else {
			T* oldArr = Arr;
			Arr = (T*)new DT[newCapacity];
            if (newCapacity < Len) {
                for (size_t i = newCapacity; i < Len; i++) oldArr[i].~T();
                Len = newCapacity;
            }
			for (size_t i = 0; i < Len; i++) { new(Arr + i) T(std::move(oldArr[i])); if constexpr (CallDestructorAfterMove) oldArr[i].~T(); }
            if (oldArr != nullptr) delete[](void*)oldArr;
			Capacity = newCapacity;
        }
    }
    //be aware that if you expand size then new memory area wont change any values that already been in this memory sector,
    //so if you change length of array and dont fill it, it can result in UB when move constructors/copy constructors/destructors will get called
    //for example if you have array of 10 elements, doing somnething like myArr.ChangeLen(20); ...somecode; myArr.ChangeLen(10); can result in UB 
    // since destructors will have to be called for 10 of those elements
    //instead use myArr.ChangeLen(20); ...someCode; myArr.CutDeadLength(10);
    void ChangeLen(size_t newLen) {
        if (newLen == Len) return;
        else if (newLen > Len) {
            if (newLen <= Capacity) Len = newLen;
            else { ChangeCapacity(newLen); Len = newLen; }
        }
        else {
            for (size_t i = newLen; i < Len; i++) Arr[i].~T();
            Len = newLen;
        }
    }
    //elements that will fall into length of cut will NOT get destroyed, destructors wont be called since these elements considered "dead"
    //used to avoid UB for calling destructors of "dead" elements
    void CutDeadLength(size_t newLen) {
        if (newLen == Len) return;
        _SafetyCheckForIndex(newLen, Len);
        Len = newLen;
    }
    template<typename...ParamsTyps>
    void ReplaceAtIndex(size_t ind, ParamsTyps&&...params) {
        _SafetyCheckForIndex(ind, Len);
        Arr[ind].~T(); new(Arr + ind) T(std::forward<ParamsTyps>(params)...);
    }
    void RemoveAtIndex(size_t ind) {
        RemoveRegion(ind, 1);
    }
    void RemoveRegion(size_t startInd, size_t lenOfRegion) {

        _SafetyCheckForIndex(startInd, Len - lenOfRegion + 1);

        for (size_t i = startInd; i < startInd + lenOfRegion; i++) Arr[i].~T();

        for (size_t i = startInd; i < Len - lenOfRegion; i++) {
            new(Arr + i) T(std::move(Arr[i + lenOfRegion]));
            if constexpr (CallDestructorAfterMove) Arr[i + lenOfRegion].~T();
        }

        Len -= lenOfRegion;

    }
    void ExpandAtIndex(size_t ind, size_t expansionSize) {
        //todo
    }

};
