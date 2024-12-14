#pragma once
#include<utility>

///if function returns arrLen+1 then it failed for some reason
///will return index of equal value if it exists
///compareFunc should check if for example p1<p2, p1 and p2 are in order of array
///equalFunc should check if params are equal
template<typename ArrType, typename ValType>
size_t BinarySearch(const ArrType* arr, size_t arrLen, ValType&& val, 
	bool(*compareFunc)(const std::remove_reference_t<ValType>&, const ArrType&),
	bool(*equalFunc)(const std::remove_reference_t<ValType>&, const ArrType&)) {

	if (arrLen == 0) return 1;
	
	size_t start = 0; size_t end = arrLen - 1;
	while (true) {
		size_t ind = start + (end - start) / 2;
		if (compareFunc(val, arr[ind])) {
			if (ind == 0) return 0;
			else if (not compareFunc(val, arr[ind - 1])) return equalFunc(val, arr[ind - 1]) ? (ind - 1) : ind;
			else {
				end = ind - 1;
				continue;
			}
		}
		else {
			if (ind == arrLen - 1) return equalFunc(val, arr[ind]) ? ind : arrLen;
			else if (compareFunc(val, arr[ind + 1])) return equalFunc(val, arr[ind]) ? ind : (ind + 1);
			else {
				start = ind + 1;
				continue;
			}
		}
	}

	return arrLen + 1;

}

///if function returns arrLen+1 then it failed for some reason
///compareFunc should check if for example p1<p2, p1 and p2 are in order of array
template<typename ArrType, typename ValType>
size_t BinarySearch(const ArrType* arr, size_t arrLen, ValType&& val, 
	bool(*compareFunc)(const std::remove_reference_t<ValType>&, const ArrType&)) {

	if (arrLen == 0) return 1;
	
	size_t start = 0; size_t end = arrLen - 1;
	while (true) {
		size_t ind = start + (end - start) / 2;
		if (compareFunc(val, arr[ind])) {
			if (ind == 0) return 0;
			else if (not compareFunc(val, arr[ind - 1])) return ind;
			else {
				end = ind - 1;
				continue;
			}
		}
		else {
			if (ind == arrLen - 1) return arrLen;
			else if (compareFunc(val, arr[ind + 1])) return ind + 1;
			else {
				start = ind + 1;
				continue;
			}
		}
	}

	return arrLen + 1;

}
