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
				DepthComponent, DepthStencil, Red, RG, RGB, RGBA
			};
			enum class DataFormatOnCPU_Enum :unsigned char {
				Red, RG, RGB, BGR, RGBA, BGRA, RedInteger, RG_Integer, RGB_Integer, BGR_Integer, RGBA_Integer, BGRA_Integer, StencilIndex,
				DepthComponent, DepthStencil
			};
			enum class DataTypeOnCPU_Enum :unsigned char {
				UnsignedByte, Byte, UnsignedShort, Short, UnsignedInt, Int, Float, UnsignedInt_24_8
			};

			DataFormatOnGPU_Enum DataFormatOnGPU;
			DataFormatOnCPU_Enum DataFormatOnCPU;
			DataTypeOnCPU_Enum DataTypeOnCPU;

            ComparisonByBytesMacro(DataSettingsStruct,==);
            ComparisonByBytesMacro(DataSettingsStruct,!=);
		};

        enum class DimensionsEnum :unsigned char {
            One, Two, Three
        };
    protected:

        //they are static just to have ability to call then from inherited classes in future

		static unsigned int _DataFormatOnGPU_SwitchCase(DataSettingsStruct::DataFormatOnGPU_Enum format) noexcept;
		static unsigned int _DataFormatOnCPU_SwitchCase(DataSettingsStruct::DataFormatOnCPU_Enum format) noexcept;
		static unsigned int _DataTypeOnCPU_SwitchCase(DataSettingsStruct::DataTypeOnCPU_Enum type) noexcept;
		static unsigned int _DataTypeOnCPU_Sizeof_SwitchCase(DataSettingsStruct::DataTypeOnCPU_Enum type) noexcept;
		static unsigned int _WrapType_SwitchCase(SettingsStruct::WrapTypeEnum wrapTyp) noexcept;
		static unsigned int _DownscalingFilterFunc_SwitchCase(SettingsStruct::DownscalingFilterFuncEnum filt) noexcept;
		static unsigned int _UpscalingFilterFunc_SwitchCase(SettingsStruct::UpscalingFilterFuncEnum filt) noexcept;
		static unsigned int _DepthStencilReadMode_SwitchCase(SettingsStruct::DepthStencilReadModeEnum readMode) noexcept;

        unsigned int ID = 0u;

        const DimensionsEnum Dimensions;
        const unsigned int GL_TexEnum;

        void _Constructor(Vector3U pixelsAmount, const void* data, const DataSettingsStruct& dataSets);

        void _UpdSettings_WrapTypeByX(SettingsStruct::WrapTypeEnum wrapTyp) const;
        void _UpdSettings_WrapTypeByY(SettingsStruct::WrapTypeEnum wrapTyp) const;
        void _UpdSettings_DownscalingFilt(SettingsStruct::DownscalingFilterFuncEnum filt) const;
        void _UpdSettings_UpscalingFilt(SettingsStruct::UpscalingFilterFuncEnum filt) const;
        void _UpdSettings_DepthStencilReadMode(SettingsStruct::DepthStencilReadModeEnum readMode) const;

        void _UpdateSettings(const SettingsStruct& sets) const;
    public:

        struct ErrorsEnumWrapperStruct :KE2::ErrorsSystemNamespace::ErrorBase {
            enum ErrorsEnum {
                STB_IMAGE_Failed,
            };
            ErrorsEnum Error;
            inline ErrorsEnumWrapperStruct(ErrorsEnum error) :Error(error) {};
        }; using ErrorsEnum = ErrorsEnumWrapperStruct; using AnyError = ErrorsEnumWrapperStruct;

        DLLTREATMENT TextureClass(DimensionsEnum dimensions, const char* filePath, Vector3U* writeSizePtr, ArrayView<void>* writeArrayView, const SettingsStruct& sets, const DataSettingsStruct& dataSets);
        DLLTREATMENT TextureClass(DimensionsEnum dimensions, Vector3U pixelsAmount, const void* data, const SettingsStruct& sets, const DataSettingsStruct& dataSets);
        DLLTREATMENT TextureClass(TextureClass&& toCopy) noexcept;
        DLLTREATMENT virtual TextureClass& operator=(TextureClass&& toCopy);
        DLLTREATMENT virtual ~TextureClass() noexcept(false);

        DLLTREATMENT virtual void SetData(Vector3U pixelsAmount, const void* data, const DataSettingsStruct& dataSets);
        DLLTREATMENT virtual void SetSubData(Vector3U pixelsOffset, Vector3U pixelsAmount, const void* data,
            DataSettingsStruct::DataFormatOnCPU_Enum dataFormatOnCPU, DataSettingsStruct::DataTypeOnCPU_Enum dataTypeOnCPU);

        DLLTREATMENT virtual void GenerateMipmaps();

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

        DLLTREATMENT virtual void sSettings_WrapTypeByX(SettingsStruct::WrapTypeEnum wrapTypeByX);
        DLLTREATMENT virtual void sSettings_WrapTypeByY(SettingsStruct::WrapTypeEnum wrapTypeByY);
        DLLTREATMENT virtual void sSettings_DownscalingFilt(SettingsStruct::DownscalingFilterFuncEnum downscalingFilt);
        DLLTREATMENT virtual void sSettings_UpscalingFilt(SettingsStruct::UpscalingFilterFuncEnum upscalingFilt);
        DLLTREATMENT virtual void sSettings_DepthStencilReadMode(SettingsStruct::DepthStencilReadModeEnum depthStencilReadMode);

		typedef unsigned int TextureID_Type;
		inline TextureID_Type gID() const noexcept { return ID; }
		inline operator TextureID_Type() const noexcept { return ID; }

        DLLTREATMENT void Bind(unsigned int bindingInd) const;
        //will not set active texture, so it will replace texture in last binded slot(by opengl rules)
        DLLTREATMENT void Bind() const;
        DLLTREATMENT void Unbind() const;

        
    };
}
