#pragma once
#include"DLL.h"
#include<vector>

//for documentation visit https://docs.gl/gl4/glBufferData
enum class VertexBufferDataUsage {
	StreamDraw,StreamRead,StreamCopy,
	StaticDraw,StaticRead,StaticCopy,
	DynamicDraw,DynamicRead,DynamicCopy,
};
class VertexBuffer {
	unsigned int ID = 0;
	mutable bool Deleted = false;
	std::vector<unsigned int> Layout;
	unsigned int SumOfLayout = 0;
	void UnbindLayout();
	void UpdateLayout();

	unsigned int GetVBUsageForGL(VertexBufferDataUsage usage);
public:
	DLLTREATMENT VertexBuffer();
	DLLTREATMENT VertexBuffer(VertexBuffer&& tempVB);
	DLLTREATMENT ~VertexBuffer();
	DLLTREATMENT void Delete();
	DLLTREATMENT unsigned int gID() const;
	DLLTREATMENT void SetLayout(const std::vector<unsigned int>&& layout);
	DLLTREATMENT void AddToLayout(unsigned int len);
	DLLTREATMENT void ReserveData(unsigned int amountOfElements, VertexBufferDataUsage usage);
	DLLTREATMENT void SetData(const std::vector<float>& data, const VertexBufferDataUsage usage);
	//dataLength as length*sizeof(float)
	DLLTREATMENT void SetData(const float* data, unsigned int dataLength, const VertexBufferDataUsage usage);
	DLLTREATMENT void SetSubData(unsigned int offset, const std::vector<float>& data);
	DLLTREATMENT void SetSubData(unsigned int offset, unsigned int elementsAmount, float* firstElement);
	DLLTREATMENT void Bind();
	DLLTREATMENT static void Unbind();
};