#include"Texture.h"
#include"Tools/DebugRuntimeAssert.h"
#include<cstring>
#include<utility>

using namespace Graphics::Abstractions;
namespace GP = Graphics::Primitives;
#define Assert_NotDeleted_Macro DebugRuntimeAssert(DebuggingTools::ErrorTypes::Critical, not Deleted, "TextureClass is deleted", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION);

TextureClass::TextureClass(DimensionsEnum dimensions, bool storeData, const char* filePath, const SettingsStruct& sets, const DataSettingsStruct& dataSets) :
	Dimensions(dimensions), Texture(dimensions, filePath, &Size, storeData ? &TexData : nullptr, sets, dataSets), 
	IsStoringTexData(storeData), Settings(sets), DataSettings(dataSets) {}
TextureClass::TextureClass(DimensionsEnum dimensions, Vector3U pixelsAmount, bool storeData, AnonDynArr* data, const SettingsStruct& sets, const DataSettingsStruct& dataSets) :
	Dimensions(dimensions), Size(pixelsAmount), Texture(dimensions, pixelsAmount, data, sets, dataSets), Settings(sets), DataSettings(dataSets), 
	IsStoringTexData(storeData) {
	if (IsStoringTexData) TexData = AnonDynArr(std::move(*data));
}

TextureClass::TextureClass(const TextureClass&& toCopy):
	Texture(std::move(toCopy.Texture)), Size(toCopy.Size), TexData(std::move(toCopy.TexData)), IsStoringTexData(toCopy.IsStoringTexData), 
	Settings(toCopy.Settings), DataSettings(toCopy.DataSettings), Dimensions(toCopy.Dimensions) {
	toCopy.Deleted = true;
}
TextureClass::~TextureClass() {
	Deleted = true;
}
void TextureClass::operator=(const TextureClass&& toCopy) {
	Delete();
	Deleted = false;
	toCopy.Deleted = true;
	
	Size = toCopy.Size;
	TexData = AnonDynArr(std::move(toCopy.TexData));
	IsStoringTexData = toCopy.IsStoringTexData;
	Settings = toCopy.Settings;
	DataSettings = toCopy.DataSettings;
	Dimensions = toCopy.Dimensions;
	Texture = GP::TextureClass(std::move(toCopy.Texture));
}
void TextureClass::ChangeData(Vector3U newSize, void* data, unsigned int dataSizeInBits) {
	Assert_NotDeleted_Macro;

	if (not DataFormatOnGPU_WasUpdated and newSize == Size)
		Texture.SetSubData(Vector3U(0u), newSize, data, DataSettings.DataFormatOnCPU, DataSettings.DataTypeOnCPU);
	else Texture.SetData(newSize, data, DataSettings);

	if (IsStoringTexData)
		TexData.SetData((byte*)data, dataSizeInBits);

}
void TextureClass::ChangeSubData(Vector3U offset, Vector3U size, const void* data) {
	Assert_NotDeleted_Macro;

	if (IsStoringTexData) {
		if (offset == Vector3U(0u) and size == Size) {
			TexData.SetSubData(0, (byte*)data, TexData.gArrSizeInBytes());
		}
		else {
			switch (Dimensions) {
			case DimensionsEnum::One: {
				unsigned int pixelBitSize = TexData.gArrSizeInBytes() / Size[0];
				TexData.SetSubData(offset[0] * pixelBitSize, (byte*)data, size[0] * pixelBitSize);
				break;
			}
			case DimensionsEnum::Two: {
				unsigned int pixelBitSize = TexData.gArrSizeInBytes() / (Size[0] * Size[1]);
				for (unsigned int y = 0; y < size[1]; y++)
					TexData.SetSubData(pixelBitSize * (Size[0] * (y + offset[1]) + offset[0]), (unsigned char*)data + size[0] * y * pixelBitSize, size[0] * pixelBitSize);
				break;
			}
			case DimensionsEnum::Three: {
				unsigned int pixelBitSize = TexData.gArrSizeInBytes() / (Size[0] * Size[1] * Size[2]);
				for (unsigned int y = 0; y < size[1]; y++) for (unsigned int z = 0; z < size[2]; z++)
					TexData.SetSubData(pixelBitSize * (offset[0] + Size[0] * (y + offset[1]) + Size[0] * Size[1] * (z + offset[2])),
						(unsigned char*)data + pixelBitSize * (y * size[0] + z * size[0] * size[1]), size[0] * pixelBitSize);
			}
			}
		}
	}
	if (DataFormatOnGPU_WasUpdated)
		Texture.SetData(Size, TexData.gArr(), DataSettings);
	else
		Texture.SetSubData(offset, size, TexData.gArr(), DataSettings.DataFormatOnCPU, DataSettings.DataTypeOnCPU);
}
void TextureClass::UpdateDataInTexture() {
	Assert_NotDeleted_Macro;
	
	if (DataFormatOnGPU_WasUpdated)
		Texture.SetData(Size, TexData.gArr(), DataSettings);
	else
		Texture.SetSubData(Vector3U(0u), Size, TexData.gArr(), DataSettings.DataFormatOnCPU, DataSettings.DataTypeOnCPU);
}

