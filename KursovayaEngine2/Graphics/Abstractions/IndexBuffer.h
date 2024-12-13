#pragma once
#include"DLL.h"
#include"Tools/DynArr.h"
#include"Graphics/Primitives/IndexBuffer.h"

namespace KE2::Graphics::Abstractions {
	class IndexBufferClass :protected Primitives::IndexBufferClass {
	public:

		struct ErrorsEnumWrapperStruct :KE2::ErrorsSystemNamespace::ErrorBase {
			enum ErrorsEnum {
				ProvidedTypeOfFileNotSupported,
#ifdef KE2_Debug
				TryingToUseDifferentBufferReadWriteMode,
#endif
			}; ErrorsEnum Error;
			inline ErrorsEnumWrapperStruct(ErrorsEnum error) :Error(error) {};
		}; using ErrorsEnum = ErrorsEnumWrapperStruct::ErrorsEnum; using AnyError = ErrorsEnumWrapperStruct;

		using IndexBufferID_Type = Primitives::IndexBufferClass::IndexBufferID_Type;
		using BufferReadWriteModeEnum = Primitives::IndexBufferClass::BufferReadWriteModeEnum;
	protected:
		const BufferReadWriteModeEnum BufferReadWriteMode;
		unsigned int DataSizeInBytes = 0u;
	public:

		inline Primitives::IndexBufferClass& gPrimitiveIndexBufferClass() noexcept { return *this; }
		inline const Primitives::IndexBufferClass& gPrimitiveIndexBufferClass() const noexcept { return *this; }

		DLLTREATMENT IndexBufferClass(BufferReadWriteModeEnum bufferReadWriteMode);
		DLLTREATMENT IndexBufferClass(BufferReadWriteModeEnum bufferReadWriteMode, const ArrayView<void>& data);
		//filePath to some 3d object, right now only .obj is accepted
		DLLTREATMENT IndexBufferClass(const IndexBufferClass& toCopy, bool copyBufferData = false);
		DLLTREATMENT IndexBufferClass(IndexBufferClass&& toCopy) noexcept;
		DLLTREATMENT virtual ~IndexBufferClass() noexcept(false) override = default;
		//wont copy data on GPU, only allocate a space on GPU
		DLLTREATMENT virtual IndexBufferClass& operator=(const IndexBufferClass& toCopy);
	private:
		inline virtual Primitives::IndexBufferClass& operator=(Primitives::IndexBufferClass&& toCopy) override final { return operator=(dynamic_cast<IndexBufferClass&&>(toCopy)); }
	public:
		DLLTREATMENT virtual IndexBufferClass& operator=(IndexBufferClass&& toCopy);

		using Primitives::IndexBufferClass::gID;
		using Primitives::IndexBufferClass::operator IndexBufferID_Type;

	private:
		inline virtual void SetData(const ArrayView<void>& data, const BufferReadWriteModeEnum bufferReadWriteMode) override final {
#ifdef KE2_Debug
			if (bufferReadWriteMode != BufferReadWriteMode)
				ErrorsSystemNamespace::SendError << "Trying to use different buffer read write mode" >> ErrorsEnumWrapperStruct(ErrorsEnum::TryingToUseDifferentBufferReadWriteMode);
#endif
			SetData(data);
		}
	public:
		DLLTREATMENT virtual void SetData(const ArrayView<void>& data);
		DLLTREATMENT virtual void SetSubData(unsigned int offsetInBytes, const ArrayView<void>& data) override;

		DLLTREATMENT virtual void CopyData(const IndexBufferClass& srcBuffer);
	private:
		inline virtual void CopySubData(const Primitives::IndexBufferClass& srcBuffer, unsigned int srcOffsetInBytes, unsigned int dstOffsetInBytes, unsigned int amountOfBytesToCopy) override final {
			CopySubData(dynamic_cast<const IndexBufferClass&>(srcBuffer), srcOffsetInBytes, dstOffsetInBytes, amountOfBytesToCopy);
		}
	public:
		DLLTREATMENT virtual void CopySubData(const IndexBufferClass& srcBuffer, unsigned int srcOffsetInBytes, unsigned int dstOffsetInBytes, unsigned int amountOfBytesToCopy);

		//data should point to already allocated memory
		DLLTREATMENT virtual void GetSubData(unsigned int offsetInBytes, unsigned int amountOfBytesToCopy, void* data) const override final;
		//data should point to already allocated memory
		DLLTREATMENT void GetData(void* data) const;

		inline BufferReadWriteModeEnum gBufferReadWriteModeEnum() const noexcept { return BufferReadWriteMode; }
		inline unsigned int gDataSizeInBytes() const noexcept { return DataSizeInBytes; }

		using Primitives::IndexBufferClass::Bind;
		using Primitives::IndexBufferClass::Unbind;

	};
}