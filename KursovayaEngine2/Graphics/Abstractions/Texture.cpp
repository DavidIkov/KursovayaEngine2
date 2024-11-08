#include"Texture.h"

using namespace Graphics::Abstractions;
namespace GP = Graphics::Primitives;

TextureClass::TextureClass(DimensionsEnum dimensions, const char* filePath, const SettingsStruct& sets, const DataSettingsStruct& dataSets) :
	GP::TextureClass(dimensions, filePath, &Size, nullptr, sets, dataSets), 
	Settings(sets), DataSettings(dataSets) {}
TextureClass::TextureClass(DimensionsEnum dimensions, Vector3U pixelsAmount, const void* data, const SettingsStruct& sets, const DataSettingsStruct& dataSets) :
	Size(pixelsAmount), GP::TextureClass(dimensions, pixelsAmount, data, sets, dataSets), Settings(sets), DataSettings(dataSets) {}

TextureClass::TextureClass(const TextureClass& toCopy) :
	Size(toCopy.Size), Settings(toCopy.Settings), DataSettings(toCopy.DataSettings),
	GP::TextureClass(Dimensions, Size, nullptr, Settings, DataSettings) {
	GP::TextureClass::CopySubData(toCopy, Vector3U(0u), Vector3U(0u), Size);
};
TextureClass::TextureClass(const TextureClass&& toCopy):
	GP::TextureClass(std::move(toCopy)), Size(toCopy.Size),
	Settings(toCopy.Settings), DataSettings(toCopy.DataSettings) { toCopy.Deleted = true; }
TextureClass::~TextureClass() {
	Deleted = true;
}
void TextureClass::operator=(const TextureClass& toCopy) {
	Delete();
	Deleted = false;
	
	Settings = toCopy.Settings;
	DataSettings = toCopy.DataSettings;
	Dimensions = toCopy.Dimensions;
	if (Size != toCopy.Size)
		GP::TextureClass::SetData(Size, nullptr, DataSettings);
	Size = toCopy.Size;
	GP::TextureClass::CopySubData(toCopy, Vector3U(0u), Vector3U(0u), Size);
}
void TextureClass::operator=(const TextureClass&& toCopy) {
	Delete();
	Deleted = false;
	toCopy.Deleted = true;
	
	Size = toCopy.Size;
	Settings = toCopy.Settings;
	DataSettings = toCopy.DataSettings;
	Dimensions = toCopy.Dimensions;
	GP::TextureClass::operator=(std::move(toCopy));
}
void TextureClass::ChangeData(Vector3U newSize, const void* data) {

	if (newSize == Size)
		GP::TextureClass::SetSubData(Vector3U(0u), newSize, data, DataSettings.DataFormatOnCPU, DataSettings.DataTypeOnCPU);
	else {
		GP::TextureClass::SetData(newSize, data, DataSettings);
		Size = newSize;
	}
}
void TextureClass::ChangeData(const void* data) {
	ChangeData(Size, data);
}
void TextureClass::ChangeSubData(Vector3U offset, const void* data, Vector3U size) {
	GP::TextureClass::SetSubData(offset, size, data, DataSettings.DataFormatOnCPU, DataSettings.DataTypeOnCPU);
}
void TextureClass::AllocatePixels(Vector3U newSize) {
	GP::TextureClass::SetData(newSize, nullptr, DataSettings);
}
void TextureClass::CopySubData(const TextureClass& srcTex, Vector3U offsetInSrc, Vector3U offsetInDst, Vector3U pixelsAmount) {
	GP::TextureClass::CopySubData(srcTex, offsetInSrc, offsetInDst, pixelsAmount);
}
void TextureClass::GetData(void* buffer) {
	GP::TextureClass::GetData(buffer, DataSettings.DataFormatOnCPU, DataSettings.DataTypeOnCPU);
}
void TextureClass::GetSubData(Vector3U offset, void* buffer, Vector3U pixelsAmount) {
	GP::TextureClass::GetSubData(offset, buffer, pixelsAmount, DataSettings.DataFormatOnCPU, DataSettings.DataTypeOnCPU);
}

Vector3U TextureClass::gSize() { 
	return Size; 
}

