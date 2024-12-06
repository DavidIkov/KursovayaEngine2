#pragma once
#include"DLL.h"
#include<vector>
#include"Tools/ErrorsSystem.h"
#include"Tools/ArrayView.h"

namespace Graphics::Primitives {

	class VertexBufferClass {
	public:
		typedef unsigned int VertexBufferID_Type;
	protected:
		VertexBufferID_Type ID = 0u;

	public:

		struct ErrorsEnumWrapperStruct :KE2::ErrorsSystemNamespace::ErrorBase {
			enum ErrorsEnum {
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
		DLLTREATMENT VertexBufferClass(VertexBufferClass&& toCopy) noexcept;
		DLLTREATMENT VertexBufferClass& operator=(VertexBufferClass&& toCopy);
		DLLTREATMENT virtual ~VertexBufferClass() noexcept(false);

		inline VertexBufferID_Type gID() const noexcept { return ID; }
		inline operator VertexBufferID_Type() const noexcept { return ID; }

		DLLTREATMENT void SetData(const ArrayView<void>& data, const BufferReadWriteModeEnum bufferReadWriteMode) const;
		DLLTREATMENT void SetSubData(unsigned int offsetInBytes, const ArrayView<void>& data) const;

		DLLTREATMENT void CopySubData(const VertexBufferClass& srcBuffer, unsigned int srcOffsetInBytes, unsigned int dstOffsetInBytes, unsigned int amountOfBytesToCopy) const;

		//data should point to already allocated memory
		DLLTREATMENT void GetSubData(unsigned int offsetInBytes, unsigned int amountOfBytesToCopy, void* data) const;

		DLLTREATMENT void Bind() const;
		DLLTREATMENT static void Unbind();

	};
}