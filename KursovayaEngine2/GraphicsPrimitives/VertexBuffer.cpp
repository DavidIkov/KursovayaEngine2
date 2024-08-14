#include"VertexBuffer.h"
#include"Tools/GLDebug.h"
#include"Tools/DebuggingTools.h"
#include"glad/glad.h"
#include"Tools/ErrorCodes.h"


void VertexBuffer::UnbindLayout() {
    glSC(glBindBuffer(GL_ARRAY_BUFFER, ID));
    for (unsigned int i = 0; i < Layout.size(); i++) {
        glSC(glDisableVertexAttribArray(i));
    }
}
void VertexBuffer::UpdateLayout() {
    glSC(glBindBuffer(GL_ARRAY_BUFFER, ID));
    float* off = 0;
    for (unsigned int i = 0; i < Layout.size(); i++) {
        glSC(glVertexAttribPointer(i, Layout[i], GL_FLOAT, GL_FALSE, SumOfLayout * sizeof(float), (void*)off));
        glSC(glEnableVertexAttribArray(i));
        off += Layout[i];
    }
}
VertexBuffer::VertexBuffer() {
    glSC(glGenBuffers(1, &ID));
    glSC(glBindBuffer(GL_ARRAY_BUFFER, ID));
}
VertexBuffer::VertexBuffer(VertexBuffer&& tempVB) {
    memcpy(this, &tempVB, sizeof(tempVB));
    tempVB.Deleted = true;
}
VertexBuffer::~VertexBuffer() {
    if (not Deleted) {
        glSC(glDeleteBuffers(1, &ID));
        Deleted = true;
    }
}
void VertexBuffer::Delete() {
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "VERTEX BUFFER IS ALREADY DELETED", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_UNNECESARY_FUNCTION });
    else this->~VertexBuffer();
}
unsigned int VertexBuffer::gID() const {
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "VERTEX BUFFER IS DELETED, ACCESING ITS ID MAY CAUSE ERRORS", KURSAVAYAENGINE2_CORE_ERRORS::ACCESSING_IMPOSSIBLE_TO_ACCESS_INSTANCE_DATA });
    return ID;
}
void VertexBuffer::SetLayout(const std::vector<unsigned int>&& layout) {
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "VERTEX BUFFER IS DELETED, YOU CANT CHANGE ITS LAYOUT", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION });
    else {
        SumOfLayout = 0;
        for (unsigned int i = 0; i < layout.size(); i++) SumOfLayout += layout[i];
        Layout = layout;
        UpdateLayout();
    }
}
void VertexBuffer::AddToLayout(unsigned int len) {
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "VERTEX BUFFER IS DELETED, YOU CANT CHANGE ITS LAYOUT", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION });
    else {
        SumOfLayout += len;
        Layout.push_back(len);
        UpdateLayout();
    }
}

unsigned int VertexBuffer::GetVBUsageForGL(VertexBufferDataUsage usage) {
    switch (usage) {
    case VertexBufferDataUsage::StreamDraw: {
        return GL_STREAM_DRAW;
    }
    case VertexBufferDataUsage::StreamRead: {
        return GL_STREAM_READ;
    }
    case VertexBufferDataUsage::StreamCopy: {
        return GL_STREAM_COPY;
    }
    case VertexBufferDataUsage::StaticDraw: {
        return GL_STATIC_DRAW;
    }
    case VertexBufferDataUsage::StaticRead: {
        return GL_STATIC_READ;
    }
    case VertexBufferDataUsage::StaticCopy: {
        return GL_STATIC_COPY;
    }
    case VertexBufferDataUsage::DynamicDraw: {
        return GL_DYNAMIC_DRAW;
    }
    case VertexBufferDataUsage::DynamicRead: {
        return GL_DYNAMIC_READ;
    }
    case VertexBufferDataUsage::DynamicCopy: {
        return GL_DYNAMIC_COPY;
    }
    }
    return 0;
}
void VertexBuffer::SetData(const std::vector<float>& data, const VertexBufferDataUsage usage) {
    SetData(&(data[0]), (unsigned int)data.size() * sizeof(float), usage);
}
void VertexBuffer::SetData(const float* data, unsigned int dataLength, const VertexBufferDataUsage usage) {
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "VERTEX BUFFER IS DELETED, YOU CANT CHANGE ITS DATA", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION });
    else {

        glSC(glBindBuffer(GL_ARRAY_BUFFER, ID));
        glSC(glBufferData(GL_ARRAY_BUFFER, dataLength, data, GetVBUsageForGL(usage)));
    }
}

void VertexBuffer::ReserveData(unsigned int amountOfElements, VertexBufferDataUsage usage) {
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "VERTEX BUFFER IS DELETED, YOU CANT CHANGE ITS DATA", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION });
    else {

        glSC(glBindBuffer(GL_ARRAY_BUFFER, ID));
        glSC(glBufferData(GL_ARRAY_BUFFER, amountOfElements * sizeof(float), 0, GetVBUsageForGL(usage)));
    }
}
void VertexBuffer::SetSubData(unsigned int offset, const std::vector<float>& data) {
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "VERTEX BUFFER IS DELETED, YOU CANT CHANGE ITS SUB DATA", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION });
    else {
        glSC(glBindBuffer(GL_ARRAY_BUFFER, ID));
        glSC(glBufferSubData(GL_ARRAY_BUFFER, offset, data.size() * sizeof(float), &(data[0])));
    }
}
void VertexBuffer::SetSubData(unsigned int offset, unsigned int elementsAmount, float* firstElement) {
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "VERTEX BUFFER IS DELETED, YOU CANT CHANGE ITS SUB DATA", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION });
    else {
        glSC(glBindBuffer(GL_ARRAY_BUFFER, ID));
        glSC(glBufferSubData(GL_ARRAY_BUFFER, offset, elementsAmount * sizeof(float), firstElement));
    }
}
void VertexBuffer::Bind() {
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "VERTEX BUFFER IS DELETED, YOU CANT BIND IT", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION });
    else {
        glSC(glBindBuffer(GL_ARRAY_BUFFER, ID));
    }
}
void VertexBuffer::Unbind() {
    glSC(glBindBuffer(GL_ARRAY_BUFFER, 0));
}