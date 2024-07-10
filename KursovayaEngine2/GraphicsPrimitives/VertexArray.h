#pragma once
class VertexArray {
	unsigned int ID = 0;
	mutable bool Deleted = false;
public:
	VertexArray();
	~VertexArray();
	unsigned int gID() const;
	void Delete();
	void Bind();
	static void Unbind();
};