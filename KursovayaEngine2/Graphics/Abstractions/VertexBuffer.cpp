#include"VertexBuffer.h"
#include<string>
#include"Tools/FileTypesReaders/Obj.h"

using namespace KE2;
using namespace Graphics::Abstractions;
namespace GP = Graphics::Primitives;

VertexBufferClass::VertexBufferClass(BufferReadWriteModeEnum bufferReadWriteMode) :BufferReadWriteMode(bufferReadWriteMode) {}
VertexBufferClass::VertexBufferClass(BufferReadWriteModeEnum bufferReadWriteMode, const ArrayView<void>& data) :
	BufferReadWriteMode(bufferReadWriteMode), DataSizeInBytes(data.gLenInBytes()) {
	GP::VertexBufferClass::SetData(data, bufferReadWriteMode);
}
VertexBufferClass::VertexBufferClass(BufferReadWriteModeEnum bufferReadWriteMode, const wchar_t* filePath) :
	BufferReadWriteMode(bufferReadWriteMode) {
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
VertexBufferClass::VertexBufferClass(const VertexBufferClass& toCopy, bool copyBufferData) :
	BufferReadWriteMode(toCopy.BufferReadWriteMode), DataSizeInBytes(toCopy.DataSizeInBytes) {
	GP::VertexBufferClass::SetData(ArrayView<void>(nullptr, DataSizeInBytes), BufferReadWriteMode);
	if (copyBufferData) GP::VertexBufferClass::CopySubData(toCopy, 0, 0, DataSizeInBytes);
}
VertexBufferClass::VertexBufferClass(const VertexBufferClass&& toCopy) :
	GP::VertexBufferClass(std::move(toCopy)),
	BufferReadWriteMode(toCopy.BufferReadWriteMode), DataSizeInBytes(toCopy.DataSizeInBytes) {};
void VertexBufferClass::operator=(const VertexBufferClass& toCopy) {
	Delete();
	new(this) VertexBufferClass(toCopy, false);
}
void VertexBufferClass::operator=(const VertexBufferClass&& toCopy) {
	Delete();
	new(this) VertexBufferClass(std::move(toCopy));
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

void VertexBufferClass::Delete() {
	this->~VertexBufferClass();
}

void VertexBufferClass::Bind() {
	GP::VertexBufferClass::Bind();
}
void VertexBufferClass::Unbind() {
	GP::VertexBufferClass::Unbind();
}
