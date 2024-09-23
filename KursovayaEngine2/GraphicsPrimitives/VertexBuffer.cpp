#include"VertexBuffer.h"
#include"Tools/GLDebug.h"
#include"Tools/DebuggingTools.h"
#include"glad/glad.h"
#include"Tools/ErrorCodes.h"

using namespace GraphicsPrimitives;

VertexBufferClass::VertexBufferClass() {
    glSC(glGenBuffers(1, &ID));
    glSC(glBindBuffer(GL_ARRAY_BUFFER, ID));
}
VertexBufferClass::VertexBufferClass(const VertexBufferClass* toCopy) {
    memcpy(this, toCopy, sizeof(VertexBufferClass));
    toCopy->Deleted = true;
}
VertexBufferClass::VertexBufferClass(const VertexBufferClass&& toCopy) {
    memcpy(this, &toCopy, sizeof(VertexBufferClass));
    toCopy.Deleted = true;
}
void VertexBufferClass::operator=(const VertexBufferClass&& toCopy) {
    memcpy(this, &toCopy, sizeof(VertexBufferClass));
    toCopy.Deleted = true;
}
VertexBufferClass::~VertexBufferClass() {
    if (not Deleted) {
        glSC(glDeleteBuffers(1, &ID));
        Deleted = true;
    }
}
void VertexBufferClass::Delete() {
#if defined Debug
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "VERTEX BUFFER IS ALREADY DELETED", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_UNNECESARY_FUNCTION });
    else 
#endif
        this->~VertexBufferClass();
}
unsigned int VertexBufferClass::gID() const {
#if defined Debug
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "VERTEX BUFFER IS DELETED, ACCESING ITS ID MAY CAUSE ERRORS", KURSAVAYAENGINE2_CORE_ERRORS::ACCESSING_IMPOSSIBLE_TO_ACCESS_INSTANCE_DATA });
#endif
    return ID;
}
void VertexBufferClass::SetLayout(BufferDataTypeEnum dataType, const std::vector<unsigned int>& layout) {
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
        case BufferDataTypeEnum::Byte: gl_dataType = GL_BYTE; break;
        case BufferDataTypeEnum::UnsignedByte: gl_dataType = GL_UNSIGNED_BYTE; break;
        case BufferDataTypeEnum::Float: gl_dataType = GL_FLOAT; break;
        case BufferDataTypeEnum::Int: gl_dataType = GL_INT; break;
        case BufferDataTypeEnum::UnsignedInt: gl_dataType = GL_UNSIGNED_INT; break;
        }

        float* off = 0;
        for (unsigned int i = 0; i < layout.size(); i++) {
            glSC(glVertexAttribPointer(i, layout[i], GL_FLOAT, GL_FALSE, sumOfLayout * sizeof(float), (void*)off));
            glSC(glEnableVertexAttribArray(i));
            off += layout[i];
        }
    }
}

unsigned int VertexBufferClass::_GetVBUsageForGL(BufferDataUsageEnum usage) {
    switch (usage) {
    case BufferDataUsageEnum::StreamDraw: return GL_STREAM_DRAW;
    case BufferDataUsageEnum::StreamRead: return GL_STREAM_READ;
    case BufferDataUsageEnum::StreamCopy: return GL_STREAM_COPY;
    case BufferDataUsageEnum::StaticDraw: return GL_STATIC_DRAW;
    case BufferDataUsageEnum::StaticRead: return GL_STATIC_READ;
    case BufferDataUsageEnum::StaticCopy: return GL_STATIC_COPY;
    case BufferDataUsageEnum::DynamicDraw: return GL_DYNAMIC_DRAW;
    case BufferDataUsageEnum::DynamicRead: return GL_DYNAMIC_READ;
    case BufferDataUsageEnum::DynamicCopy: return GL_DYNAMIC_COPY;
    }
    return 0;
}
void VertexBufferClass::SetData(unsigned int dataLength, const void* data, const BufferDataUsageEnum usage) {
#if defined Debug
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "VERTEX BUFFER IS DELETED, YOU CANT CHANGE ITS DATA", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION });
    else 
#endif
    {

        glSC(glBindBuffer(GL_ARRAY_BUFFER, ID));
        glSC(glBufferData(GL_ARRAY_BUFFER, dataLength, data, _GetVBUsageForGL(usage)));
    }
}

void VertexBufferClass::SetSubData(unsigned int offset, unsigned int dataLength, void* firstElement) {
#if defined Debug
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "VERTEX BUFFER IS DELETED, YOU CANT CHANGE ITS SUB DATA", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION });
    else 
#endif
    {
        glSC(glBindBuffer(GL_ARRAY_BUFFER, ID));
        glSC(glBufferSubData(GL_ARRAY_BUFFER, offset, dataLength, firstElement));
    }
}
void VertexBufferClass::Bind() const {
#if defined Debug
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "VERTEX BUFFER IS DELETED, YOU CANT BIND IT", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION });
    else 
#endif
    {
        glSC(glBindBuffer(GL_ARRAY_BUFFER, ID));
    }
}
void VertexBufferClass::Unbind() {
    glSC(glBindBuffer(GL_ARRAY_BUFFER, 0));
}