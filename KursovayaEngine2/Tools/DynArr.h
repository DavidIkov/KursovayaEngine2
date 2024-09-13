#pragma once

typedef unsigned char byte;

/*TODO have a "bug", if you for example have array of 3 elements and insert at index 0,
and it will have to resize array then if will firstly resize array and copy everything from
old memory to new one, and only after that it will move all elements to right by 1 index,
so 3 unnecesary "responsibility constructors" will be called
*/

/*
this is using "responsibility constructor"
"responsibility constructor" have (type*) parameter only
*/
template<typename StoreType>
class DynArr {
	unsigned int Length = 0;
	unsigned int Capacity = 0;
	StoreType* Arr = nullptr;


public:

	//might be a weird name but it makes sence... right?
	class Stalker {
		friend DynArr;
		bool Deleted = false;
		DynArr<StoreType>* DynArrPtr;
		unsigned int TargetInd;
		unsigned int StalkerInd;
	public:
		Stalker() = delete;
		Stalker(const Stalker&) = delete;
		Stalker(DynArr<StoreType>* dynArrPtr, unsigned int targetInd) :
			DynArrPtr(dynArrPtr), TargetInd(targetInd) {

			if (targetInd >= dynArrPtr->Length) {
				//////////////////////////////////////MAKE DEBUGGING
				throw 1;
			}

			if (DynArrPtr->StalkersArrLength == DynArrPtr->StalkersArrCapacity) {
				DynArrPtr->StalkersArrCapacity += DynArrPtr->StalkersArrLengthExpansionStep;
				Stalker** newMem = new Stalker*[DynArrPtr->StalkersArrCapacity];
				if (DynArrPtr->StalkersArr != nullptr) {
					memcpy(newMem, DynArrPtr->StalkersArr, sizeof(Stalker*) * DynArrPtr->StalkersArrLength);
					delete[] DynArrPtr->StalkersArr;
				}
				DynArrPtr->StalkersArr = newMem;
			}

			StalkerInd = DynArrPtr->StalkersArrLength;
			DynArrPtr->StalkersArrLength++;
			*(DynArrPtr->StalkersArr + StalkerInd) = this;

		}
		~Stalker() {
			if (not Deleted) {
				Deleted = true;
				for (unsigned int i = StalkerInd + 1; i < DynArrPtr->StalkersArrLength; i++) DynArrPtr->StalkersArr[i - 1] = DynArrPtr->StalkersArr[i];
				DynArrPtr->StalkersArrLength--;
			}
		}
		StoreType* GetTarget() { return Deleted ? nullptr : (DynArrPtr->Arr + TargetInd); }
		bool gIsTargetDeleted() const { return Deleted; }
	};
private:
	Stalker** StalkersArr = nullptr;
	unsigned int StalkersArrLength = 0; unsigned int StalkersArrCapacity = 0;

