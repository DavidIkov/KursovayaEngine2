#pragma once
typedef unsigned char byte;
template<typename StoreType>
class DynArr {
	unsigned int Length = 0;
	unsigned int Capacity = 0;
	StoreType* Arr = nullptr;

	void _ClearArr() {
		if (Arr != nullptr) {
			for (unsigned int i = 0; i < Length; i++) Arr[i].~StoreType();
			delete[](byte*)Arr;
		}
	}
	void _CopyArrayToArray(unsigned int len, StoreType* arrFROM, StoreType* arrTO) {
		for (unsigned int i = 0; i < len; i++) new(arrTO + i) StoreType(arrFROM + i);
	}
	void _ResizeArray(unsigned int newSize) {
		if (newSize != Capacity) {
			if (newSize == 0) {
				_ClearArr();
				Arr = nullptr;
				Capacity = 0;
				Length = 0;
			}
			else {
				Capacity = newSize;
				StoreType* newArr = (StoreType*)(new byte[sizeof(StoreType) * Capacity]);
				if (Capacity < Length) {
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

	//how much space of class will be added on every resize of array, cant be 0
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
		_ClearArr();
		Length = arrToCopy.Length;
		Capacity = arrToCopy.Capacity;
		Arr = (StoreType*)(new byte[sizeof(StoreType) * Capacity]);
		_CopyArrayToArray(Length, arrToCopy.Arr, Arr);
	}
	~DynArr() {
		_ClearArr();
	}

	unsigned int gLength() { return Length; };
	unsigned int gCapacity() { return Capacity; };

	StoreType& operator[](const unsigned int ind) { return Arr[ind]; }
	const StoreType& operator[](const unsigned int ind) const { return Arr[ind]; }

	template<typename...ConstructorParametersTyp>
	void InsertByConstructor(unsigned int ind, ConstructorParametersTyp&&...params) {
		if (Length == Capacity) _ResizeArray(Capacity + SizeExpansionStep);

		if (ind < Length)
			for (unsigned int i = Length - 1;; i--) {
				new(Arr + i + 1) StoreType(Arr + i);
				Arr[i].~StoreType();
				if (i == ind) break;
			}
		new(Arr + ind) StoreType(std::forward<ConstructorParametersTyp>(params)...);
		Length++;
	}
	void InsertByCopy(unsigned int ind, const StoreType& val) {
		if (Length == Capacity) _ResizeArray(Capacity + SizeExpansionStep);

		if (ind < Length)
			for (unsigned int i = Length - 1;; i--) {
				new(Arr + i + 1) StoreType(Arr + i);
				Arr[i].~StoreType();
				if (i == ind) break;
			}
		new(Arr + ind) StoreType(&val);
		Length++;
	}

	void Remove(unsigned int ind) {
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