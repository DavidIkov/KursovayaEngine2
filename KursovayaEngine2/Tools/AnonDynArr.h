#pragma once
#include<utility>
#include<cstring>
#include"DLL.h"
typedef unsigned char byte;


class AnonDynArr {
	byte* Arr = nullptr;
	unsigned int ArrSizeInBytes = 0;
	mutable bool Deleted = false;
public:
	template<typename Type> struct TypeContainer {};
	DLLTREATMENT AnonDynArr();
	//takes responsibility for deleting "data", "data" should be allocated with new as array
	DLLTREATMENT AnonDynArr(byte* data, unsigned int dataSizeInBytes);

	template<typename Type>
	AnonDynArr(const AnonDynArr& toCopy, TypeContainer<Type>) :ArrSizeInBytes(toCopy.ArrSizeInBytes) {
		if (ArrSizeInBytes == 0) return;
		alignas(Type) byte* adress[ArrSizeInBytes](0);
		Arr = new(adress) byte[ArrSizeInBytes];
		memcpy(Arr, toCopy.Arr, ArrSizeInBytes);
	}

	DLLTREATMENT AnonDynArr(const AnonDynArr&& toCopy);
	DLLTREATMENT ~AnonDynArr();

	template<typename Type>
	void operator=(const AnonDynArr& toCopy) {
		if (Arr != nullptr) delete[] Arr;
		ArrSizeInBytes = toCopy.ArrSizeInBytes;
		alignas(Type) byte* adress[ArrSizeInBytes](0);
		Arr = new(adress) byte[ArrSizeInBytes];
	}

	DLLTREATMENT void operator=(const AnonDynArr&& toCopy);
	//takes responsibility for deleting "data", "data" should be allocated with new as array
	//be aware that "data" should not overlap with "Arr", otherwise undefined behaviour
	DLLTREATMENT void SetData(byte* data, unsigned int dataSizeInBytes);
	//will copy "data" and wont take responsibility
	DLLTREATMENT void SetSubData(unsigned int offsetInBytes, byte* data, unsigned int dataSizeInBytes);
	DLLTREATMENT void DeleteData();
	DLLTREATMENT unsigned int gArrSizeInBytes() const;
	DLLTREATMENT byte* gArr();
	DLLTREATMENT byte const* gArr() const;
};