AnonDynArr& TextureClass::GetData() {
	Assert_NotDeleted_Macro;
	return TexData;
}
void TextureClass::StartStoringData() {
	Assert_NotDeleted_Macro;
	IsStoringTexData = true;
}
void TextureClass::StopStoringData() {
	Assert_NotDeleted_Macro;
	TexData.DeleteData();
	IsStoringTexData = false;
}
Vector3U TextureClass::gSize() { 
	Assert_NotDeleted_Macro;
	return Size; 
}

void TextureClass::sSettings_WrapTypeByX(SettingsStruct::WrapTypeEnum wrapTypeByX) { Assert_NotDeleted_Macro; Texture.sSettings_WrapTypeByX(wrapTypeByX); }
TextureClass::SettingsStruct::WrapTypeEnum TextureClass::gSettings_WrapTypeByX() { Assert_NotDeleted_Macro; return Settings.WrapTypeByX; }
void TextureClass::sSettings_WrapTypeByY(SettingsStruct::WrapTypeEnum wrapTypeByY) { Assert_NotDeleted_Macro; Texture.sSettings_WrapTypeByY(wrapTypeByY); }
TextureClass::SettingsStruct::WrapTypeEnum TextureClass::gSettings_WrapTypeByY() { Assert_NotDeleted_Macro; return Settings.WrapTypeByY; }
void TextureClass::sSettings_DownscalingFilt(SettingsStruct::DownscalingFilterFuncEnum downscalingFilt) { Assert_NotDeleted_Macro; Texture.sSettings_DownscalingFilt(downscalingFilt); }
TextureClass::SettingsStruct::DownscalingFilterFuncEnum TextureClass::gSettings_DownscalingFilt() { Assert_NotDeleted_Macro; return Settings.DownscalingFilt; }
void TextureClass::sSettings_UpscalingFilt(SettingsStruct::UpscalingFilterFuncEnum upscalingFilt) { Assert_NotDeleted_Macro; Texture.sSettings_UpscalingFilt(upscalingFilt); }
TextureClass::SettingsStruct::UpscalingFilterFuncEnum TextureClass::gSettings_UpscalingFilt() { Assert_NotDeleted_Macro; return Settings.UpscalingFilt; }
void TextureClass::sSettings_DepthStencilReadMode(SettingsStruct::DepthStencilReadModeEnum depthStencilReadMode) { Assert_NotDeleted_Macro; Texture.sSettings_DepthStencilReadMode(depthStencilReadMode); }
TextureClass::SettingsStruct::DepthStencilReadModeEnum TextureClass::gSettings_DepthStencilReadMode() { Assert_NotDeleted_Macro; return Settings.DepthStencilReadMode; }

void TextureClass::sDataSettings_DataFormatOnGPU(DataSettingsStruct::DataFormatOnGPU_Enum dataFormat) { Assert_NotDeleted_Macro; DataSettings.DataFormatOnGPU = dataFormat; DataFormatOnGPU_WasUpdated = true; }
TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum TextureClass::gDataSettings_DataFormatOnGPU() { Assert_NotDeleted_Macro; return DataSettings.DataFormatOnGPU; DataFormatOnGPU_WasUpdated = true; }
void TextureClass::sDataSettings_DataFormatOnCPU(DataSettingsStruct::DataFormatOnCPU_Enum dataFormat) { Assert_NotDeleted_Macro; DataSettings.DataFormatOnCPU = dataFormat; }
TextureClass::DataSettingsStruct::DataFormatOnCPU_Enum TextureClass::gDataSettings_DataFormatOnCPU() { Assert_NotDeleted_Macro; return DataSettings.DataFormatOnCPU; }
void TextureClass::sDataSettings_DataTypeOnCPU(DataSettingsStruct::DataTypeOnCPU_Enum dataType) { Assert_NotDeleted_Macro; DataSettings.DataTypeOnCPU = dataType; }
TextureClass::DataSettingsStruct::DataTypeOnCPU_Enum TextureClass::gDataSettings_DataTypeOnCPU() { Assert_NotDeleted_Macro; return DataSettings.DataTypeOnCPU; }


void TextureClass::Delete() {
	Assert_NotDeleted_Macro;
	this->~TextureClass();
}
void TextureClass::Bind(unsigned int bindingInd) {
	Assert_NotDeleted_Macro;
	Texture.Bind(bindingInd);
}
void TextureClass::Unbind() {
	Assert_NotDeleted_Macro;
	Texture.Unbind();
}
