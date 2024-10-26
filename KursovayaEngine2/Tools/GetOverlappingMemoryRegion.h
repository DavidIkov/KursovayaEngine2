#pragma once
void GetOverlappingMemoryRegion(void const** writeOverlapStart, unsigned int* writeOverlapLengthInBytes,
	const void* startOfRegion1, unsigned int lenOfRegion1InBytes, const void* startOfRegion2, unsigned int lenOfRegion2InBytes);
