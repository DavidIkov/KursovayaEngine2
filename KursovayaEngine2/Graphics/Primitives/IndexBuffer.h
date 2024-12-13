#pragma once
#include"DLL.h"
#include"Tools/ErrorsSystem.h"
#include"Tools/ArrayView.h"

namespace KE2::Graphics::Primitives {

	class IndexBufferClass {
	protected:
		unsigned int ID = 0u;

	public:

		struct ErrorsEnumWrapperStruct :KE2::ErrorsSystemNamespace::ErrorBase {
			enum ErrorsEnum {
				BufferReadWriteModeInNone,
			};
			ErrorsEnum Error;
			inline ErrorsEnumWrapperStruct(ErrorsEnum error) :Error(error) {};
		};  using ErrorsEnum = ErrorsEnumWrapperStruct; using AnyError = ErrorsEnumWrapperStruct;

		enum class BufferReadWriteModeEnum :unsigned char {
			StreamDraw, StreamRead, StreamCopy,
			StaticDraw, StaticRead, StaticCopy,
			DynamicDraw, DynamicRead, DynamicCopy,
		};
		
		DLLTREATMENT IndexBufferClass();
		DLLTREATMENT IndexBufferClass(IndexBufferClass&& toCopy) noexcept;
		DLLTREATMENT virtual IndexBufferClass& operator=(IndexBufferClass&& toCopy);
		DLLTREATMENT virtual ~IndexBufferClass() noexcept(false);

		typedef unsigned int IndexBufferID_Type;
		inline IndexBufferID_Type gID() const noexcept { return ID; }
		inline operator IndexBufferID_Type() const noexcept { return ID; }

		DLLTREATMENT virtual void SetData(const ArrayView<void>& data, const BufferReadWriteModeEnum bufferReadWriteMode);
		DLLTREATMENT virtual void SetSubData(unsigned int offsetInBytes, const ArrayView<void>& data);

		DLLTREATMENT virtual void CopySubData(const IndexBufferClass& srcBuffer, unsigned int srcOffsetInBytes, unsigned int dstOffsetInBytes, unsigned int amountOfBytesToCopy);

		//data should point to already allocated memory
		DLLTREATMENT virtual void GetSubData(unsigned int offsetInBytes, unsigned int amountOfBytesToCopy, void* data) const;

		DLLTREATMENT void Bind() const;
		DLLTREATMENT static void Unbind();

	};
}
