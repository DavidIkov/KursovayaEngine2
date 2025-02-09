#include"VertexBuffer.h"
#include"Tools/GLDebug.h"
#include"glad/glad.h"
#include"Graphics/Globals.h"

using namespace KE2;
using namespace Graphics::Primitives;
#if defined KE2_Debug
#define Assert_Binded_Macro if(BindedInstances.gVertexBufferID() != ID) ErrorsSystemNamespace::SendWarning<<"VertexBuffer is not binded">>ErrorsSystemNamespace::EndOfWarning;
#else
#define Assert_Binded_Macro
#endif

VertexBufferClass::VertexBufferClass() {
    glSC(glGenBuffers(1, &ID));
    Bind();
}
VertexBufferClass::VertexBufferClass(VertexBufferClass&& toCopy) noexcept :
    ID(toCopy.ID) {
    toCopy.ID = 0u;
}
VertexBufferClass& VertexBufferClass::operator=(VertexBufferClass&& toCopy) {
    this->~VertexBufferClass();
    new(this) VertexBufferClass(std::move(toCopy));
    return *this;
}
VertexBufferClass::~VertexBufferClass() noexcept(false) {
    if (ID != 0u) {
        Unbind();
        glSC(glDeleteBuffers(1, &ID));
        ID = 0u;
    }
};

static unsigned int _GetVBUsageForGL(VertexBufferClass::BufferReadWriteModeEnum usage) {
    switch (usage) {
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
    Assert_Binded_Macro;

	glSC(glBufferData(GL_ARRAY_BUFFER, data.gLenInBytes(), data.gDataPtr(), _GetVBUsageForGL(bufferReadWriteMode)));
}
void VertexBufferClass::SetSubData(size_t offsetInBytes, const ArrayView<void>& data) {
    Assert_Binded_Macro;

	glSC(glBufferSubData(GL_ARRAY_BUFFER, offsetInBytes, data.gLenInBytes(), data.gDataPtr()));
}

void VertexBufferClass::CopySubData(const VertexBufferClass& srcBuffer, size_t srcOffsetInBytes, size_t dstOffsetInBytes, size_t amountOfBytesToCopy) {
    glSC(glBindBuffer(GL_COPY_READ_BUFFER, srcBuffer.ID));
    glSC(glBindBuffer(GL_COPY_WRITE_BUFFER, ID));

    glSC(glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, srcOffsetInBytes, dstOffsetInBytes, amountOfBytesToCopy));

    glSC(glBindBuffer(GL_COPY_READ_BUFFER, 0));
    glSC(glBindBuffer(GL_COPY_WRITE_BUFFER, 0));
}

void VertexBufferClass::GetSubData(size_t offsetInBytes, size_t amountOfBytesToCopy, void* data) const {
    Assert_Binded_Macro;

    glSC(glGetBufferSubData(GL_ARRAY_BUFFER, offsetInBytes, amountOfBytesToCopy, data));
}

void VertexBufferClass::Bind() const {
#if defined KE2_Debug
    BindedInstances.sVertexBufferID(ID);
#endif
	glSC(glBindBuffer(GL_ARRAY_BUFFER, ID));
}
void VertexBufferClass::Unbind() {
    glSC(glBindBuffer(GL_ARRAY_BUFFER, 0));
#if defined KE2_Debug
    BindedInstances.sVertexBufferID(0);
#endif
}