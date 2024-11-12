#pragma once
#include"DLL.h"
#include"Tools/AnonDynArr.h"
#include"Tools/DynArr.h"
#include"Graphics/Primitives/VertexBuffer.h"
#include"Graphics/Primitives/VertexArray.h"

namespace Graphics::Abstractions {
	class VertexBufferClass :protected Primitives::VertexBufferClass {
	public:

		struct ErrorsEnumWrapperStruct :KE2::ErrorsSystemNamespace::ErrorBase {
			enum ErrorsEnum {
				ProvidedTypeOfFileNotSupported,
			}; ErrorsEnum Error;
			inline ErrorsEnumWrapperStruct(ErrorsEnum error) :Error(error) {};
		}; using ErrorsEnum = ErrorsEnumWrapperStruct::ErrorsEnum; using AnyError = ErrorsEnumWrapperStruct;

		using BufferReadWriteModeEnum = Primitives::VertexBufferClass::BufferReadWriteModeEnum;
		using LayoutDataStruct = Primitives::VertexBufferClass::LayoutDataStruct;
	protected:
		BufferReadWriteModeEnum BufferReadWriteMode = BufferReadWriteModeEnum::None;
		Primitives::VertexArrayClass VA;
		unsigned int DataSizeInBytes = 0;
		DynArr<LayoutDataStruct> Layout;
	public:
		DLLTREATMENT VertexBufferClass(BufferReadWriteModeEnum bufferReadWriteMode);
		DLLTREATMENT VertexBufferClass(BufferReadWriteModeEnum bufferReadWriteMode, const ArrayView<void>& data, const DynArr<LayoutDataStruct>& layout);
		//filePath to some 3d object, right now only .obj is accepted
		DLLTREATMENT VertexBufferClass(BufferReadWriteModeEnum bufferReadWriteMode, const wchar_t* filePath);
		DLLTREATMENT VertexBufferClass(BufferReadWriteModeEnum bufferReadWriteMode, const wchar_t* filePath, const DynArr<LayoutDataStruct>& layout);
		DLLTREATMENT VertexBufferClass(const VertexBufferClass& toCopy, bool copyBufferData = false);
		DLLTREATMENT VertexBufferClass(const VertexBufferClass&& toCopy);
		DLLTREATMENT ~VertexBufferClass() = default;
		//wont copy data on GPU, only allocate a space on GPU
		DLLTREATMENT void operator=(const VertexBufferClass& toCopy);
		DLLTREATMENT void operator=(const VertexBufferClass&& toCopy);

		DLLTREATMENT void SetLayout(DynArr<LayoutDataStruct> layout);

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
		DLLTREATMENT const DynArr<LayoutDataStruct>& gLayout() const;

		DLLTREATMENT void Delete();
		
		DLLTREATMENT void Bind();
		DLLTREATMENT static void Unbind();

		//use this function only if you using abstractions with primitives or want to work with vertex array manually
		DLLTREATMENT void BindForRender();
		DLLTREATMENT static void UnbindFromRender();


	};
}
