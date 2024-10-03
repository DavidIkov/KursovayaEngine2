#pragma once
#include"DLL.h"

#if defined KE2_Debug
namespace Graphics {

	//not for user, you cant call sSomeClassID
	class _BindedInstancesClass {
		
	private:
		unsigned int FrameBufferID = 0;
	public:
		unsigned int gFrameBufferID();
		void sFrameBuffer_ID(unsigned int newID);

	private:
		unsigned int RenderBufferID = 0;
	public:
		unsigned int gRenderBufferID();
		void sRenderBuffer_ID(unsigned int newID);

	private:
		unsigned int ShaderProgramID = 0;
	public:
		unsigned int gShaderProgramID();
		void sShaderProgram_ID(unsigned int newID);

	private:
		unsigned int TextureID = 0;
	public:
		unsigned int gTextureID();
		void sTexture_ID(unsigned int newID);

	private:
		unsigned int VertexArrayID = 0;
		void* VertexArrayPtr=nullptr;
	public:
		unsigned int gVertexArrayID();
		void* gVertexArrayPtr();
		void sVertexArray_ID_And_Ptr(unsigned int newID, void* newPtr);

	private:
		unsigned int VertexBufferID = 0;
		void* VertexBufferPtr=nullptr;
	public:
		unsigned int gVertexBufferID();
		void sVertexBuffer_ID(unsigned int newID);

	public:
		_BindedInstancesClass();
	};
	inline _BindedInstancesClass BindedInstances;
}
#endif