	void _ClearArr() {
		if (Arr != nullptr) {
			for (unsigned int i = 0; i < Length; i++) Arr[i].~StoreType();
			delete[](byte*)Arr;
			Arr = nullptr;
		}
	}
	void _DeleteStalkers() {
		if (StalkersArr != nullptr) {
			for (unsigned int i = 0; i < StalkersArrLength; i++) StalkersArr[i]->Deleted = true;
			delete[] StalkersArr;
		}

	}
	void _CopyArrayToArray(unsigned int len, StoreType* arrFROM, StoreType* arrTO) {
		for (unsigned int i = 0; i < len; i++) new(arrTO + i) StoreType(arrFROM + i);
	}
	void _RemoveStalker(unsigned int elemInd) {
		for (int i = (int)StalkersArrLength - 1; i >= 0; i--) {
			if (StalkersArr[i]->TargetInd > elemInd) StalkersArr[i]->TargetInd--;
			else if (StalkersArr[i]->TargetInd == elemInd) StalkersArr[i]->~Stalker();
		}

	}
	void _ResizeArray(unsigned int newSize) {
		if (newSize != Capacity) {
			if (newSize == 0) {
				_ClearArr();
				Capacity = 0;
				Length = 0;
				_DeleteStalkers();
				StalkersArrLength = 0;
				StalkersArrCapacity = 0;

			}
			else {
				Capacity = newSize;
				StoreType* newArr = (StoreType*)(new byte[sizeof(StoreType) * Capacity]);
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
	//how much more space should be allocated of stalkers when there is no more space left, cant be 0
	unsigned int StalkersArrLengthExpansionStep = 1;
	
	//how much more space should be allocated of "StoreType" when there is no more space left, cant be 0
	unsigned int SizeExpansionStep = 1;

	DynArr() { }
	DynArr(const unsigned int len, const StoreType& val) {
		Arr = (StoreType*)(new byte[sizeof(StoreType) * len]);
		Length = len;
		Capacity = len;
		//this type of constructor is special for DynArr, from where and to where
		for (unsigned int i = 0; i < len; i++) new(Arr + i) StoreType(&val);
	}
	template<typename...ConstructorParametersTyp>
	DynArr(const unsigned int len, ConstructorParametersTyp&&...params) {
		Arr = (StoreType*)(new byte[sizeof(StoreType) * len]);
		Length = len;
		Capacity = len;
		for (unsigned int i = 0; i < len; i++) new(Arr + i) StoreType(std::forward<ConstructorParametersTyp>(params)...);
	}
	DynArr(const DynArr<StoreType>& arrToCopy) {
		Length = arrToCopy.Length;
		Capacity = arrToCopy.Capacity;
		Arr = (StoreType*)(new byte[sizeof(StoreType) * Capacity]);
		_CopyArrayToArray(Length, arrToCopy.Arr, Arr);
	}
	void operator=(const DynArr<StoreType>& arrToCopy) {
		_DeleteStalkers();
		StalkersArrLength = 0;
		StalkersArrCapacity = 0;
		_ClearArr();
		Length = arrToCopy.Length;
		Capacity = arrToCopy.Capacity;
		Arr = (StoreType*)(new byte[sizeof(StoreType) * Capacity]);
		_CopyArrayToArray(Length, arrToCopy.Arr, Arr);
	}
	~DynArr() {
		_ClearArr();
		_DeleteStalkers();
	}

	unsigned int gLength() { return Length; };
	unsigned int gCapacity() { return Capacity; };

	StoreType& operator[](const unsigned int ind) { return Arr[ind]; }
	const StoreType& operator[](const unsigned int ind) const { return Arr[ind]; }



private:
	void _MoveElements(unsigned int moveInd) {
		if (Length == Capacity) _ResizeArray(Capacity + SizeExpansionStep);

		if (moveInd < Length) {
			for (unsigned int i = 0; i < StalkersArrLength; i++) if (StalkersArr[i]->TargetInd >= moveInd) StalkersArr[i]->TargetInd++;
			for (int i = Length - 1; i >= (int)moveInd; i--) {
				new(Arr + i + 1) StoreType(Arr + i);
				Arr[i].~StoreType();
			};
		}
	}
public:

	template<typename...ConstructorParametersTyp>
	void InsertByConstructor(unsigned int ind, ConstructorParametersTyp&&...params) {
		_MoveElements(ind);
		new(Arr + ind) StoreType(std::forward<ConstructorParametersTyp>(params)...);
		Length++;
	}
	//the (const type*) constructor
	void InsertByResponsibilityConstructor(unsigned int ind, const StoreType& val) {
		_MoveElements(ind);
		new(Arr + ind) StoreType(&val);
		Length++;
	}

	void Remove(unsigned int ind) {

		_RemoveStalker(ind);
		
		Arr[ind].~StoreType();
		for (unsigned int i = ind + 1; i < Length; i++) {
			new(Arr + i - 1) StoreType(Arr + i);
			Arr[i].~StoreType();
		}
		Length--;
	}

	void Reserve(const unsigned int capacityAdd) {
		_ResizeArray(Capacity + capacityAdd);
	}

	void Resize(const unsigned int newSize) {
		_ResizeArray(newSize);
	}


};