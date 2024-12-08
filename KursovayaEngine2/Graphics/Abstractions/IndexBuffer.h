#pragma once
#include"DLL.h"
#include"Tools/DynArr.h"
#include"Graphics/Primitives/IndexBuffer.h"

namespace Graphics::Abstractions {
	class IndexBufferClass :protected Primitives::IndexBufferClass {
	public:

		struct ErrorsEnumWrapperStruct :KE2::ErrorsSystemNamespace::ErrorBase {
			enum ErrorsEnum {
				ProvidedTypeOfFileNotSupported,
			}; ErrorsEnum Error;
			inline ErrorsEnumWrapperStruct(ErrorsEnum error) :Error(error) {};
		}; using ErrorsEnum = ErrorsEnumWrapperStruct::ErrorsEnum; using AnyError = ErrorsEnumWrapperStruct;

		using IndexBufferID_Type = Primitives::IndexBufferClass::IndexBufferID_Type;
		using BufferReadWriteModeEnum = Primitives::IndexBufferClass::BufferReadWriteModeEnum;
	protected:
		const BufferReadWriteModeEnum BufferReadWriteMode;
		unsigned int DataSizeInBytes = 0u;
	public:
		DLLTREATMENT IndexBufferClass(BufferReadWriteModeEnum bufferReadWriteMode);
		DLLTREATMENT IndexBufferClass(BufferReadWriteModeEnum bufferReadWriteMode, const ArrayView<void>& data);
		//filePath to some 3d object, right now only .obj is accepted
		DLLTREATMENT IndexBufferClass(const IndexBufferClass& toCopy, bool copyBufferData = false);
		DLLTREATMENT IndexBufferClass(IndexBufferClass&& toCopy) noexcept;
		DLLTREATMENT virtual ~IndexBufferClass() noexcept(false) override = default;
		//wont copy data on GPU, only allocate a space on GPU
		DLLTREATMENT IndexBufferClass& operator=(const IndexBufferClass& toCopy);
		DLLTREATMENT IndexBufferClass& operator=(IndexBufferClass&& toCopy);

		using Primitives::IndexBufferClass::gID;
		using Primitives::IndexBufferClass::operator IndexBufferID_Type;

		DLLTREATMENT void SetData(const ArrayView<void>& data);
		using Primitives::IndexBufferClass::SetSubData;

		DLLTREATMENT void CopyData(const IndexBufferClass& srcBuffer);
		DLLTREATMENT void CopySubData(const IndexBufferClass& srcBuffer, unsigned int srcOffsetInBytes, unsigned int dstOffsetInBytes, unsigned int amountOfBytesToCopy);

		//data should point to already allocated memory
		DLLTREATMENT void GetSubData(unsigned int offsetInBytes, unsigned int amountOfBytesToCopy, void* data) const;
		//data should point to already allocated memory
		DLLTREATMENT void GetData(void* data) const;

		inline BufferReadWriteModeEnum gBufferReadWriteModeEnum() const noexcept { return BufferReadWriteMode; }
		inline unsigned int gDataSizeInBytes() const noexcept { return DataSizeInBytes; }

		using Primitives::IndexBufferClass::Bind;
		using Primitives::IndexBufferClass::Unbind;

	};
}