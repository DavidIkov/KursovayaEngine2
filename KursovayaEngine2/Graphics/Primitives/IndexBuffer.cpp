#include"IndexBuffer.h"
#include"Tools/GLDebug.h"
#include"glad/glad.h"
#include"Graphics/Globals.h"

using namespace KE2;
using namespace Graphics::Primitives;
#if defined KE2_Debug
#define Assert_Binded_Macro if(BindedInstances.gIndexBufferID() != ID) ErrorsSystemNamespace::SendWarning<<"IndexBuffer is not binded">>ErrorsSystemNamespace::EndOfWarning;
#else
#define Assert_Binded_Macro
#endif

IndexBufferClass::IndexBufferClass() {
    glSC(glGenBuffers(1, &ID));
    Bind();
}
IndexBufferClass::IndexBufferClass(IndexBufferClass&& toCopy) noexcept :
    ID(toCopy.ID) {
    toCopy.ID = 0u;
}
IndexBufferClass& IndexBufferClass::operator=(IndexBufferClass&& toCopy) {
    this->~IndexBufferClass();
    new(this) IndexBufferClass(std::move(toCopy));
    return *this;
}
IndexBufferClass::~IndexBufferClass() noexcept(false) {
    if (ID != 0u) {
        Unbind();
        glSC(glDeleteBuffers(1, &ID));
        ID = 0u;
    }
};

static unsigned int _GetIBUsageForGL(IndexBufferClass::BufferReadWriteModeEnum usage) {
    switch (usage) {
    case IndexBufferClass::BufferReadWriteModeEnum::StreamDraw: return GL_STREAM_DRAW;
    case IndexBufferClass::BufferReadWriteModeEnum::StreamRead: return GL_STREAM_READ;
    case IndexBufferClass::BufferReadWriteModeEnum::StreamCopy: return GL_STREAM_COPY;
    case IndexBufferClass::BufferReadWriteModeEnum::StaticDraw: return GL_STATIC_DRAW;
    case IndexBufferClass::BufferReadWriteModeEnum::StaticRead: return GL_STATIC_READ;
    case IndexBufferClass::BufferReadWriteModeEnum::StaticCopy: return GL_STATIC_COPY;
    case IndexBufferClass::BufferReadWriteModeEnum::DynamicDraw: return GL_DYNAMIC_DRAW;
    case IndexBufferClass::BufferReadWriteModeEnum::DynamicRead: return GL_DYNAMIC_READ;
    case IndexBufferClass::BufferReadWriteModeEnum::DynamicCopy: return GL_DYNAMIC_COPY;
    }
    return 0;
}
void IndexBufferClass::SetData(const ArrayView<void>& data, const BufferReadWriteModeEnum bufferReadWriteMode) const {
    Assert_Binded_Macro;

	glSC(glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.gLenInBytes() * sizeof(unsigned int), data.gDataPtr(), _GetIBUsageForGL(bufferReadWriteMode)));
}
void IndexBufferClass::SetSubData(unsigned int offsetInBytes, const ArrayView<void>& data) const {
    Assert_Binded_Macro;

	glSC(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offsetInBytes, data.gLenInBytes() * sizeof(unsigned int), data.gDataPtr()));
}

void IndexBufferClass::CopySubData(const IndexBufferClass& srcBuffer, unsigned int srcOffsetInBytes, unsigned int dstOffsetInBytes, unsigned int amountOfBytesToCopy) const {
    glSC(glBindBuffer(GL_COPY_READ_BUFFER, srcBuffer.ID));
    glSC(glBindBuffer(GL_COPY_WRITE_BUFFER, ID));

    glSC(glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, srcOffsetInBytes, dstOffsetInBytes, amountOfBytesToCopy));

    glSC(glBindBuffer(GL_COPY_READ_BUFFER, 0));
    glSC(glBindBuffer(GL_COPY_WRITE_BUFFER, 0));
}

void IndexBufferClass::GetSubData(unsigned int offsetInBytes, unsigned int amountOfBytesToCopy, void* data) const {
    Assert_Binded_Macro;

    glSC(glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offsetInBytes, amountOfBytesToCopy, data));
}

void IndexBufferClass::Bind() const {
#if defined KE2_Debug
    BindedInstances.sIndexBufferID(ID);
#endif
	glSC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID));
}
void IndexBufferClass::Unbind() {
    glSC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
#if defined KE2_Debug
    BindedInstances.sIndexBufferID(0);
#endif
}