#pragma once
#include"DLL.h"
#include"Tools/ErrorsSystem.h"
#include"Tools/ArrayView.h"

namespace KE2::Graphics::Primitives {

	class VertexBufferClass {
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
		
		DLLTREATMENT VertexBufferClass();
		DLLTREATMENT VertexBufferClass(VertexBufferClass&& toCopy) noexcept;
		DLLTREATMENT virtual VertexBufferClass& operator=(VertexBufferClass&& toCopy);
		DLLTREATMENT virtual ~VertexBufferClass() noexcept(false);

		typedef unsigned int VertexBufferID_Type;
		inline VertexBufferID_Type gID() const noexcept { return ID; }
		inline operator VertexBufferID_Type() const noexcept { return ID; }

		DLLTREATMENT virtual void SetData(const ArrayView<void>& data, const BufferReadWriteModeEnum bufferReadWriteMode);

		DLLTREATMENT virtual void SetSubData(size_t offsetInBytes, const ArrayView<void>& data);

		DLLTREATMENT virtual void CopySubData(const VertexBufferClass& srcBuffer, size_t srcOffsetInBytes, size_t dstOffsetInBytes, size_t amountOfBytesToCopy);

		//data should point to already allocated memory
		DLLTREATMENT virtual void GetSubData(size_t offsetInBytes, size_t amountOfBytesToCopy, void* data) const;

		DLLTREATMENT void Bind() const;
		DLLTREATMENT static void Unbind();

	};
}