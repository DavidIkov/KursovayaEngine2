#pragma once
#include"DLL.h"
#include<vector>

namespace GraphicsPrimitives {

	class VertexBufferClass {
		unsigned int ID = 0;
		mutable bool Deleted = false;
		unsigned short int EnabledAttributesAmount = 0;

	public:
		enum class BufferDataUsageEnum :unsigned short int {
			StreamDraw, StreamRead, StreamCopy,
			StaticDraw, StaticRead, StaticCopy,
			DynamicDraw, DynamicRead, DynamicCopy,
		};
		enum class BufferDataTypeEnum :unsigned short int {
			Byte, UnsignedByte, Float, Int, UnsignedInt
		};
	private:
		unsigned int _GetVBUsageForGL(BufferDataUsageEnum usage);
	public:
		DLLTREATMENT VertexBufferClass();
		DLLTREATMENT VertexBufferClass(const VertexBufferClass* toCopy);
		DLLTREATMENT VertexBufferClass(const VertexBufferClass&& toCopy);
		DLLTREATMENT void operator=(const VertexBufferClass&& toCopy);
		DLLTREATMENT ~VertexBufferClass();
		DLLTREATMENT void Delete();
		DLLTREATMENT unsigned int gID() const;
		DLLTREATMENT void SetLayout(BufferDataTypeEnum dataType, const std::vector<unsigned int>& layout);
		//dataLength as length*sizeof(type)
		DLLTREATMENT void SetData(unsigned int dataLength, const void* data, const BufferDataUsageEnum usage);
		//dataLength as length*sizeof(type)
		DLLTREATMENT void SetSubData(unsigned int offset, unsigned int dataLength, void* firstElement);
		DLLTREATMENT void Bind() const;
		DLLTREATMENT static void Unbind();
	};
}