#pragma once
template<typename PT, typename CT>
size_t PtrToMember_ToOff(PT const CT::* const ptr) {
	//alternative __builtin_offsetof(class,member)
	return (size_t) & (((CT*)0)->*ptr);
}
