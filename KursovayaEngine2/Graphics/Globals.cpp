#include"Globals.h"

#if defined KE2_Debug

using namespace Graphics;

_BindedInstancesClass::_BindedInstancesClass() {}

unsigned int _BindedInstancesClass::gFrameBufferID() { return FrameBufferID; }
unsigned int _BindedInstancesClass::gRenderBufferID() { return RenderBufferID; }
unsigned int _BindedInstancesClass::gShaderProgramID() { return ShaderProgramID; }
unsigned int _BindedInstancesClass::gTextureID() { return TextureID; }
unsigned int _BindedInstancesClass::gVertexArrayID() { return VertexArrayID; }
unsigned int _BindedInstancesClass::gVertexBufferID() { return VertexBufferID; }

void _BindedInstancesClass::sFrameBuffer_ID(unsigned int newID) { FrameBufferID = newID; }
void _BindedInstancesClass::sRenderBuffer_ID(unsigned int newID) { RenderBufferID = newID; }
void _BindedInstancesClass::sShaderProgram_ID(unsigned int newID) { ShaderProgramID = newID; }
void _BindedInstancesClass::sTexture_ID(unsigned int newID) { TextureID=newID; }
void _BindedInstancesClass::sVertexArray_ID(unsigned int newID) { VertexArrayID=newID; }
void _BindedInstancesClass::sVertexBuffer_ID(unsigned int newID) { VertexBufferID=newID; }

#endif

