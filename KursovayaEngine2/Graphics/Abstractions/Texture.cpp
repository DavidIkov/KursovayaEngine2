#include"Texture.h"

using namespace KE2;
using namespace Graphics::Abstractions;
namespace GP = Graphics::Primitives;

TextureClass::TextureClass(DimensionsEnum dimensions, const char* filePath, unsigned int mipmapLevels, SettingsStruct sets, DataSettingsStruct dataSets) :
	GP::TextureClass(dimensions, filePath, &Size, nullptr, mipmapLevels, sets, dataSets), 
	Settings(sets), DataSettings(dataSets), MipmapLevels(mipmapLevels) {}
TextureClass::TextureClass(DimensionsEnum dimensions, Vector3U pixelsAmount, const void* data, unsigned int mipmapLevels, SettingsStruct sets, DataSettingsStruct dataSets) :
	GP::TextureClass(dimensions, pixelsAmount, data, mipmapLevels, sets, dataSets), 
	Size(pixelsAmount), Settings(sets), DataSettings(dataSets), MipmapLevels(mipmapLevels) {}
TextureClass::TextureClass(DimensionsEnum dimensions, Vector3U pixelsAmount, unsigned int mipmapLevels, DataSettingsStruct::DataFormatOnGPU_Enum dataFormatOnGPU, SettingsStruct sets) :
	GP::TextureClass(dimensions, pixelsAmount, mipmapLevels, dataFormatOnGPU, sets), Size(pixelsAmount),
	Settings(sets), DataSettings{ dataFormatOnGPU,DataSettingsStruct::DataFormatOnCPU_Enum::None,DataSettingsStruct::DataTypeOnCPU_Enum::None }, MipmapLevels(mipmapLevels) {};
TextureClass::TextureClass(const TextureClass& toCopy, bool copyTextureData) :
	GP::TextureClass(toCopy.Dimensions, toCopy.Size, toCopy.MipmapLevels, toCopy.DataSettings.DataFormatOnGPU, Settings),
	Size(toCopy.Size), Settings(toCopy.Settings), DataSettings(toCopy.DataSettings), MipmapLevels(toCopy.MipmapLevels) {
	if (copyTextureData) GP::TextureClass::CopySubData(toCopy, Vector3U(0u), Vector3U(0u), Size);
};
TextureClass::TextureClass(TextureClass&& toCopy) noexcept:
	GP::TextureClass(std::move(toCopy)), Size(toCopy.Size),
	Settings(toCopy.Settings), DataSettings(toCopy.DataSettings) { }
TextureClass& TextureClass::operator=(const TextureClass& toCopy) {
	this->~TextureClass();
	new(this) TextureClass(toCopy, false);
	return *this;
}
TextureClass& TextureClass::operator=(TextureClass&& toCopy) {
	this->~TextureClass();
	new(this) TextureClass(std::move(toCopy));
	return *this;
}
void TextureClass::SetSubData(Vector3U pixelsOffset, Vector3U pixelsAmount, const void* data) {
	GP::TextureClass::SetSubData(pixelsOffset, pixelsAmount, data, DataSettings.DataFormatOnCPU, DataSettings.DataTypeOnCPU);
}
void TextureClass::CopySubData(const TextureClass& srcTex, Vector3U offsetInSrc, Vector3U offsetInDst, Vector3U pixelsAmount) {
	GP::TextureClass::CopySubData(srcTex, offsetInSrc, offsetInDst, pixelsAmount);
}
void TextureClass::GetData(void* buffer) const {
	GP::TextureClass::GetData(buffer, DataSettings.DataFormatOnCPU, DataSettings.DataTypeOnCPU);
}
void TextureClass::GetSubData(Vector3U offset, void* buffer, Vector3U pixelsAmount) const {
	GP::TextureClass::GetSubData(offset, buffer, pixelsAmount, DataSettings.DataFormatOnCPU, DataSettings.DataTypeOnCPU);
}

void TextureClass::sSettings_WrapTypeByX(SettingsStruct::WrapTypeEnum wrapTypeByX) { Settings.WrapTypeByX = wrapTypeByX; GP::TextureClass::sSettings_WrapTypeByX(wrapTypeByX); }
void TextureClass::sSettings_WrapTypeByY(SettingsStruct::WrapTypeEnum wrapTypeByY) { Settings.WrapTypeByY = wrapTypeByY; GP::TextureClass::sSettings_WrapTypeByY(wrapTypeByY); }
void TextureClass::sSettings_DownscalingFilt(SettingsStruct::DownscalingFilterFuncEnum downscalingFilt) { Settings.DownscalingFilt = downscalingFilt; GP::TextureClass::sSettings_DownscalingFilt(downscalingFilt); }
void TextureClass::sSettings_UpscalingFilt(SettingsStruct::UpscalingFilterFuncEnum upscalingFilt) { Settings.UpscalingFilt = upscalingFilt; GP::TextureClass::sSettings_UpscalingFilt(upscalingFilt); }
void TextureClass::sSettings_DepthStencilReadMode(SettingsStruct::DepthStencilReadModeEnum depthStencilReadMode) { Settings.DepthStencilReadMode = depthStencilReadMode; GP::TextureClass::sSettings_DepthStencilReadMode(depthStencilReadMode); }

void TextureClass::sSettings(SettingsStruct newSets) {
	sSettings_WrapTypeByX(newSets.WrapTypeByX);
	sSettings_WrapTypeByY(newSets.WrapTypeByY);
	sSettings_DownscalingFilt(newSets.DownscalingFilt);
	sSettings_UpscalingFilt(newSets.UpscalingFilt);
	sSettings_DepthStencilReadMode(newSets.DepthStencilReadMode);
}

