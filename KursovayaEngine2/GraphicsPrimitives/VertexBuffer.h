#pragma once
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
public:
	VertexBuffer();
	~VertexBuffer();
	void Delete();
	unsigned int gID() const;
	void SetLayout(const std::vector<unsigned int>&& layout);
	void AddToLayout(unsigned int len);
	void SetData(const std::vector<float>& data, const VertexBufferDataUsage usage);
	void SetSubData(unsigned int offset, const std::vector<float>& data);
	void Bind();
	static void Unbind();
};