#pragma once
#include"DLL.h"
#include"Maths/Vector.h"
#include"Tools/ErrorsSystem.h"
#include"Tools/UsefullMacros/ComparisonByBytes.h"
#include"Tools/ArrayView.h"

namespace KE2::Graphics::Primitives {
    class TextureClass {
    public:
		struct SettingsStruct {
			enum class WrapTypeEnum :unsigned char {
				ClampToEdge, ClampToBorder, MirroredRepeat, Repeat, MirrorClampToEdge
			};
			enum class DownscalingFilterFuncEnum :unsigned char {
				Nearest, Linear, NearestMipmapNearest, NearestMipmapLinear, LinearMipmapLinear, LinearMipmapNearest
			};
			enum class UpscalingFilterFuncEnum :unsigned char {
				Nearest, Linear
			};
			enum class DepthStencilReadModeEnum :unsigned char {
				Depth, Stencil, 
				/*you cant technically select mode as None but in here its used to just dont mention depth mode texture when they have nothing to do with depth*/
				None
			};

			WrapTypeEnum WrapTypeByX;
			WrapTypeEnum WrapTypeByY;
			DownscalingFilterFuncEnum DownscalingFilt;
			UpscalingFilterFuncEnum UpscalingFilt;
			DepthStencilReadModeEnum DepthStencilReadMode;

			ComparisonByBytesMacro(SettingsStruct,==);
            ComparisonByBytesMacro(SettingsStruct,!=);

		};
		struct DataSettingsStruct {
			enum class DataFormatOnGPU_Enum :unsigned char {
				Depth, DepthStencil, Stencil, Red, RG, RGB, RGBA, None
			};
			enum class DataFormatOnCPU_Enum :unsigned char {
				Red, RG, RGB, BGR, RGBA, BGRA, RedInteger, RG_Integer, RGB_Integer, BGR_Integer, RGBA_Integer, BGRA_Integer, Stencil,
				Depth, DepthStencil, None
			};
			enum class DataTypeOnCPU_Enum :unsigned char {
				UnsignedByte, Byte, UnsignedShort, Short, UnsignedInt, Int, Float, UnsignedInt_24_8, None
			};

			DataFormatOnGPU_Enum DataFormatOnGPU = DataFormatOnGPU_Enum::None;
			DataFormatOnCPU_Enum DataFormatOnCPU = DataFormatOnCPU_Enum::None;
			DataTypeOnCPU_Enum DataTypeOnCPU = DataTypeOnCPU_Enum::None;

            ComparisonByBytesMacro(DataSettingsStruct,==);
            ComparisonByBytesMacro(DataSettingsStruct,!=);
		};

        enum class DimensionsEnum :unsigned char {
            One, Two, Three
        };
    protected:

		static unsigned int _DataFormatOnGPU_SwitchCase(DataSettingsStruct::DataFormatOnGPU_Enum format) noexcept;
		static unsigned int _DataFormatOnCPU_SwitchCase(DataSettingsStruct::DataFormatOnCPU_Enum format) noexcept;
		static unsigned int _DataTypeOnCPU_SwitchCase(DataSettingsStruct::DataTypeOnCPU_Enum type) noexcept;
		static unsigned int _DataTypeOnCPU_Sizeof_SwitchCase(DataSettingsStruct::DataTypeOnCPU_Enum type) noexcept;
		static unsigned int _WrapType_SwitchCase(SettingsStruct::WrapTypeEnum wrapTyp) noexcept;
		static unsigned int _DownscalingFilterFunc_SwitchCase(SettingsStruct::DownscalingFilterFuncEnum filt) noexcept;
		static unsigned int _UpscalingFilterFunc_SwitchCase(SettingsStruct::UpscalingFilterFuncEnum filt) noexcept;
		static unsigned int _DepthStencilReadMode_SwitchCase(SettingsStruct::DepthStencilReadModeEnum readMode) noexcept;

        static unsigned int _GL_TextureEnum_SwitchCase(TextureClass::DimensionsEnum dim) noexcept;

        unsigned int ID = 0u;

        const DimensionsEnum Dimensions;
        const unsigned int GL_TexEnum;

        void _UpdSettings_WrapTypeByX(SettingsStruct::WrapTypeEnum wrapTyp) const;
        void _UpdSettings_WrapTypeByY(SettingsStruct::WrapTypeEnum wrapTyp) const;
        void _UpdSettings_DownscalingFilt(SettingsStruct::DownscalingFilterFuncEnum filt) const;
        void _UpdSettings_UpscalingFilt(SettingsStruct::UpscalingFilterFuncEnum filt) const;
        void _UpdSettings_DepthStencilReadMode(SettingsStruct::DepthStencilReadModeEnum readMode) const;

