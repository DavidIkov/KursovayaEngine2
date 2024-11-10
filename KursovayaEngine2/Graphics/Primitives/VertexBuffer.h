#pragma once
#include"DLL.h"
#include<vector>
#include"Tools/ClassFunctionsAccessController.h"
#include"Tools/DynArr.h"
#include"Tools/AnonDynArr.h"
#include"Tools/ErrorsSystem.h"

namespace Graphics::Primitives {

	class VertexBufferClass {
	protected:
		unsigned int ID = 0;
		mutable bool Deleted = false;
		unsigned short int EnabledAttributesAmount = 0;

	public:

		struct ErrorsEnumWrapperStruct :KE2::ErrorsSystemNamespace::ErrorBase {
			enum ErrorsEnum {
				AlreadyDeleted,
				ChangingLayoutOfVertexBufferWithoutAnyVertexArrayBinded,
				BufferReadWriteModeInNone,
			};
			ErrorsEnum Error;
			inline ErrorsEnumWrapperStruct(ErrorsEnum error) :Error(error) {};
		};  using ErrorsEnum = ErrorsEnumWrapperStruct; using AnyError = ErrorsEnumWrapperStruct;

		enum class BufferReadWriteModeEnum :unsigned char {
			None,
			StreamDraw, StreamRead, StreamCopy,
			StaticDraw, StaticRead, StaticCopy,
			DynamicDraw, DynamicRead, DynamicCopy,
		};
		
		DLLTREATMENT VertexBufferClass();
		DLLTREATMENT VertexBufferClass(const VertexBufferClass&& toCopy);
		DLLTREATMENT void operator=(const VertexBufferClass&& toCopy);
		DLLTREATMENT ~VertexBufferClass();
		DLLTREATMENT void Delete();
		DLLTREATMENT unsigned int gID();

		struct LayoutDataStruct {
			unsigned int ComponentsAmount;
			enum class DataTypeEnum :unsigned char {
				Byte, UnsignedByte, Float, Int, UnsignedInt
			};
			DataTypeEnum DataType;
		};
		DLLTREATMENT void SetLayout(const DynArr<LayoutDataStruct>& layout);

		DLLTREATMENT void SetData(const void* data, unsigned int dataSizeInBytes, const BufferReadWriteModeEnum bufferReadWriteMode);
		DLLTREATMENT void SetSubData(unsigned int offsetInBytes, const void* data, unsigned int dataSizeInBytes);

		DLLTREATMENT void CopySubData(const VertexBufferClass& srcBuffer, unsigned int srcOffsetInBytes, unsigned int dstOffsetInBytes, unsigned int amountOfBytesToCopy);

		//data should point to already allocated memory
		DLLTREATMENT void GetSubData(unsigned int offsetInBytes, unsigned int amountOfBytesToCopy, void* data);

		DLLTREATMENT void Bind();
		DLLTREATMENT static void Unbind();

#define CFAC_ClassName VertexBufferClass
		CFAC_ClassConstructor(FullAccess,
			CFAC_FuncConstr(gID)
			CFAC_FuncConstr(SetLayout)
			CFAC_FuncConstr(SetData)
			CFAC_FuncConstr(SetSubData)
			CFAC_FuncConstr(CopySubData)
			CFAC_FuncConstr(GetSubData)
			CFAC_FuncConstr(Bind)
			CFAC_FuncConstr(Unbind)
		);
#undef CFAC_ClassName
	};
}