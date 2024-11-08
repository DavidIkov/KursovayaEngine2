#pragma once
#include"DLL.h"
#include"Tools/AnonDynArr.h"
#include"Tools/DynArr.h"
#include"Graphics/Primitives/VertexBuffer.h"
#include"Graphics/Primitives/VertexArray.h"

namespace Graphics::Abstractions {
	class VertexBufferClass :protected Primitives::VertexBufferClass {
	public:
		using Primitives::VertexBufferClass::BufferReadWriteModeEnum;
		using Primitives::VertexBufferClass::LayoutDataStruct;
	protected:
		BufferReadWriteModeEnum BufferReadWriteMode = BufferReadWriteModeEnum::None;
		Primitives::VertexArrayClass VA;
		unsigned int DataSizeInBytes = 0;
		DynArr<LayoutDataStruct> Layout;
	public:
		DLLTREATMENT VertexBufferClass();
		DLLTREATMENT VertexBufferClass(BufferReadWriteModeEnum bufferReadWriteMode, const void* data, unsigned int dataSizeInBytes, const DynArr<LayoutDataStruct>& layout);
		//filePath to some 3d object, right now only .obj is accepted
		DLLTREATMENT VertexBufferClass(const wchar_t* filePath);
		DLLTREATMENT VertexBufferClass(const VertexBufferClass&& toCopy);
		DLLTREATMENT void operator=(const VertexBufferClass&& toCopy);

		DLLTREATMENT void SetLayout(DynArr<LayoutDataStruct> layout);

		DLLTREATMENT void SetBufferReadWriteModeEnum(BufferReadWriteModeEnum newBufferUsage);

		DLLTREATMENT void SetData(const void* data, unsigned int dataSizeInBytes);
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
		
		DLLTREATMENT void Bind();
		DLLTREATMENT void Unbind();
		DLLTREATMENT void BindForRender();
		DLLTREATMENT void UnbindFromRender();

	};
}
