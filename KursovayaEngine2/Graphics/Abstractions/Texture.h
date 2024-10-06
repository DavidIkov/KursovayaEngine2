#pragma once
#include"DLL.h"
#include"Tools/DynArr.h"
#include"Graphics/Primitives/Texture.h"

namespace Graphics::Abstractions {

	class TextureClass {
		mutable bool Deleted = false;
		Primitives::TextureClass Texture;
		Vector3U Size;
		void* Data = nullptr;//may be used to copy images, if its set to 0 then copying is disabled but if anything else then enabled
	public:
		typedef Primitives::TextureClass::SettingsStruct SettingsStruct;
		typedef Primitives::TextureClass::DataSettingsStruct DataSettingsStruct;
	private:
		SettingsStruct Settings;
		DataSettingsStruct DataSettings;
	public:
	typedef Primitives::TextureClass::DimensionsEnum DimensionsEnum;
	private:
		DimensionsEnum Dimensions;
	public:
		DLLTREATMENT TextureClass(DimensionsEnum dimensions, bool keepData, const char* filePath, const SettingsStruct& sets, const DataSettingsStruct& dataSets);
		//if keepData is true then you dont need to delete your data, TextureClass will handle this stuff, also data should be on the heap
		DLLTREATMENT TextureClass(DimensionsEnum dimensions, Vector3U pixelsAmount, bool keepData, void* data, const SettingsStruct& sets, const DataSettingsStruct& dataSets);
		DLLTREATMENT TextureClass(RespConstrFlag, const TextureClass& toCopy);
		DLLTREATMENT TextureClass(const TextureClass& toCopy);
		DLLTREATMENT TextureClass(const TextureClass&& toCopy);
		DLLTREATMENT ~TextureClass();
		DLLTREATMENT void operator=(const TextureClass& toCopy);
		DLLTREATMENT void operator=(const TextureClass&& toCopy);
		
		DLLTREATMENT void ChangeData(bool keepData, const void* data);
		DLLTREATMENT void ChangeSubData(Vector3U offset, Vector3U size, const void* data);

		DLLTREATMENT void GetData(void* data);

		DLLTREATMENT void sSettings_WrapTypeByX(SettingsStruct::WrapTypeEnum wrapTypeByX);
        DLLTREATMENT void sSettings_WrapTypeByY(SettingsStruct::WrapTypeEnum wrapTypeByY);
        DLLTREATMENT void sSettings_DownscalingFilt(SettingsStruct::DownscalingFilterFuncEnum downscalingFilt);
        DLLTREATMENT void sSettings_UpscalingFilt(SettingsStruct::UpscalingFilterFuncEnum upscalingFilt);
        DLLTREATMENT void sSettings_DepthStencilReadMode(SettingsStruct::DepthStencilReadModeEnum depthStencilReadMode);

        DLLTREATMENT void Delete();
        DLLTREATMENT void Bind(unsigned int textureInd = 0);
        DLLTREATMENT void Unbind();

	};
}



