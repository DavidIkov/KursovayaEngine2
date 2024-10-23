#pragma once
#include"DLL.h"
#include"Tools/AnonDynArr.h"
#include"Tools/DynArr.h"
#include"Graphics/Primitives/VertexBuffer.h"

namespace Graphics::Abstractions {
	//can keep data in RAM if you want
	class VertexBufferClass :public Primitives::VertexBufferClass {
		AnonDynArr Buffer;
		bool IsStoringBuffer;
	public:
		typedef Primitives::VertexBufferClass::BufferDataTypeEnum BufferDataTypeEnum;
		struct LayoutDataStruct: Primitives::VertexBufferClass::LayoutDataStruct {
			bool Skip;
		};
		VertexBufferClass(bool keepData, AnonDynArr data, DynArr<LayoutDataStruct> layout);
		VertexBufferClass();
		template<typename BufferStoreType = void>
		VertexBufferClass(const VertexBufferClass& toCopy, AnonDynArr::TypeContainer<BufferStoreType>) {

		}
		VertexBufferClass(const VertexBufferClass&& toCopy);
		template<typename BufferStoreType = void>
		void operator=(const VertexBufferClass& toCopy) {

		}
		void operator=(const VertexBufferClass&& toCopy);

		

	};
}
