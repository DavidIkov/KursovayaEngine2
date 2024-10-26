#include"GetOverlappingMemoryRegion.h"
void GetOverlappingMemoryRegion(void const** writeOverlapStart, unsigned int* writeOverlapLengthInBytes,
	const void* startOfRegion1, unsigned int lenOfRegion1InBytes, const void* startOfRegion2, unsigned int lenOfRegion2InBytes) {

	if (startOfRegion1 == nullptr or startOfRegion2 == nullptr) return;
	else if (startOfRegion1 == startOfRegion2) {
		*writeOverlapStart = startOfRegion1;
		*writeOverlapLengthInBytes = (lenOfRegion1InBytes > lenOfRegion2InBytes) ? lenOfRegion2InBytes : lenOfRegion1InBytes;
		return;
	}
	else if (startOfRegion1 > startOfRegion2 and startOfRegion1 < ((unsigned char*)startOfRegion2 + lenOfRegion2InBytes)) {
		*writeOverlapStart = startOfRegion1;
		*writeOverlapLengthInBytes = lenOfRegion2InBytes - (unsigned int)((unsigned char*)startOfRegion2 - (unsigned char*)startOfRegion1);
		if (*writeOverlapLengthInBytes > lenOfRegion1InBytes) *writeOverlapLengthInBytes = lenOfRegion1InBytes;
		return;
	}
	else if (startOfRegion2 > startOfRegion1 and startOfRegion2 < ((unsigned char*)startOfRegion1 + lenOfRegion1InBytes)) {
		*writeOverlapStart = startOfRegion2;
		*writeOverlapLengthInBytes = lenOfRegion1InBytes - (unsigned int)((unsigned char*)startOfRegion1 - (unsigned char*)startOfRegion2);
		if (*writeOverlapLengthInBytes > lenOfRegion2InBytes) *writeOverlapLengthInBytes = lenOfRegion2InBytes;
		return;
	}
	*writeOverlapStart = nullptr;
	*writeOverlapLengthInBytes = 0;
}
