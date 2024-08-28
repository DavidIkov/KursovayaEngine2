#pragma once
#include"DLL.h"
#include<vector>

//for documentation visit https://docs.gl/gl4/glBufferData
class VertexBuffer {
	unsigned int ID = 0;
	mutable bool Deleted = false;
	unsigned short int EnabledAttributesAmount = 0;

public:
	enum class BufferDataUsage :unsigned short int {
		StreamDraw, StreamRead, StreamCopy,
		StaticDraw, StaticRead, StaticCopy,
		DynamicDraw, DynamicRead, DynamicCopy,
	};
	enum class BufferDataType :unsigned short int {
		Byte, UnsignedByte, Float, Int, UnsignedInt
	};
private:
	unsigned int _GetVBUsageForGL(BufferDataUsage usage);
public:
	DLLTREATMENT VertexBuffer();
	DLLTREATMENT VertexBuffer(const VertexBuffer* toCopy);
	DLLTREATMENT VertexBuffer(const VertexBuffer&& toCopy);
	DLLTREATMENT void operator=(const VertexBuffer&& toCopy);
	DLLTREATMENT ~VertexBuffer();
	DLLTREATMENT void Delete();
	DLLTREATMENT unsigned int gID() const;
	DLLTREATMENT void SetLayout(BufferDataType dataType, const std::vector<unsigned int>& layout);
	//dataLength as length*sizeof(type)
	DLLTREATMENT void ReserveData(unsigned int dataLength, BufferDataUsage usage);
	//dataLength as length*sizeof(type)
	DLLTREATMENT void SetData(const void* data, unsigned int dataLength, const BufferDataUsage usage);
	//dataLength as length*sizeof(type)
	DLLTREATMENT void SetSubData(unsigned int offset, unsigned int dataLength, void* firstElement);
	DLLTREATMENT void Bind() const;
	DLLTREATMENT static void Unbind();
};