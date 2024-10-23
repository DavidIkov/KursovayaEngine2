#include"AnonDynArr.h"



AnonDynArr::AnonDynArr() {}
AnonDynArr::AnonDynArr(byte* data, unsigned int dataSizeInBytes) :
	Arr(data),ArrSizeInBytes(dataSizeInBytes) {}
AnonDynArr::AnonDynArr(const AnonDynArr&& toCopy) :
	Arr(toCopy.Arr), ArrSizeInBytes(toCopy.ArrSizeInBytes) {
	toCopy.Deleted = true;
}
AnonDynArr::~AnonDynArr() {
	if (not Deleted) {
		Deleted = true;
		if (Arr != nullptr) delete[] Arr;
	}
}
void AnonDynArr::operator=(const AnonDynArr&& toCopy) {
	this->~AnonDynArr();
	Deleted = false;
	toCopy.Deleted = true;
	Arr = toCopy.Arr;
	ArrSizeInBytes = toCopy.ArrSizeInBytes;
}
void AnonDynArr::SetData(byte* data, unsigned int dataSizeInBytes) {
	if (Arr != nullptr) delete[] Arr;
	Arr = data;
	ArrSizeInBytes = dataSizeInBytes;
}
void AnonDynArr::SetSubData(unsigned int offsetInBytes, byte* data, unsigned int dataSizeInBytes) {
	memcpy(Arr + offsetInBytes, data, dataSizeInBytes);
}
void AnonDynArr::DeleteData() {
	if (Arr != nullptr) {
		delete[] Arr;
		Arr = nullptr;
		ArrSizeInBytes = 0;
	}
}

unsigned int AnonDynArr::gArrSizeInBytes() const {
	return ArrSizeInBytes;
}
byte* AnonDynArr::gArr() {
	return Arr;
}
byte const* AnonDynArr::gArr() const {
	return Arr;
}