void TextureClass::sSettings_WrapTypeByX(SettingsStruct::WrapTypeEnum wrapTypeByX) { Settings.WrapTypeByX = wrapTypeByX; GP::TextureClass::sSettings_WrapTypeByX(wrapTypeByX); }
TextureClass::SettingsStruct::WrapTypeEnum TextureClass::gSettings_WrapTypeByX() { return Settings.WrapTypeByX; }
void TextureClass::sSettings_WrapTypeByY(SettingsStruct::WrapTypeEnum wrapTypeByY) { Settings.WrapTypeByY = wrapTypeByY; GP::TextureClass::sSettings_WrapTypeByY(wrapTypeByY); }
TextureClass::SettingsStruct::WrapTypeEnum TextureClass::gSettings_WrapTypeByY() { return Settings.WrapTypeByY; }
void TextureClass::sSettings_DownscalingFilt(SettingsStruct::DownscalingFilterFuncEnum downscalingFilt) { Settings.DownscalingFilt = downscalingFilt; GP::TextureClass::sSettings_DownscalingFilt(downscalingFilt); }
TextureClass::SettingsStruct::DownscalingFilterFuncEnum TextureClass::gSettings_DownscalingFilt() { return Settings.DownscalingFilt; }
void TextureClass::sSettings_UpscalingFilt(SettingsStruct::UpscalingFilterFuncEnum upscalingFilt) { Settings.UpscalingFilt = upscalingFilt; GP::TextureClass::sSettings_UpscalingFilt(upscalingFilt); }
TextureClass::SettingsStruct::UpscalingFilterFuncEnum TextureClass::gSettings_UpscalingFilt() { return Settings.UpscalingFilt; }
void TextureClass::sSettings_DepthStencilReadMode(SettingsStruct::DepthStencilReadModeEnum depthStencilReadMode) { Settings.DepthStencilReadMode = depthStencilReadMode; GP::TextureClass::sSettings_DepthStencilReadMode(depthStencilReadMode); }
TextureClass::SettingsStruct::DepthStencilReadModeEnum TextureClass::gSettings_DepthStencilReadMode() { return Settings.DepthStencilReadMode; }

TextureClass::SettingsStruct TextureClass::gSettings() { return Settings; }
void TextureClass::sSettings(SettingsStruct newSets) {
	sSettings_WrapTypeByX(newSets.WrapTypeByX);
	sSettings_WrapTypeByY(newSets.WrapTypeByY);
	sSettings_DownscalingFilt(newSets.DownscalingFilt);
	sSettings_UpscalingFilt(newSets.UpscalingFilt);
	sSettings_DepthStencilReadMode(newSets.DepthStencilReadMode);
}

void TextureClass::SetDataSettings_DataFormatOnGPU(bool instantUpdate, DataSettingsStruct::DataFormatOnGPU_Enum dataFormat) {
	DataSettings.DataFormatOnGPU = dataFormat;
	if (instantUpdate) {
		GP::TextureClass tempTex(Dimensions, Size, nullptr, Settings, DataSettings);
		tempTex.CopySubData(*this, Vector3U(0u), Vector3U(0u), Size);
		GP::TextureClass::operator=(std::move(tempTex));
	}
}
TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum TextureClass::gDataSettings_DataFormatOnGPU() { return DataSettings.DataFormatOnGPU; }
void TextureClass::sDataSettings_DataFormatOnCPU(DataSettingsStruct::DataFormatOnCPU_Enum dataFormat) { DataSettings.DataFormatOnCPU = dataFormat; }
TextureClass::DataSettingsStruct::DataFormatOnCPU_Enum TextureClass::gDataSettings_DataFormatOnCPU() { return DataSettings.DataFormatOnCPU; }
void TextureClass::sDataSettings_DataTypeOnCPU(DataSettingsStruct::DataTypeOnCPU_Enum dataType) { DataSettings.DataTypeOnCPU = dataType; }
TextureClass::DataSettingsStruct::DataTypeOnCPU_Enum TextureClass::gDataSettings_DataTypeOnCPU() { return DataSettings.DataTypeOnCPU; }

TextureClass::DataSettingsStruct TextureClass::gDataSettings() { return DataSettings; }
void TextureClass::sDataSettings(bool instantUpdateOfDataFormatOnGPU, DataSettingsStruct newDataSets) { 
	SetDataSettings_DataFormatOnGPU(instantUpdateOfDataFormatOnGPU, newDataSets.DataFormatOnGPU);
	sDataSettings_DataFormatOnCPU(newDataSets.DataFormatOnCPU);
	sDataSettings_DataTypeOnCPU(newDataSets.DataTypeOnCPU);
}


void TextureClass::Delete() {
	this->~TextureClass();
}

const GP::TextureClass& TextureClass::gPrimitiveTexture() {
	return *this;
}
GP::TextureClass::CFAC_FullAccess_Class TextureClass::gPrimitiveTextureCFAC() {
	return GP::TextureClass::CFAC_FullAccess_Class(*this);
}


