#pragma once
#include"DLL.h"
#include"Tools/DynArr.h"
#include"Graphics/Primitives/VertexBuffer.h"

namespace Graphics::Abstractions {
	class VertexBufferClass :protected Primitives::VertexBufferClass {
	public:

		struct ErrorsEnumWrapperStruct :KE2::ErrorsSystemNamespace::ErrorBase {
			enum ErrorsEnum {
				ProvidedTypeOfFileNotSupported,
			}; ErrorsEnum Error;
			inline ErrorsEnumWrapperStruct(ErrorsEnum error) :Error(error) {};
		}; using ErrorsEnum = ErrorsEnumWrapperStruct::ErrorsEnum; using AnyError = ErrorsEnumWrapperStruct;

		using VertexBufferID_Type = Primitives::VertexBufferClass::VertexBufferID_Type;
		using BufferReadWriteModeEnum = Primitives::VertexBufferClass::BufferReadWriteModeEnum;
	protected:
		BufferReadWriteModeEnum BufferReadWriteMode = BufferReadWriteModeEnum::None;
		unsigned int DataSizeInBytes = 0;
	public:
		DLLTREATMENT VertexBufferClass(BufferReadWriteModeEnum bufferReadWriteMode);
		DLLTREATMENT VertexBufferClass(BufferReadWriteModeEnum bufferReadWriteMode, const ArrayView<void>& data);
		//filePath to some 3d object, right now only .obj is accepted
		DLLTREATMENT VertexBufferClass(BufferReadWriteModeEnum bufferReadWriteMode, const wchar_t* filePath);
		DLLTREATMENT VertexBufferClass(const VertexBufferClass& toCopy, bool copyBufferData = false);
		DLLTREATMENT VertexBufferClass(const VertexBufferClass&& toCopy);
		DLLTREATMENT ~VertexBufferClass() = default;
		//wont copy data on GPU, only allocate a space on GPU
		DLLTREATMENT void operator=(const VertexBufferClass& toCopy);
		DLLTREATMENT void operator=(const VertexBufferClass&& toCopy);

		inline operator VertexBufferID_Type() const { return ID; }
		inline VertexBufferID_Type gID() const { return ID; }

		DLLTREATMENT void SetData(const ArrayView<void>& data);
		using Primitives::VertexBufferClass::SetSubData;

		DLLTREATMENT void CopyData(const VertexBufferClass& srcBuffer);
		DLLTREATMENT void CopySubData(const VertexBufferClass& srcBuffer, unsigned int srcOffsetInBytes, unsigned int dstOffsetInBytes, unsigned int amountOfBytesToCopy);

		//data should point to already allocated memory
		DLLTREATMENT void GetSubData(unsigned int offsetInBytes, unsigned int amountOfBytesToCopy, void* data);
		//data should point to already allocated memory
		DLLTREATMENT void GetData(void* data);

		DLLTREATMENT BufferReadWriteModeEnum gBufferReadWriteModeEnum() const;
		DLLTREATMENT unsigned int gDataSizeInBytes() const;

		DLLTREATMENT void Delete();
		
		DLLTREATMENT void Bind();
		DLLTREATMENT static void Unbind();


	};
}
