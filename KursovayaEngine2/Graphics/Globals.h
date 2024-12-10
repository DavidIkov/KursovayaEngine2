#pragma once
#include"DLL.h"

#if defined KE2_Debug
namespace KE2::Graphics {

	//not for user, you cant call sSomeClassID
	class _BindedInstancesClass {
		
	private:
		unsigned int FrameBufferID = 0;
	public:
		inline unsigned int gFrameBufferID() const noexcept{ return FrameBufferID; }
		inline void sFrameBufferID(unsigned int newID) noexcept { FrameBufferID = newID; }

	private:
		unsigned int RenderBufferID = 0;
	public:
		inline unsigned int gRenderBufferID() const noexcept{ return RenderBufferID; }
		inline void sRenderBufferID(unsigned int newID) noexcept { RenderBufferID = newID; }

	private:
		unsigned int ShaderProgramID = 0;
	public:
		inline unsigned int gShaderProgramID() const noexcept{ return ShaderProgramID; }
		inline void sShaderProgramID(unsigned int newID) noexcept { ShaderProgramID = newID; }

	private:
		unsigned int TextureID = 0;
	public:
		inline unsigned int gTextureID() const noexcept{ return TextureID; }
		inline void sTextureID(unsigned int newID) noexcept { TextureID = newID; }

	private:
		unsigned int VertexArrayID = 0;
	public:
		inline unsigned int gVertexArrayID() const noexcept{ return VertexArrayID; }
		inline void sVertexArrayID(unsigned int newID) noexcept { VertexArrayID = newID; }

	private:
		unsigned int VertexBufferID = 0;
	public:
		inline unsigned int gVertexBufferID() const noexcept{ return VertexBufferID; }
		inline void sVertexBufferID(unsigned int newID) noexcept { VertexBufferID = newID; }

	private:
		unsigned int IndexBufferID = 0;
	public:
		inline unsigned int gIndexBufferID() const noexcept{ return IndexBufferID; }
		inline void sIndexBufferID(unsigned int newID) noexcept { IndexBufferID = newID; }

	public:
		inline _BindedInstancesClass() noexcept {};
	};
	inline _BindedInstancesClass BindedInstances;
}
#endif
