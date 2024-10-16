#pragma once
#include<utility>
#include<cstring>
#include"DebuggingTools.h"
typedef unsigned char byte;

template<typename>
class DynArr;


//might be a weird name but it makes sence... right?
class StalkerClass {
	template<typename>
	friend class DynArr;

	struct DynArrStalkersDataClass {
		StalkerClass** StalkersArr = nullptr;
		unsigned int StalkersArrLength = 0; unsigned int StalkersArrCapacity = 0;
		//how much more space should be allocated of stalkers when there is no more space left, cant be 0
		unsigned int StalkersArrLengthExpansionStep = 1;

	};

	mutable bool Deleted = false;

	DynArrStalkersDataClass* StalkersDataPtr;

	byte** ArrayPtr;
	unsigned int TypeSize;

	unsigned int TargetInd;
	unsigned int StalkerInd;
	void _TargetStalker() {
		if (StalkersDataPtr->StalkersArrLength == StalkersDataPtr->StalkersArrCapacity) {
			StalkersDataPtr->StalkersArrCapacity += StalkersDataPtr->StalkersArrLengthExpansionStep;
			StalkerClass** newMem = new StalkerClass*[StalkersDataPtr->StalkersArrCapacity];
			if (StalkersDataPtr->StalkersArr != nullptr) {
				memcpy(newMem, StalkersDataPtr->StalkersArr, sizeof(StalkerClass*) * StalkersDataPtr->StalkersArrLength);
				delete[] StalkersDataPtr->StalkersArr;
			}
			StalkersDataPtr->StalkersArr = newMem;
		}

		StalkerInd = StalkersDataPtr->StalkersArrLength;
		StalkersDataPtr->StalkersArrLength++;
		StalkersDataPtr->StalkersArr[StalkerInd] = this;
	}
public:
	StalkerClass() = delete;
	StalkerClass(const StalkerClass& toCopy) :
		StalkersDataPtr(toCopy.StalkersDataPtr), ArrayPtr(toCopy.ArrayPtr), TypeSize(toCopy.TypeSize), TargetInd(toCopy.TargetInd) {
		_TargetStalker();
	}
	StalkerClass(const StalkerClass&& toCopy) {
		memcpy(this, &toCopy, sizeof(StalkerClass));
		toCopy.Deleted = true;
		StalkersDataPtr->StalkersArr[StalkerInd] = this;
	}
	template<typename ArrType>
	StalkerClass(DynArr<ArrType>* DynArrPtr, unsigned int targetInd) :
		StalkersDataPtr(&DynArrPtr->StalkersData), ArrayPtr((byte**)&DynArrPtr->Arr), TypeSize(sizeof(ArrType)), TargetInd(targetInd) {
		_TargetStalker();
	}
	~StalkerClass() {
		if (not Deleted) {
			Deleted = true;
			for (unsigned int i = StalkerInd + 1; i < StalkersDataPtr->StalkersArrLength; i++) StalkersDataPtr->StalkersArr[i - 1] = StalkersDataPtr->StalkersArr[i];
			StalkersDataPtr->StalkersArrLength--;
		}
	}
	void operator=(const StalkerClass&& toCopy) {
		this->~StalkerClass();
		Deleted = false;
		toCopy.Deleted = true;
		memcpy(this, &toCopy, sizeof(StalkerClass));
	}
	template<typename Type>
	Type& GetTarget() const { return *(Type*)(Deleted ? nullptr : (*ArrayPtr + TypeSize * TargetInd)); }
	unsigned int gTargetInd() { return TargetInd; }
	bool gIsTargetDeleted() const { return Deleted; }
};

/*TODO have a "bug", if you for example have array of 3 elements and insert at index 0,
and it will have to resize array then if will firstly resize array and copy everything from
old memory to new one, and only after that it will move all elements to right by 1 index,
so 3 unnecesary "responsibility constructors" will be called
*/

