#pragma once
#include"DLL.h"
#include"Tools/DynArr.h"
#include"Graphics/Primitives/Texture.h"

namespace Graphics::Abstractions {

	class TextureClass {
		mutable bool Deleted = false;
		Primitives::TextureClass Texture;
		Vector3U Size;
		void* Data = nullptr;//may be used to copy images
		unsigned int DataSizeInBits = 0;
		bool StoringData = false;
	public:
		typedef Primitives::TextureClass::SettingsStruct SettingsStruct;
		typedef Primitives::TextureClass::DataSettingsStruct DataSettingsStruct;
	private:
		SettingsStruct Settings;
		DataSettingsStruct DataSettings;
		bool DataFormatOnGPU_WasUpdated = false;
	public:
	typedef Primitives::TextureClass::DimensionsEnum DimensionsEnum;
	private:
		DimensionsEnum Dimensions;
	public:
		DLLTREATMENT TextureClass(DimensionsEnum dimensions, bool storeData, const char* filePath, const SettingsStruct& sets, const DataSettingsStruct& dataSets);
		//if storeData is true then you dont need to delete your data, TextureClass will handle this stuff, also data should be on the heap
		DLLTREATMENT TextureClass(DimensionsEnum dimensions, Vector3U pixelsAmount, bool storeData, void* data, unsigned int dataSizeInBits, const SettingsStruct& sets, const DataSettingsStruct& dataSets);
		DLLTREATMENT TextureClass(RespConstrFlag, const TextureClass& toCopy);
		//if Data is nullptr then it will not copy texture data but copy everything else
		DLLTREATMENT TextureClass(const TextureClass& toCopy);
		DLLTREATMENT TextureClass(const TextureClass&& toCopy);
		DLLTREATMENT ~TextureClass();
		DLLTREATMENT void operator=(const TextureClass& toCopy);
		DLLTREATMENT void operator=(const TextureClass&& toCopy);
		
		//if texture is not storing data then dataSizeInBits can be kept zero, otherwise it cant
		DLLTREATMENT void ChangeData(Vector3U size, void* data, unsigned int dataSizeInBits);
		DLLTREATMENT void ChangeSubData(Vector3U offset, Vector3U size, const void* data);

		DLLTREATMENT void GetData(void** writeDataPtr, unsigned int* writeDataSizeInBitsPtr);

		DLLTREATMENT void StartStoringData();
		//will free currently stored data if there is any
		DLLTREATMENT void StopStoringData();

		DLLTREATMENT Vector3U gSize();

		DLLTREATMENT void sSettings_WrapTypeByX(SettingsStruct::WrapTypeEnum wrapTypeByX);
        DLLTREATMENT void sSettings_WrapTypeByY(SettingsStruct::WrapTypeEnum wrapTypeByY);
        DLLTREATMENT void sSettings_DownscalingFilt(SettingsStruct::DownscalingFilterFuncEnum downscalingFilt);
        DLLTREATMENT void sSettings_UpscalingFilt(SettingsStruct::UpscalingFilterFuncEnum upscalingFilt);
        DLLTREATMENT void sSettings_DepthStencilReadMode(SettingsStruct::DepthStencilReadModeEnum depthStencilReadMode);

		//changes will happen when data will
		DLLTREATMENT void sDataSettings_DataFormatOnGPU(DataSettingsStruct::DataFormatOnGPU_Enum dataFormat);
		DLLTREATMENT void sDataSettings_DataFormatOnCPU(DataSettingsStruct::DataFormatOnCPU_Enum dataFormat);
		DLLTREATMENT void sDataSettings_DataTypeOnCPU(DataSettingsStruct::DataTypeOnCPU_Enum dataType);

        DLLTREATMENT void Delete();
        DLLTREATMENT void Bind(unsigned int bindingInd = 0);
        DLLTREATMENT void Unbind();

	};
}



