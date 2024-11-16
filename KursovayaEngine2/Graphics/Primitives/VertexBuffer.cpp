#include"VertexBuffer.h"
#include"Tools/GLDebug.h"
#include"glad/glad.h"
#include"Graphics/Globals.h"

using namespace KE2;
using namespace Graphics::Primitives;
#define Assert_NotDeleted_Macro if (Deleted) ErrorsSystemNamespace::SendError<<"VertexBuffer is already deleted">>ErrorsEnumWrapperStruct(ErrorsEnum::AlreadyDeleted);
#if defined KE2_Debug
#define Assert_Binded_Macro if(BindedInstances.gVertexBufferID() != ID) ErrorsSystemNamespace::SendWarning<<"VertexBuffer is not binded">>ErrorsSystemNamespace::EndOfWarning;
#else
#define Assert_Binded_Macro
#endif

VertexBufferClass::VertexBufferClass() {
    glSC(glGenBuffers(1, &ID));
    Bind();
}
VertexBufferClass::VertexBufferClass(const VertexBufferClass&& toCopy) {
    memcpy(this, &toCopy, sizeof(VertexBufferClass));
    toCopy.Deleted = true;
}
void VertexBufferClass::operator=(const VertexBufferClass&& toCopy) {
    Delete();
    memcpy(this, &toCopy, sizeof(VertexBufferClass));
    toCopy.Deleted = true;
}
VertexBufferClass::~VertexBufferClass() {
    if (not Deleted) {
        Unbind();
        glSC(glDeleteBuffers(1, &ID));
        Deleted = true;
    }
}
void VertexBufferClass::Delete() {
    Assert_NotDeleted_Macro;
    this->~VertexBufferClass();
}
unsigned int VertexBufferClass::gID() {
    Assert_NotDeleted_Macro;
    return ID;
}

static unsigned int _GetVBUsageForGL(VertexBufferClass::BufferReadWriteModeEnum usage) {
    switch (usage) {
    case VertexBufferClass::BufferReadWriteModeEnum::None: return 0;
    case VertexBufferClass::BufferReadWriteModeEnum::StreamDraw: return GL_STREAM_DRAW;
    case VertexBufferClass::BufferReadWriteModeEnum::StreamRead: return GL_STREAM_READ;
    case VertexBufferClass::BufferReadWriteModeEnum::StreamCopy: return GL_STREAM_COPY;
    case VertexBufferClass::BufferReadWriteModeEnum::StaticDraw: return GL_STATIC_DRAW;
    case VertexBufferClass::BufferReadWriteModeEnum::StaticRead: return GL_STATIC_READ;
    case VertexBufferClass::BufferReadWriteModeEnum::StaticCopy: return GL_STATIC_COPY;
    case VertexBufferClass::BufferReadWriteModeEnum::DynamicDraw: return GL_DYNAMIC_DRAW;
    case VertexBufferClass::BufferReadWriteModeEnum::DynamicRead: return GL_DYNAMIC_READ;
    case VertexBufferClass::BufferReadWriteModeEnum::DynamicCopy: return GL_DYNAMIC_COPY;
    }
    return 0;
}
void VertexBufferClass::SetData(const ArrayView<void>& data, const BufferReadWriteModeEnum bufferReadWriteMode) {
    Assert_NotDeleted_Macro;
    Assert_Binded_Macro;

    if (bufferReadWriteMode == BufferReadWriteModeEnum::None) ErrorsSystemNamespace::SendError << "BufferReadWriteMode is none" >> ErrorsEnumWrapperStruct(ErrorsEnum::BufferReadWriteModeInNone);
	glSC(glBufferData(GL_ARRAY_BUFFER, data.gLenInBytes(), data.gDataPtr(), _GetVBUsageForGL(bufferReadWriteMode)));
}
void VertexBufferClass::SetSubData(unsigned int offsetInBytes, const ArrayView<void>& data) {
    Assert_NotDeleted_Macro;
    Assert_Binded_Macro;

	glSC(glBufferSubData(GL_ARRAY_BUFFER, offsetInBytes, data.gLenInBytes(), data.gDataPtr()));
}

void VertexBufferClass::CopySubData(const VertexBufferClass& srcBuffer, unsigned int srcOffsetInBytes, unsigned int dstOffsetInBytes, unsigned int amountOfBytesToCopy) {
    Assert_NotDeleted_Macro;
    
    glSC(glBindBuffer(GL_COPY_READ_BUFFER, srcBuffer.ID));
    glSC(glBindBuffer(GL_COPY_WRITE_BUFFER, ID));

    glSC(glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, srcOffsetInBytes, dstOffsetInBytes, amountOfBytesToCopy));

    glSC(glBindBuffer(GL_COPY_READ_BUFFER, 0));
    glSC(glBindBuffer(GL_COPY_WRITE_BUFFER, 0));
}

void VertexBufferClass::GetSubData(unsigned int offsetInBytes, unsigned int amountOfBytesToCopy, void* data) {
    Assert_NotDeleted_Macro;
    Assert_Binded_Macro;

    glSC(glGetBufferSubData(GL_ARRAY_BUFFER, offsetInBytes, amountOfBytesToCopy, data));
}

void VertexBufferClass::Bind() {
    Assert_NotDeleted_Macro;
#if defined KE2_Debug
    BindedInstances.sVertexBuffer_ID(ID);
#endif
	glSC(glBindBuffer(GL_ARRAY_BUFFER, ID));
}
void VertexBufferClass::Unbind() {
    glSC(glBindBuffer(GL_ARRAY_BUFFER, 0));
#if defined KE2_Debug
    BindedInstances.sVertexBuffer_ID(0);
#endif
}