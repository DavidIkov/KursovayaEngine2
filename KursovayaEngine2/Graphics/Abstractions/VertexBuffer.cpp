#include"VertexBuffer.h"
#include<string>
#include"Tools/FileTypesReaders/Obj.h"

using namespace KE2;
using namespace Graphics::Abstractions;
namespace GP = Graphics::Primitives;

VertexBufferClass::VertexBufferClass(BufferReadWriteModeEnum bufferReadWriteMode) :BufferReadWriteMode(bufferReadWriteMode) { VA.Unbind(); }
VertexBufferClass::VertexBufferClass(BufferReadWriteModeEnum bufferReadWriteMode, const ArrayView<void>& data, const DynArr<LayoutDataStruct>& layout) :
	BufferReadWriteMode(bufferReadWriteMode), DataSizeInBytes(data.gLenInBytes()), Layout(layout) {
	GP::VertexBufferClass::SetData(data, bufferReadWriteMode);
	GP::VertexBufferClass::SetLayout(layout);
	VA.Unbind();
}
VertexBufferClass::VertexBufferClass(BufferReadWriteModeEnum bufferReadWriteMode, const wchar_t* filePath) :
	BufferReadWriteMode(bufferReadWriteMode) {
	VA.Unbind();
	std::wstring filePathStr(filePath);
	std::wstring fileType = filePathStr.substr(filePathStr.find_last_of(L'.') + 1);
	if (fileType == L"obj") {
		std::vector<float> data = ReadObjFileType(filePath);
		DataSizeInBytes = (unsigned int)(data.size() * sizeof(float));
		GP::VertexBufferClass::SetData(ArrayView<void>(&data[0], DataSizeInBytes), bufferReadWriteMode);
	}
	else {
		ErrorsSystemNamespace::SendError << "provided type of file: [" << fileType << "] is not supported" >> ErrorsEnumWrapperStruct(ErrorsEnum::ProvidedTypeOfFileNotSupported);
	}
}
VertexBufferClass::VertexBufferClass(BufferReadWriteModeEnum bufferReadWriteMode, const wchar_t* filePath, const DynArr<LayoutDataStruct>& layout):
	VertexBufferClass(bufferReadWriteMode,filePath) {
	VA.Bind();
	GP::VertexBufferClass::Bind();
	GP::VertexBufferClass::SetLayout(layout);
	VA.Unbind();
}
VertexBufferClass::VertexBufferClass(const VertexBufferClass& toCopy, bool copyBufferData) :
	BufferReadWriteMode(toCopy.BufferReadWriteMode), DataSizeInBytes(toCopy.DataSizeInBytes), Layout(toCopy.Layout) {
	GP::VertexBufferClass::SetLayout(Layout);
	GP::VertexBufferClass::SetData(ArrayView<void>(nullptr, DataSizeInBytes), BufferReadWriteMode);
	if (copyBufferData) GP::VertexBufferClass::CopySubData(toCopy, 0, 0, DataSizeInBytes);
	VA.Unbind();
}
VertexBufferClass::VertexBufferClass(const VertexBufferClass&& toCopy) :
	GP::VertexBufferClass(std::move(toCopy)), VA(std::move(toCopy.VA)), Layout(std::move(toCopy.Layout)),
	BufferReadWriteMode(toCopy.BufferReadWriteMode), DataSizeInBytes(toCopy.DataSizeInBytes) {
	VA.Unbind();
};
void VertexBufferClass::operator=(const VertexBufferClass& toCopy) {
	Delete();
	new(this) VertexBufferClass(toCopy, false);
}
void VertexBufferClass::operator=(const VertexBufferClass&& toCopy) {
	Delete();
	new(this) VertexBufferClass(std::move(toCopy));
}

void VertexBufferClass::SetLayout(DynArr<LayoutDataStruct> layout) {
	VA.Bind();
	GP::VertexBufferClass::SetLayout(layout);
	VA.Unbind();
}

void VertexBufferClass::SetData(const ArrayView<void>& data) {
	if (DataSizeInBytes == data.gLenInBytes())
		GP::VertexBufferClass::SetSubData(0, data);
	else {
		DataSizeInBytes = data.gLenInBytes();
		GP::VertexBufferClass::SetData(data, BufferReadWriteMode);
	}
}

void VertexBufferClass::CopyData(const VertexBufferClass& srcBuffer) {
	if (DataSizeInBytes == srcBuffer.DataSizeInBytes) GP::VertexBufferClass::CopySubData(srcBuffer, 0, 0, DataSizeInBytes);
	else {
		DataSizeInBytes = srcBuffer.DataSizeInBytes;
		GP::VertexBufferClass::SetData(ArrayView<void>(nullptr, DataSizeInBytes), BufferReadWriteMode);
		GP::VertexBufferClass::CopySubData(srcBuffer, 0, 0, DataSizeInBytes);
	}
}
void VertexBufferClass::CopySubData(const VertexBufferClass& srcBuffer, unsigned int srcOffsetInBytes, unsigned int dstOffsetInBytes, unsigned int amountOfBytesToCopy) {
	GP::VertexBufferClass::CopySubData(srcBuffer, srcOffsetInBytes, dstOffsetInBytes, amountOfBytesToCopy);
}

void VertexBufferClass::GetSubData(unsigned int offsetInBytes, unsigned int amountOfBytesToCopy, void* data) {
	GP::VertexBufferClass::GetSubData(offsetInBytes, amountOfBytesToCopy, data);
}
void VertexBufferClass::GetData(void* data) {
	GP::VertexBufferClass::GetSubData(0, DataSizeInBytes, data);
}

VertexBufferClass::BufferReadWriteModeEnum VertexBufferClass::gBufferReadWriteModeEnum() const {
	return BufferReadWriteMode;
}
unsigned int VertexBufferClass::gDataSizeInBytes() const {
	return DataSizeInBytes;
}
const DynArr<VertexBufferClass::LayoutDataStruct>& VertexBufferClass::gLayout() const {
	return Layout;
}

void VertexBufferClass::Delete() {
	this->~VertexBufferClass();
}

void VertexBufferClass::Bind() {
	GP::VertexBufferClass::Bind();
}
void VertexBufferClass::Unbind() {
	GP::VertexBufferClass::Unbind();
}
void VertexBufferClass::BindForRender() {
	VA.Bind();
}
void VertexBufferClass::UnbindFromRender() {
	GP::VertexArrayClass::Unbind();
}
