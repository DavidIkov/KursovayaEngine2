#pragma once
#include"DLL.h"
#include<vector>
#include"Tools/ClassFunctionsAccessController.h"
#include"Tools/ErrorsSystem.h"
#include"Tools/ArrayView.h"

namespace Graphics::Primitives {

	class VertexBufferClass {
	public:
		typedef unsigned int VertexBufferID_Type;
	protected:
		VertexBufferID_Type ID = 0;
		mutable bool Deleted = false;

	public:

		struct ErrorsEnumWrapperStruct :KE2::ErrorsSystemNamespace::ErrorBase {
			enum ErrorsEnum {
				AlreadyDeleted,
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
		DLLTREATMENT virtual ~VertexBufferClass();
		DLLTREATMENT void Delete();
		DLLTREATMENT VertexBufferID_Type gID();

		inline operator VertexBufferID_Type() { return ID; }

		DLLTREATMENT void SetData(const ArrayView<void>& data, const BufferReadWriteModeEnum bufferReadWriteMode);
		DLLTREATMENT void SetSubData(unsigned int offsetInBytes, const ArrayView<void>& data);

		DLLTREATMENT void CopySubData(const VertexBufferClass& srcBuffer, unsigned int srcOffsetInBytes, unsigned int dstOffsetInBytes, unsigned int amountOfBytesToCopy);

		//data should point to already allocated memory
		DLLTREATMENT void GetSubData(unsigned int offsetInBytes, unsigned int amountOfBytesToCopy, void* data);

		DLLTREATMENT void Bind();
		DLLTREATMENT static void Unbind();

#define CFAC_ClassName VertexBufferClass
		CFAC_ClassConstructor(FullAccess,
			CFAC_FuncConstr(gID)
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