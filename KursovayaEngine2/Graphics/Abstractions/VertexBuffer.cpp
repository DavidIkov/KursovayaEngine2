#include"VertexBuffer.h"
#include<string>
/*
using namespace Graphics::Abstractions;
namespace GP = Graphics::Primitives;

VertexBufferClass::VertexBufferClass() {}
VertexBufferClass::VertexBufferClass(BufferReadWriteModeEnum bufferReadWriteMode, const void* data, unsigned int dataSizeInBytes, const DynArr<LayoutDataStruct>& layout) :
	BufferReadWriteMode(bufferReadWriteMode), DataSizeInBytes(dataSizeInBytes), Layout(layout) {
	GP::VertexBufferClass::SetData(data, dataSizeInBytes, bufferReadWriteMode);
	GP::VertexBufferClass::SetLayout(layout);
	VA.Unbind();
	GP::VertexBufferClass::Unbind();
}
VertexBufferClass::VertexBufferClass(const wchar_t* filePath) {
	std::wstring filePathStr(filePath);
	if (filePathStr.substr(filePathStr.size() - 3, 3) == L"obj") {

	}
	else DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical,L"the type of file \"" + filePathStr + L"\" is not accepted",KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_FUNCTION_WITH_INVALID_ARGUMENTS });
}
DLLTREATMENT VertexBufferClass(const VertexBufferClass&& toCopy);
DLLTREATMENT void operator=(const VertexBufferClass&& toCopy);

DLLTREATMENT void SetLayout(DynArr<LayoutDataStruct> layout);

DLLTREATMENT void SetData(const void* data, unsigned int dataSizeInBytes);
using Primitives::VertexBufferClass::SetSubData;

DLLTREATMENT void CopyData(const VertexBufferClass& srcBuffer);
DLLTREATMENT void CopySubData(const VertexBufferClass& srcBuffer, unsigned int srcOffsetInBytes, unsigned int dstOffsetInBytes, unsigned int amountOfBytesToCopy);

//data should point to already allocated memory
DLLTREATMENT void GetSubData(unsigned int offsetInBytes, unsigned int amountOfBytesToCopy, void* data);
//data should point to already allocated memory
DLLTREATMENT void GetData(void* data);

DLLTREATMENT void Bind();
DLLTREATMENT void Unbind();
DLLTREATMENT void BindForRender();
DLLTREATMENT void UnbindFromRender();
*/
