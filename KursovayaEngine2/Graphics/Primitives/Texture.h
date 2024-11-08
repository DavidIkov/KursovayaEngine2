#pragma once
#include"DLL.h"
#include"Maths/Vector.h"
#include"Tools/ClassFunctionsAccessController.h"
#include"Tools/AnonDynArr.h"
#include"Tools/ErrorsSystem.h"

namespace Graphics::Primitives {
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
				/*you cant technically select mode as None but in here but its used to just dont mention depth mode texture when they have nothing to do with depth*/
				None
			};

			WrapTypeEnum WrapTypeByX;
			WrapTypeEnum WrapTypeByY;
			DownscalingFilterFuncEnum DownscalingFilt;
			UpscalingFilterFuncEnum UpscalingFilt;
			DepthStencilReadModeEnum DepthStencilReadMode;
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
		};

        enum class DimensionsEnum :unsigned char {
            One, Two, Three
        };
    protected:

        //they are static just to have ability to call then from inherited classes in future

		static unsigned int _DataFormatOnGPU_SwitchCase(DataSettingsStruct::DataFormatOnGPU_Enum format);
		static unsigned int _DataFormatOnCPU_SwitchCase(DataSettingsStruct::DataFormatOnCPU_Enum format);
		static unsigned int _DataTypeOnCPU_SwitchCase(DataSettingsStruct::DataTypeOnCPU_Enum type);
		static unsigned int _DataTypeOnCPU_Sizeof_SwitchCase(DataSettingsStruct::DataTypeOnCPU_Enum type);
		static unsigned int _WrapType_SwitchCase(SettingsStruct::WrapTypeEnum wrapTyp);
		static unsigned int _DownscalingFilterFunc_SwitchCase(SettingsStruct::DownscalingFilterFuncEnum filt);
		static unsigned int _UpscalingFilterFunc_SwitchCase(SettingsStruct::UpscalingFilterFuncEnum filt);
		static unsigned int _DepthStencilReadMode_SwitchCase(SettingsStruct::DepthStencilReadModeEnum readMode);

        unsigned int GL_TexEnum;
        unsigned int ID;
        DimensionsEnum Dimensions;
        mutable bool Deleted = false;

        void _Constructor(Vector3U pixelsAmount, const void* data, const DataSettingsStruct& dataSets);

        void _UpdSettings_WrapTypeByX(SettingsStruct::WrapTypeEnum wrapTyp);
        void _UpdSettings_WrapTypeByY(SettingsStruct::WrapTypeEnum wrapTyp);
        void _UpdSettings_DownscalingFilt(SettingsStruct::DownscalingFilterFuncEnum filt);
        void _UpdSettings_UpscalingFilt(SettingsStruct::UpscalingFilterFuncEnum filt);
        void _UpdSettings_DepthStencilReadMode(SettingsStruct::DepthStencilReadModeEnum readMode);

        void _UpdateSettings(const SettingsStruct& sets);
    public:

        struct ErrorsEnumWrapperStruct :KE2::ErrorsSystemNamespace::ErrorBase {
            enum ErrorsEnum {
                AlreadyDeleted,
                STB_IMAGE_Failed,
            };
            ErrorsEnum Error;
            inline ErrorsEnumWrapperStruct(ErrorsEnum error) :Error(error) {};
        }; using ErrorsEnum = ErrorsEnumWrapperStruct; using AnyError = ErrorsEnumWrapperStruct;

        DLLTREATMENT TextureClass(DimensionsEnum dimensions, const char* filePath, Vector3U* writeSizePtr, AnonDynArr* writeAnonDynArr, const SettingsStruct& sets, const DataSettingsStruct& dataSets);
        DLLTREATMENT TextureClass(DimensionsEnum dimensions, Vector3U pixelsAmount, const void* data, const SettingsStruct& sets, const DataSettingsStruct& dataSets);
        DLLTREATMENT TextureClass(const TextureClass&& toCopy);
        DLLTREATMENT void operator=(const TextureClass&& toCopy);
        DLLTREATMENT ~TextureClass();

        DLLTREATMENT void SetData(Vector3U pixelsAmount, const void* data, const DataSettingsStruct& dataSets);
        DLLTREATMENT void SetSubData(Vector3U pixelsOffset, Vector3U pixelsAmount, const void* data,
            DataSettingsStruct::DataFormatOnCPU_Enum dataFormatOnCPU, DataSettingsStruct::DataTypeOnCPU_Enum dataTypeOnCPU);

        DLLTREATMENT void GenerateMipmaps();

        //if you dont use some axes in pixelsAmounts then dont leave them 0, use 1
        //make sure that your texture have enough pixels for copying
        DLLTREATMENT void CopySubData(const TextureClass& srcTex, Vector3U offsetInSource, Vector3U offsetInDestination, Vector3U pixelsAmount);

		//this function is slow since it will get data from gpu to cpu
        //buffer should not be nullptr, it should point to already allocated memory
        DLLTREATMENT void GetData(void* buffer, DataSettingsStruct::DataFormatOnCPU_Enum dataFormat, DataSettingsStruct::DataTypeOnCPU_Enum dataType) const;
        //this function is slow since it will get data from gpu to cpu
        //buffer should not be nullptr, it should point to already allocated memory
		DLLTREATMENT void GetSubData(Vector3U offset, void* buffer, Vector3U pixelsAmount, DataSettingsStruct::DataFormatOnCPU_Enum dataFormat, DataSettingsStruct::DataTypeOnCPU_Enum dataType) const;

        DLLTREATMENT void sSettings_WrapTypeByX(SettingsStruct::WrapTypeEnum wrapTypeByX);
        DLLTREATMENT void sSettings_WrapTypeByY(SettingsStruct::WrapTypeEnum wrapTypeByY);
        DLLTREATMENT void sSettings_DownscalingFilt(SettingsStruct::DownscalingFilterFuncEnum downscalingFilt);
        DLLTREATMENT void sSettings_UpscalingFilt(SettingsStruct::UpscalingFilterFuncEnum upscalingFilt);
        DLLTREATMENT void sSettings_DepthStencilReadMode(SettingsStruct::DepthStencilReadModeEnum depthStencilReadMode);

        DLLTREATMENT unsigned int gID();
        DLLTREATMENT void Delete();
        DLLTREATMENT void Bind(unsigned int textureInd = 0) const;
        DLLTREATMENT void Unbind();

#define CFAC_ClassName TextureClass
        CFAC_ClassConstructor(FullAccess,
            CFAC_FuncConstr(SetData)
            CFAC_FuncConstr(SetSubData)
            CFAC_FuncConstr(GenerateMipmaps)
            CFAC_FuncConstr(GetData)
            CFAC_FuncConstr(sSettings_WrapTypeByX)
            CFAC_FuncConstr(sSettings_WrapTypeByY)
            CFAC_FuncConstr(sSettings_DownscalingFilt)
            CFAC_FuncConstr(sSettings_UpscalingFilt)
            CFAC_FuncConstr(sSettings_DepthStencilReadMode)
            CFAC_FuncConstr(gID)
            CFAC_FuncConstr(Bind)
            CFAC_FuncConstr(Unbind)
        );
#undef CFAC_ClassName

        
    };
}
