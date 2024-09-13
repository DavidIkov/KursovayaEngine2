#include"VertexBuffer.h"
#include"Tools/GLDebug.h"
#include"Tools/DebuggingTools.h"
#include"glad/glad.h"
#include"Tools/ErrorCodes.h"

VertexBuffer::VertexBuffer() {
    glSC(glGenBuffers(1, &ID));
    glSC(glBindBuffer(GL_ARRAY_BUFFER, ID));
}
VertexBuffer::VertexBuffer(const VertexBuffer* toCopy) {
    memcpy(this, toCopy, sizeof(VertexBuffer));
    toCopy->Deleted = true;
}
VertexBuffer::VertexBuffer(const VertexBuffer&& toCopy) {
    memcpy(this, &toCopy, sizeof(VertexBuffer));
    toCopy.Deleted = true;
}
void VertexBuffer::operator=(const VertexBuffer&& toCopy) {
    memcpy(this, &toCopy, sizeof(VertexBuffer));
    toCopy.Deleted = true;
}
VertexBuffer::~VertexBuffer() {
    if (not Deleted) {
        glSC(glDeleteBuffers(1, &ID));
        Deleted = true;
    }
}
void VertexBuffer::Delete() {
#if defined Debug
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "VERTEX BUFFER IS ALREADY DELETED", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_UNNECESARY_FUNCTION });
    else 
#endif
        this->~VertexBuffer();
}
unsigned int VertexBuffer::gID() const {
#if defined Debug
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "VERTEX BUFFER IS DELETED, ACCESING ITS ID MAY CAUSE ERRORS", KURSAVAYAENGINE2_CORE_ERRORS::ACCESSING_IMPOSSIBLE_TO_ACCESS_INSTANCE_DATA });
#endif
    return ID;
}
void VertexBuffer::SetLayout(BufferDataType dataType, const std::vector<unsigned int>& layout) {
#if defined Debug
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "VERTEX BUFFER IS DELETED, YOU CANT CHANGE ITS LAYOUT", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION });
    else 
#endif
    {
        glSC(glBindBuffer(GL_ARRAY_BUFFER, ID));

        for (unsigned int i = 0; i < EnabledAttributesAmount; i++) { glSC(glDisableVertexAttribArray(i)); }
        EnabledAttributesAmount = (unsigned short int)layout.size();

        unsigned int sumOfLayout = 0; for (unsigned int i = 0; i < layout.size(); i++) sumOfLayout += layout[i];

        unsigned int gl_dataType = 0;
        switch (dataType) {
        case BufferDataType::Byte: gl_dataType = GL_BYTE; break;
        case BufferDataType::UnsignedByte: gl_dataType = GL_UNSIGNED_BYTE; break;
        case BufferDataType::Float: gl_dataType = GL_FLOAT; break;
        case BufferDataType::Int: gl_dataType = GL_INT; break;
        case BufferDataType::UnsignedInt: gl_dataType = GL_UNSIGNED_INT; break;
        }

        float* off = 0;
        for (unsigned int i = 0; i < layout.size(); i++) {
            glSC(glVertexAttribPointer(i, layout[i], GL_FLOAT, GL_FALSE, sumOfLayout * sizeof(float), (void*)off));
            glSC(glEnableVertexAttribArray(i));
            off += layout[i];
        }
    }
}

unsigned int VertexBuffer::_GetVBUsageForGL(BufferDataUsage usage) {
    switch (usage) {
    case BufferDataUsage::StreamDraw: return GL_STREAM_DRAW;
    case BufferDataUsage::StreamRead: return GL_STREAM_READ;
    case BufferDataUsage::StreamCopy: return GL_STREAM_COPY;
    case BufferDataUsage::StaticDraw: return GL_STATIC_DRAW;
    case BufferDataUsage::StaticRead: return GL_STATIC_READ;
    case BufferDataUsage::StaticCopy: return GL_STATIC_COPY;
    case BufferDataUsage::DynamicDraw: return GL_DYNAMIC_DRAW;
    case BufferDataUsage::DynamicRead: return GL_DYNAMIC_READ;
    case BufferDataUsage::DynamicCopy: return GL_DYNAMIC_COPY;
    }
    return 0;
}
void VertexBuffer::SetData(unsigned int dataLength, const void* data, const BufferDataUsage usage) {
#if defined Debug
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "VERTEX BUFFER IS DELETED, YOU CANT CHANGE ITS DATA", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION });
    else 
#endif
    {

        glSC(glBindBuffer(GL_ARRAY_BUFFER, ID));
        glSC(glBufferData(GL_ARRAY_BUFFER, dataLength, data, _GetVBUsageForGL(usage)));
    }
}

void VertexBuffer::SetSubData(unsigned int offset, unsigned int dataLength, void* firstElement) {
#if defined Debug
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "VERTEX BUFFER IS DELETED, YOU CANT CHANGE ITS SUB DATA", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION });
    else 
#endif
    {
        glSC(glBindBuffer(GL_ARRAY_BUFFER, ID));
        glSC(glBufferSubData(GL_ARRAY_BUFFER, offset, dataLength, firstElement));
    }
}
void VertexBuffer::Bind() const {
#if defined Debug
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "VERTEX BUFFER IS DELETED, YOU CANT BIND IT", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION });
    else 
#endif
    {
        glSC(glBindBuffer(GL_ARRAY_BUFFER, ID));
    }
}
void VertexBuffer::Unbind() {
    glSC(glBindBuffer(GL_ARRAY_BUFFER, 0));
}