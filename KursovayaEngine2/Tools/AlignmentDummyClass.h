#pragma once
template<typename Type>
class alignas(Type) AlignmentDummyClass { unsigned char _[sizeof(Type)]; };
