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
    unsigned int off = 0;
    for (unsigned int i = 0; i < Layout.size(); i++) {
        glSC(glVertexAttribPointer(i, Layout[i], GL_FLOAT, GL_FALSE, SumOfLayout * sizeof(float), (void*)off));
        glSC(glEnableVertexAttribArray(i));
        off += Layout[i]*sizeof(float);
    }
}
VertexBuffer::VertexBuffer() {
    glSC(glGenBuffers(1, &ID));
    glSC(glBindBuffer(GL_ARRAY_BUFFER, ID));
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
void VertexBuffer::SetData(const std::vector<float>& data, const VertexBufferDataUsage usage) {
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "VERTEX BUFFER IS DELETED, YOU CANT CHANGE ITS DATA", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION });
    else {
        int GLusage = 0;
        switch (usage) {
        case VertexBufferDataUsage::StreamDraw: {
            GLusage = GL_STREAM_DRAW;
            break;
        }
        case VertexBufferDataUsage::StreamRead: {
            GLusage = GL_STREAM_READ;
            break;
        }
        case VertexBufferDataUsage::StreamCopy: {
            GLusage = GL_STREAM_COPY;
            break;
        }
        case VertexBufferDataUsage::StaticDraw: {
            GLusage = GL_STATIC_DRAW;
            break;
        }
        case VertexBufferDataUsage::StaticRead: {
            GLusage = GL_STATIC_READ;
            break;
        }
        case VertexBufferDataUsage::StaticCopy: {
            GLusage = GL_STATIC_COPY;
            break;
        }
        case VertexBufferDataUsage::DynamicDraw: {
            GLusage = GL_DYNAMIC_DRAW;
            break;
        }
        case VertexBufferDataUsage::DynamicRead: {
            GLusage = GL_DYNAMIC_READ;
            break;
        }
        case VertexBufferDataUsage::DynamicCopy: {
            GLusage = GL_DYNAMIC_COPY;
            break;
        }
        }
        glSC(glBindBuffer(GL_ARRAY_BUFFER, ID));
        glSC(glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &(data[0]), GLusage));
    }
}
void VertexBuffer::SetSubData(unsigned int offset, const std::vector<float>& data) {
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "VERTEX BUFFER IS DELETED, YOU CANT CHANGE ITS SUB DATA", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION });
    else {
        glSC(glBindBuffer(GL_ARRAY_BUFFER, ID));
        glSC(glBufferSubData(GL_ARRAY_BUFFER, offset, data.size() * sizeof(float), &(data[0])));
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