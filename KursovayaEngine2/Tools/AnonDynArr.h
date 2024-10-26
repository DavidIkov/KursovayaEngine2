#pragma once
#include<utility>
#include<cstring>
#include"DLL.h"
#include"AlignmentDummyClass.h"
#include"GetOverlappingMemoryRegion.h"
#include"DebuggingTools.h"
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
		Arr = (byte*)(new AlignmentDummyClass<Type>[ArrSizeInBytes / sizeof(Type)]);
		memcpy(Arr, toCopy.Arr, ArrSizeInBytes);
	}

	DLLTREATMENT AnonDynArr(const AnonDynArr&& toCopy);
	DLLTREATMENT ~AnonDynArr();

	template<typename Type>
	//if memory overlaps then undefined behaviour
	void operator=(const AnonDynArr& toCopy) {
#if defined KE2_Debug
		byte* overlapStart = nullptr; unsigned int overlapLen = 0;
		GetOverlappingMemoryRegion((void const**)&overlapStart, &overlapLen, Arr, ArrSizeInBytes, toCopy.Arr, toCopy.ArrSizeInBytes);
		if (overlapStart != nullptr) {
			DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical,
				"memory is overlaping",
				KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION });
			return;
		}
#endif
		if (Arr != nullptr) delete[] Arr;
		ArrSizeInBytes = toCopy.ArrSizeInBytes;
		Arr = (byte*)(new AlignmentDummyClass<Type>[ArrSizeInBytes / sizeof(Type)]);
		memcpy(Arr, toCopy.Arr, ArrSizeInBytes);
	}
	
	DLLTREATMENT void operator=(const AnonDynArr&& toCopy);
	//takes responsibility for deleting "data", "data" should be allocated with new as array
	//if memory is overlaping then undefined behaviour
	DLLTREATMENT void SetData(byte* data, unsigned int dataSizeInBytes);
	//will copy "data" and wont take responsibility
	DLLTREATMENT void SetSubData(unsigned int offsetInBytes, byte* data, unsigned int dataSizeInBytes);
	DLLTREATMENT void DeleteData();
	DLLTREATMENT unsigned int gArrSizeInBytes() const;
	DLLTREATMENT byte* gArr();
	DLLTREATMENT byte const* gArr() const;
};