template<typename StoreType>
class DynArr {

	static_assert(std::is_constructible_v<StoreType, StoreType&&>, "move constructor is not defined in current type, but its required");

	friend class StalkerClass;

	unsigned int Length = 0;
	unsigned int Capacity = 0;
	StoreType* Arr = nullptr;

private:

	struct alignas(StoreType) StoreTypeDummyStruct { unsigned char _[sizeof(StoreType)]; };

	StalkerClass::DynArrStalkersDataClass StalkersData;

	void _CheckIfIndexInBounds(unsigned int ind, unsigned int maxInd) {
#if defined KE2_Debug
		if (ind > maxInd)
			DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical, "index went out of bounds", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_FUNCTION_WITH_INVALID_ARGUMENTS });
#endif
	}

	void _ClearArr() {
		if (Arr != nullptr) {
			for (unsigned int i = 0; i < Length; i++) Arr[i].~StoreType();
			delete[] (void*)Arr;
			Arr = nullptr;
		}
	}
	void _DeleteStalkers() {
		if (StalkersData.StalkersArr != nullptr) {
			for (unsigned int i = 0; i < StalkersData.StalkersArrLength; i++) StalkersData.StalkersArr[i]->Deleted = true;
			delete[] StalkersData.StalkersArr;
			StalkersData.StalkersArr = nullptr;
		}
	}
	void _CopyArrayToArray(unsigned int len, StoreType* arrFROM, StoreType* arrTO) {
		for (unsigned int i = 0; i < len; i++) new(arrTO + i) StoreType(std::move(*(arrFROM + i)));
	}
	void _RemoveStalker(unsigned int elemInd) {
		for (int i = (int)StalkersData.StalkersArrLength - 1; i >= 0; i--) {
			if (StalkersData.StalkersArr[i]->TargetInd > elemInd) StalkersData.StalkersArr[i]->TargetInd--;
			else if (StalkersData.StalkersArr[i]->TargetInd == elemInd) StalkersData.StalkersArr[i]->~StalkerClass();
		}

	}
	void _ResizeArrayMemory(unsigned int newSize) {
		if (newSize != Capacity) {
			if (newSize == 0) {
				_ClearArr();
				Capacity = 0;
				Length = 0;
				_DeleteStalkers();
				StalkersData.StalkersArrLength = 0;
				StalkersData.StalkersArrCapacity = 0;

			}
			else {
				Capacity = newSize;
				StoreType* newArr = (StoreType*)(new StoreTypeDummyStruct[Capacity]);
				if (Capacity < Length) {

					//disconnect some stalkers
					//Length cant be 0 here so 0u-1 wont be a problem
					for (int ti = Length - 1; ti >= (int)Capacity; ti--)
						_RemoveStalker(ti);

					_CopyArrayToArray(Capacity, Arr, newArr);
					Length = Capacity;
				}
				else _CopyArrayToArray(Length, Arr, newArr);
				_ClearArr();
				Arr = newArr;
			}
		}
	}


