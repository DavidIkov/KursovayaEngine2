#include"Texture.h"
#include"Tools/DebugRuntimeAssert.h"
#include<cstring>
#include<utility>

using namespace Graphics::Abstractions;
namespace GP = Graphics::Primitives;
#define Assert_NotDeleted_Macro DebugRuntimeAssert(DebuggingTools::ErrorTypes::Critical, not Deleted, "TextureClass is deleted", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION);

TextureClass::TextureClass(DimensionsEnum dimensions, bool storeData, const char* filePath, const SettingsStruct& sets, const DataSettingsStruct& dataSets) :
	Dimensions(dimensions), Texture(dimensions, filePath, &Size, storeData ? &Data : nullptr, storeData ? &DataSizeInBits : nullptr, sets, dataSets), 
	StoringData(storeData), Settings(sets), DataSettings(dataSets) {}
TextureClass::TextureClass(DimensionsEnum dimensions, Vector3U pixelsAmount, bool storeData, void* data, unsigned int dataSizeInBits, const SettingsStruct& sets, const DataSettingsStruct& dataSets) :
	Dimensions(dimensions), Size(pixelsAmount), Texture(dimensions, pixelsAmount, data, sets, dataSets), Settings(sets), DataSettings(dataSets), 
	Data(storeData ? data : nullptr), DataSizeInBits(storeData ? dataSizeInBits : 0), StoringData(storeData) {}

TextureClass::TextureClass(const TextureClass& toCopy) :
	Size(toCopy.Size), DataSizeInBits(toCopy.DataSizeInBits), StoringData(toCopy.StoringData), Settings(toCopy.Settings), DataSettings(toCopy.DataSettings), 
	Dimensions(toCopy.Dimensions), Texture(Dimensions, Size, toCopy.Data, Settings, DataSettings) {
	if (toCopy.Data != nullptr) {
		Data = new unsigned char[DataSizeInBits];
		memcpy(&Data, toCopy.Data, DataSizeInBits);
	}
}
TextureClass::TextureClass(const TextureClass&& toCopy):
	Texture(std::move(toCopy.Texture)), Size(toCopy.Size), Data(toCopy.Data), DataSizeInBits(toCopy.DataSizeInBits), StoringData(toCopy.StoringData), 
	Settings(toCopy.Settings), DataSettings(toCopy.DataSettings), Dimensions(toCopy.Dimensions) {
	toCopy.Deleted = true;
}
TextureClass::~TextureClass() {
	if (not Deleted) {
		Deleted = true;
		if (Data != nullptr) {
			delete[] Data;
			Data = nullptr;
		}
	}
}
void TextureClass::operator=(const TextureClass& toCopy) {
	Delete();
	Deleted = false;
	
	Size = toCopy.Size;
	DataSizeInBits = toCopy.DataSizeInBits;
	StoringData = toCopy.StoringData;
	Settings = toCopy.Settings;
	DataSettings = toCopy.DataSettings;
	Dimensions = toCopy.Dimensions;
	
	if (toCopy.Data != nullptr) {
		Data = new unsigned char[DataSizeInBits];
		memcpy(&Data, toCopy.Data, DataSizeInBits);
	}
	Texture = GP::TextureClass(Dimensions, Size, Data, Settings, DataSettings);
}
void TextureClass::operator=(const TextureClass&& toCopy) {
	Delete();
	Deleted = false;
	toCopy.Deleted = true;
	
	Size = toCopy.Size;
	Data = toCopy.Data;
	DataSizeInBits = toCopy.DataSizeInBits;
	StoringData = toCopy.StoringData;
	Settings = toCopy.Settings;
	DataSettings = toCopy.DataSettings;
	Dimensions = toCopy.Dimensions;
	Texture = GP::TextureClass(std::move(toCopy.Texture));
}
void TextureClass::ChangeData(Vector3U size, void* data, unsigned int dataSizeInBits) {
	Assert_NotDeleted_Macro;

	if (not DataFormatOnGPU_WasUpdated and size == Size)
		Texture.SetSubData(Vector3U(0u), size, data, DataSettings.DataFormatOnCPU, DataSettings.DataTypeOnCPU);
	else Texture.SetData(size, data, DataSettings);

	if (StoringData) {
		if (Data != nullptr) delete[] Data;
		Data = data;
		DataSizeInBits = dataSizeInBits;
	}

}
void TextureClass::ChangeSubData(Vector3U offset, Vector3U size, const void* data) {
	Assert_NotDeleted_Macro;

	if (offset == Vector3U(0u) and size == Size) {
		memcpy(Data, data, DataSizeInBits);
	}
	else {
		switch (Dimensions) {
		case DimensionsEnum::One: {
			unsigned int pixelBitSize = DataSizeInBits / Size[0];
			memcpy((unsigned char*)Data + offset[0] * pixelBitSize, data, size[0] * pixelBitSize);
			break;
		}
		case DimensionsEnum::Two: {
			unsigned int pixelBitSize = DataSizeInBits / (Size[0] * Size[1]);
			for (unsigned int y = 0; y < size[1]; y++)
				memcpy((unsigned char*)Data + pixelBitSize * (Size[0] * (y + offset[1]) + offset[0]), (unsigned char*)data + size[0] * y * pixelBitSize, size[0] * pixelBitSize);
			break;
		}
		case DimensionsEnum::Three: {
			unsigned int pixelBitSize = DataSizeInBits / (Size[0] * Size[1] * Size[2]);
			for (unsigned int y = 0; y < size[1]; y++) for (unsigned int z = 0; z < size[2]; z++)
				memcpy((unsigned char*)Data + pixelBitSize * (offset[0] + Size[0] * (y + offset[1]) + Size[0] * Size[1] * (z + offset[2])),
					(unsigned char*)data + pixelBitSize * (y * size[0] + z * size[0] * size[1]), size[0] * pixelBitSize);
		}
		}
	}
	if (DataFormatOnGPU_WasUpdated)
		Texture.SetSubData(offset, size, Data, DataSettings.DataFormatOnCPU, DataSettings.DataTypeOnCPU);
	else
		Texture.SetData(Size, Data, DataSettings);
}

