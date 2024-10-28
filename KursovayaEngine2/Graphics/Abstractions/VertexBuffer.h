#pragma once
#include"DLL.h"
#include"Tools/AnonDynArr.h"
#include"Tools/DynArr.h"
#include"Graphics/Primitives/VertexBuffer.h"
#include"Graphics/Primitives/VertexArray.h"

namespace Graphics::Abstractions {
	class VertexBufferClass :protected Primitives::VertexBufferClass {
	protected:
		Primitives::VertexArrayClass VA;
	public:
		typedef Primitives::VertexBufferClass::BufferDataTypeEnum BufferDataTypeEnum;
		typedef Primitives::VertexBufferClass::LayoutDataStruct LayoutDataStruct ;
		DLLTREATMENT VertexBufferClass(void* data, DynArr<LayoutDataStruct> layout);
		//filePath to some 3d object, right now only .obj is accepted
		DLLTREATMENT VertexBufferClass(const wchar_t* filePath);
		DLLTREATMENT VertexBufferClass(const VertexBufferClass&& toCopy);
		DLLTREATMENT void operator=(const VertexBufferClass&& toCopy);

		using Primitives::VertexBufferClass::SetData;
		using Primitives::VertexBufferClass::SetSubData;



	}
}
