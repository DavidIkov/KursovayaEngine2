#pragma once

//faster that any implementation, even std::function
template<unsigned int StorageSize, typename RetType, typename...ParamsType>
class StaticLambda_Heap {
	
public:

	class CaptureListExplorer {

		//firstly all offsets are stored and then actual data is stored
		unsigned char Storage[StorageSize];

	public:

		void* operator[](unsigned int ind) {
			return (unsigned char*)((unsigned int*)Storage + ind) + ((unsigned int*)Storage)[ind];
		}
	};

private:

	CaptureListExplorer StorageExplorer;
	RetType(*Func)(CaptureListExplorer&, ParamsType&&...);

	template<typename Type, typename...OtherTypesT>
	constexpr void FillStorage(unsigned int counter, unsigned char* offset, Type&& val, OtherTypesT&&...other) {
		new(offset) (std::remove_reference_t<Type>)(std::forward<Type>(val));
		unsigned int* intOff = (unsigned int*)StorageExplorer.Storage + counter;
		new(intOff) unsigned int((unsigned int)(offset - (unsigned char*)intOff));
		if constexpr (sizeof...(OtherTypesT) != 0) FillStorage(counter + 1, offset + sizeof(Type), ((OtherTypesT&&)other)...);
	}

public:

	template<typename...CaptureListT>
	StaticLambda_Heap(RetType(*func)(CaptureListExplorer&, ParamsType&&...), CaptureListT&&...CaptureList) :Func(func) {
		FillStorage(0, (unsigned char*)StorageExplorer.Storage + sizeof(unsigned int) * sizeof...(CaptureListT), ((CaptureListT&&)CaptureList)...);
	}

	RetType operator()(ParamsType&&...params) {
		return Func(StorageExplorer, std::forward<ParamsType>(params)...);
	}

};






//slower than _Heap variant and slower constructor/destructor that std::function but is memory efficient and have same time of call as std::function
template<typename RetType, typename...ParamsType>
class StaticLambda_Dynamic {

public:

	class CaptureListExplorer {

		//firstly all offsets are stored and then actual data is stored
		void* Storage = nullptr;

	public:

		void* operator[](unsigned int ind) {
			return (unsigned char*)((unsigned int*)Storage + ind) + ((unsigned int*)Storage)[ind];
		}

		~CaptureListExplorer() {
			delete[](unsigned char*)Storage;
		}
	};

private:

	CaptureListExplorer StorageExplorer;
	RetType(*Func)(CaptureListExplorer&, ParamsType&&...);

	template<typename Type, typename...OtherTypesT>
	constexpr unsigned int GetCaptureListSize() {
		if constexpr (sizeof...(OtherTypesT) == 0) return sizeof(Type);
		else return sizeof(Type) + GetCaptureListSize<OtherTypesT...>();
	}

	template<typename Type, typename...OtherTypesT>
	constexpr void FillStorage(unsigned int counter, unsigned char* offset, Type&& val, OtherTypesT&&...other) {
		new(offset) (std::remove_reference_t<Type>)(std::forward<Type>(val));
		unsigned int* intOff = (unsigned int*)StorageExplorer.Storage + counter;
		new(intOff) unsigned int((unsigned int)(offset - (unsigned char*)intOff));
		if constexpr (sizeof...(OtherTypesT) != 0) FillStorage(counter + 1, offset + sizeof(Type), ((OtherTypesT&&)other)...);
	}

public:

	template<typename...CaptureListT>
	StaticLambda_Dynamic(RetType(*func)(CaptureListExplorer&, ParamsType&&...), CaptureListT&&...CaptureList) :Func(func) {
		StorageExplorer.Storage = new unsigned char[sizeof(unsigned int) * sizeof...(CaptureListT) + GetCaptureListSize<CaptureListT...>()];
		FillStorage(0, (unsigned char*)StorageExplorer.Storage + sizeof(unsigned int) * sizeof...(CaptureListT), ((CaptureListT&&)CaptureList)...);
	}

	RetType operator()(ParamsType&&...params) {
		return Func(StorageExplorer, std::forward<ParamsType>(params)...);
	}

};