        void _UpdateSettings(const SettingsStruct& sets) const;

        void _AllocatePixels(Vector3U pixelsAmount, unsigned int mipmapLevels, DataSettingsStruct::DataFormatOnGPU_Enum dataFormatOnGPU);
    public:

        struct ErrorsEnumWrapperStruct :KE2::ErrorsSystemNamespace::ErrorBase {
            enum ErrorsEnum {
                STB_IMAGE_Failed,
            };
            ErrorsEnum Error;
            inline ErrorsEnumWrapperStruct(ErrorsEnum error) :Error(error) {};
        }; using ErrorsEnum = ErrorsEnumWrapperStruct; using AnyError = ErrorsEnumWrapperStruct;

        DLLTREATMENT TextureClass(DimensionsEnum dimensions, const char* filePath, Vector3U* writeSizePtr, ArrayView<void>* writeArrayView, unsigned int mipmapLevels, const SettingsStruct& sets, const DataSettingsStruct& dataSets);
        DLLTREATMENT TextureClass(DimensionsEnum dimensions, Vector3U pixelsAmount, const void* data, unsigned int mipmapLevels, const SettingsStruct& sets, const DataSettingsStruct& dataSets);
        DLLTREATMENT TextureClass(DimensionsEnum dimensions, Vector3U pixelsAmount, unsigned int mipmapLevels, DataSettingsStruct::DataFormatOnGPU_Enum dataFormatOnGPU, const SettingsStruct& sets);
        DLLTREATMENT TextureClass(TextureClass&& toCopy) noexcept;
        DLLTREATMENT virtual TextureClass& operator=(TextureClass&& toCopy);
        DLLTREATMENT virtual ~TextureClass() noexcept(false);

        DLLTREATMENT virtual void SetSubData(Vector3U pixelsOffset, Vector3U pixelsAmount, const void* data,
            DataSettingsStruct::DataFormatOnCPU_Enum dataFormat, DataSettingsStruct::DataTypeOnCPU_Enum dataType);

        //if you dont use some axes in pixelsAmounts then dont leave them 0, use 1
        //make sure that your texture have enough pixels for copying
        DLLTREATMENT virtual void CopySubData(const TextureClass& srcTex, Vector3U offsetInSource, Vector3U offsetInDestination, Vector3U pixelsAmount);

		//this function is slow since it will get data from gpu to cpu
        //buffer should not be nullptr, it should point to already allocated memory
        DLLTREATMENT virtual void GetData(void* buffer, DataSettingsStruct::DataFormatOnCPU_Enum dataFormat, DataSettingsStruct::DataTypeOnCPU_Enum dataType) const;
        //this function is slow since it will get data from gpu to cpu
        //buffer should not be nullptr, it should point to already allocated memory
		DLLTREATMENT virtual void GetSubData(Vector3U offset, void* buffer, Vector3U pixelsAmount, 
            DataSettingsStruct::DataFormatOnCPU_Enum dataFormat, DataSettingsStruct::DataTypeOnCPU_Enum dataType) const;

        DLLTREATMENT virtual void SetSettings_WrapTypeByX(SettingsStruct::WrapTypeEnum wrapTypeByX);
        DLLTREATMENT virtual void SetSettings_WrapTypeByY(SettingsStruct::WrapTypeEnum wrapTypeByY);
        DLLTREATMENT virtual void SetSettings_DownscalingFilt(SettingsStruct::DownscalingFilterFuncEnum downscalingFilt);
        DLLTREATMENT virtual void SetSettings_UpscalingFilt(SettingsStruct::UpscalingFilterFuncEnum upscalingFilt);
        DLLTREATMENT virtual void SetSettings_DepthStencilReadMode(SettingsStruct::DepthStencilReadModeEnum depthStencilReadMode);

		typedef unsigned int TextureID_Type;
		inline TextureID_Type gID() const noexcept { return ID; }
		inline operator TextureID_Type() const noexcept { return ID; }

        DLLTREATMENT void Bind(unsigned int bindingInd) const;
        //will not set active texture, so it will replace texture in last binded slot(by opengl rules)
        DLLTREATMENT void Bind() const;
        DLLTREATMENT void Unbind() const;

        
    };
}
