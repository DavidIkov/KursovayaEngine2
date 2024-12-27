#pragma once
#include"Graphics/Primitives/RenderBuffer.h"
#include"Tools/ConnectionSlot.h"
#include"Tools/DynArr.h"

namespace KE2::Graphics::Abstractions {
	class RenderBufferClass :protected Primitives::RenderBufferClass {
	public:
		using BufferDataFormatEnum = Primitives::RenderBufferClass::BufferDataFormatEnum;
	protected:
		friend class FrameBufferClass;

		const BufferDataFormatEnum BufferDataFormat;
		const Vector2U Size;

		struct BindingS:public ConnectionSlotC{
			RenderBufferClass* ThisRB;
			BindingS(RenderBufferClass* thisRB) noexcept:ThisRB(thisRB) {}
			BindingS(BindingS&& toMove) noexcept:ThisRB(toMove.ThisRB) { toMove.ThisRB = nullptr; }
			~BindingS() override {
				if (ThisRB != nullptr) {
					ThisRB->Bindings.RemoveAtIndex_WithoutDestructor(this - *ThisRB->Bindings);
					ThisRB = nullptr;
				}
			}
		};
		DynArr<BindingS> Bindings;
	public:


		inline Primitives::RenderBufferClass& gPrimitiveRenderBufferClass() noexcept { return *this; }
		inline const Primitives::RenderBufferClass& gPrimitiveRenderBufferClass() const noexcept { return *this; }

		DLLTREATMENT RenderBufferClass(Vector2U size, BufferDataFormatEnum bufferDataFormat);
		DLLTREATMENT RenderBufferClass(RenderBufferClass&& toCopy) noexcept;
		DLLTREATMENT RenderBufferClass(const RenderBufferClass& toCopy) noexcept;
	private:
		inline virtual Primitives::RenderBufferClass& operator=(Primitives::RenderBufferClass&& toCopy) override final { return operator=(dynamic_cast<RenderBufferClass&&>(toCopy)); }
	public:
		DLLTREATMENT virtual RenderBufferClass& operator=(RenderBufferClass&& toCopy);
		DLLTREATMENT virtual RenderBufferClass& operator=(const RenderBufferClass& toCopy);
		DLLTREATMENT virtual ~RenderBufferClass() noexcept(false) override = default;

		inline BufferDataFormatEnum gBufferDataFormat() const noexcept { return BufferDataFormat; }
		inline Vector2U gSize() const noexcept { return Size; }

		using Primitives::RenderBufferClass::Bind;
		using Primitives::RenderBufferClass::Unbind;

		using RenderBufferID_Type = Primitives::RenderBufferClass::RenderBufferID_Type;
		using Primitives::RenderBufferClass::gID;
		using Primitives::RenderBufferClass::operator RenderBufferID_Type;

	};
}