void TextureClass::GetData(void** writeDataPtr, unsigned int* writeDataSizeInBitsPtr) {
	Assert_NotDeleted_Macro;
	if (writeDataPtr != nullptr) *writeDataPtr = Data;
	if (writeDataSizeInBitsPtr != nullptr) *writeDataSizeInBitsPtr = DataSizeInBits;
}
void TextureClass::StartStoringData() {
	Assert_NotDeleted_Macro;
	StoringData = true;
}
void TextureClass::StopStoringData() {
	Assert_NotDeleted_Macro;
	if (Data != nullptr) {
		delete[] Data;
		Data = nullptr;
	}
	StoringData = false;
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

void TextureClass::sDataSettings_DataFormatOnGPU(DataSettingsStruct::DataFormatOnGPU_Enum dataFormat) { Assert_NotDeleted_Macro; DataSettings.DataFormatOnGPU = dataFormat; }
TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum TextureClass::gDataSettings_DataFormatOnGPU() { Assert_NotDeleted_Macro; return DataSettings.DataFormatOnGPU; }
void TextureClass::sDataSettings_DataFormatOnCPU(DataSettingsStruct::DataFormatOnCPU_Enum dataFormat) { Assert_NotDeleted_Macro; DataSettings.DataFormatOnCPU = dataFormat; }
TextureClass::DataSettingsStruct::DataFormatOnCPU_Enum TextureClass::gDataSettings_DataFormatOnCPU() { Assert_NotDeleted_Macro; return DataSettings.DataFormatOnCPU; }
void TextureClass::sDataSettings_DataTypeOnCPU(DataSettingsStruct::DataTypeOnCPU_Enum dataType) { Assert_NotDeleted_Macro; DataSettings.DataTypeOnCPU = dataType; DataFormatOnGPU_WasUpdated = true; }
TextureClass::DataSettingsStruct::DataTypeOnCPU_Enum TextureClass::gDataSettings_DataTypeOnCPU() { Assert_NotDeleted_Macro; return DataSettings.DataTypeOnCPU; DataFormatOnGPU_WasUpdated = true; }


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