public:
	
	//how much more space should be allocated of "StoreType" when there is no more space left, cant be 0
	unsigned int SizeExpansionStep = 5;

	DynArr() { }
	DynArr(const unsigned int len, const StoreType& val) {
		Arr = (StoreType*)(new StoreTypeDummyStruct[len]);
		Length = len;
		Capacity = len;
		//this type of constructor is special for DynArr, from where and to where
		for (unsigned int i = 0; i < len; i++) new(Arr + i) StoreType(std::move(val));
	}
	template<typename...ConstructorParametersTyp>
	DynArr(const unsigned int len, ConstructorParametersTyp&&...params) {
		Arr = (StoreType*)(new StoreTypeDummyStruct[len]);
		Length = len;
		Capacity = len;
		for (unsigned int i = 0; i < len; i++) new(Arr + i) StoreType(std::forward<ConstructorParametersTyp>(params)...);
	}
	DynArr(const DynArr<StoreType>& arrToCopy) {
		Length = arrToCopy.Length;
		Capacity = arrToCopy.Capacity;
		Arr = (StoreType*)(new StoreTypeDummyStruct[Capacity]);
		_CopyArrayToArray(Length, arrToCopy.Arr, Arr);
	}
	void operator=(const DynArr<StoreType>& arrToCopy) {
		_DeleteStalkers();
		StalkersData.StalkersArrLength = 0;
		StalkersData.StalkersArrCapacity = 0;
		_ClearArr();
		Length = arrToCopy.Length;
		Capacity = arrToCopy.Capacity;
		Arr = (StoreType*)(new StoreTypeDummyStruct[Capacity]);
		_CopyArrayToArray(Length, arrToCopy.Arr, Arr);
	}
	~DynArr() {
		_ClearArr();
		_DeleteStalkers();
	}

	unsigned int gLength() { return Length; };
	unsigned int gCapacity() { return Capacity; };

	StoreType& operator[](const unsigned int ind) { _CheckIfIndexInBounds(ind, Length - 1); return Arr[ind]; }
	const StoreType& operator[](const unsigned int ind) const { _CheckIfIndexInBounds(ind, Length - 1); return Arr[ind]; }



private:
	void _MoveElements(unsigned int moveInd) {
		if (Length == Capacity) _ResizeArrayMemory(Capacity + SizeExpansionStep);

		if (moveInd < Length) {
			for (unsigned int i = 0; i < StalkersData.StalkersArrLength; i++) if (StalkersData.StalkersArr[i]->TargetInd >= moveInd) StalkersData.StalkersArr[i]->TargetInd++;
			for (int i = Length - 1; i >= (int)moveInd; i--) {
				new(Arr + i + 1) StoreType(std::move(*(Arr + i)));
				Arr[i].~StoreType();
			};
		}

		Length++;
	}
public:

	template<typename...ConstructorParametersTyp>
	void InsertByConstructor(unsigned int ind, ConstructorParametersTyp&&...params) {
		_CheckIfIndexInBounds(ind, Length);
		_MoveElements(ind);
		new(Arr + ind) StoreType(std::forward<ConstructorParametersTyp>(params)...);
	}
	/*void InsertByResponsibilityConstructor(unsigned int ind, const StoreType& val) {
		_MoveElements(ind);
		_MoveInstance(&val, Arr + ind);
		Length++;
	}*/

	void Remove(unsigned int ind) {
		_CheckIfIndexInBounds(ind, Length - 1);

		_RemoveStalker(ind);
		
		Arr[ind].~StoreType();
		for (unsigned int i = ind + 1; i < Length; i++) {
			new(Arr + i - 1) StoreType(std::move(*(Arr + i)));
			Arr[i].~StoreType();
		}
		Length--;
	}

	void ChangeCapacity(const unsigned int newCapacity) {
		_ResizeArrayMemory(newCapacity);
	}


	/*template<typename...ConstructorParametersTyp>
	void Resize(const unsigned int newSize, ConstructorParametersTyp&&...params) {
		unsigned int oldLen = Length;
		_ResizeArrayMemory(newSize);
		if (newSize > oldLen) for (unsigned int i=oldLen;i<newSize;i++) new(Arr + i) StoreType(std::forward<ConstructorParametersTyp>(params)...);
		Length = newSize;
	}*/
	void Clear() {
		_ResizeArrayMemory(0);
	}

	bool IsStalkersAmountForIndexEqualTo(unsigned int ind, unsigned int toCompare) {
		_CheckIfIndexInBounds(ind, Length - 1);

		unsigned int curAmount = 0;
		if (toCompare == 0) return true;
		for (unsigned int i = 0; i < StalkersData.StalkersArrLength; i++)
			if (StalkersData.StalkersArr[i]->TargetInd == ind) if (++curAmount > toCompare) return false;
		return curAmount == toCompare;
	}

};