#pragma once
#include"DLL.h"
#include"Maths/Vector.h"
#include"Tools/DynArr.h"
#include"Tools/ClassFunctionsAccessController.h"

namespace GraphicsPrimitives {

    struct TextureSettingsClass {
        enum class WrapTypeEnum :unsigned short int {
            ClampToEdge, ClampToBorder, MirroredRepeat, Repeat, MirrorClampToEdge
        };
        enum class DownscalingFilterFuncEnum :unsigned short int {
            Nearest, Linear, NearestMipmapNearest, NearestMipmapLinear, LinearMipmapLinear, LinearMipmapNearest
        };
        enum class UpscalingFilterFuncEnum :unsigned short int {
            Nearest, Linear
        };
        enum class DepthStencilReadModeEnum :unsigned short int {
            Depth, Stencil
        };

        WrapTypeEnum WrapTypeByX;
        WrapTypeEnum WrapTypeByY;
        DownscalingFilterFuncEnum DownscalingFilt;
        UpscalingFilterFuncEnum UpscalingFilt;
        DepthStencilReadModeEnum DepthStencilReadMode;
    };
    struct TextureDataSettingsClass {
        enum class DataFormatOnGPU_Enum :unsigned short int {
            DepthComponent, DepthStencil, Red, RG, RGB, RGBA
        };
        enum class DataFormatOnCPU_Enum :unsigned short int {
            Red, RG, RGB, BGR, RGBA, BGRA, RedInteger, RG_Integer, RGB_Integer, BGR_Integer, RGBA_Integer, BGRA_Integer, StencilIndex,
            DepthComponent, DepthStencil
        };
        enum class DataTypeOnCPU_Enum :unsigned short int {
            UnsignedByte, Byte, UnsignedShort, Short, UnsignedInt, Int, Float, UnsignedInt_24_8
        };

        DataFormatOnGPU_Enum DataFormatOnGPU;
        DataFormatOnCPU_Enum DataFormatOnCPU;
        DataTypeOnCPU_Enum DataTypeOnCPU;
    };

    class TextureClass {
        
        unsigned int ID;
        mutable bool Deleted = false;

    public:
        enum class DimensionsEnum :unsigned short int {
            One, Two, Three
        };
    private:
        DimensionsEnum Dimensions;
        unsigned int GL_TexEnum;

        void _Constructor(Vector3U pixelsAmount, const void* data, const TextureDataSettingsClass& dataSets);

        void _UpdSettings_WrapTypeByX(TextureSettingsClass::WrapTypeEnum wrapTyp);
        void _UpdSettings_WrapTypeByY(TextureSettingsClass::WrapTypeEnum wrapTyp);
        void _UpdSettings_DownscalingFilt(TextureSettingsClass::DownscalingFilterFuncEnum filt);
        void _UpdSettings_UpscalingFilt(TextureSettingsClass::UpscalingFilterFuncEnum filt);
        void _UpdSettings_DepthStencilReadMode(TextureSettingsClass::DepthStencilReadModeEnum readMode);

        void _UpdateSettings(const TextureSettingsClass& sets);
    public:

        DLLTREATMENT TextureClass(DimensionsEnum dimensions, const char* filePath, const TextureSettingsClass& sets);
        DLLTREATMENT TextureClass(DimensionsEnum dimensions, Vector3U pixelsAmount, const void* data, const TextureSettingsClass& sets, const TextureDataSettingsClass& dataSets);
        DLLTREATMENT TextureClass(RespConstrFlag, const TextureClass& toCopy);
        DLLTREATMENT TextureClass(const TextureClass&& toCopy);
        DLLTREATMENT void operator=(const TextureClass&& toCopy);
        DLLTREATMENT ~TextureClass();

        DLLTREATMENT void SetData(Vector3U pixelsAmount, const void* data, const TextureDataSettingsClass& dataSets);
        DLLTREATMENT void SetSubData(Vector3U pixelsOffset, Vector3U pixelsAmount, const void* data,
            TextureDataSettingsClass::DataFormatOnCPU_Enum dataFormatOnCPU, TextureDataSettingsClass::DataTypeOnCPU_Enum dataTypeOnCPU);

        DLLTREATMENT void GenerateMipmaps();

		//this function is slow since it will get data from gpu to cpu
        //buffer should be not be nullptr, it should point to already allocated memory
        DLLTREATMENT void GetData(TextureDataSettingsClass::DataFormatOnCPU_Enum dataFormat, TextureDataSettingsClass::DataTypeOnCPU_Enum dataType, void* buffer);

        DLLTREATMENT void sSettings_WrapTypeByX(TextureSettingsClass::WrapTypeEnum wrapTypeByX);
        DLLTREATMENT void sSettings_WrapTypeByY(TextureSettingsClass::WrapTypeEnum wrapTypeByY);
        DLLTREATMENT void sSettings_DownscalingFilt(TextureSettingsClass::DownscalingFilterFuncEnum downscalingFilt);
        DLLTREATMENT void sSettings_UpscalingFilt(TextureSettingsClass::UpscalingFilterFuncEnum upscalingFilt);
        DLLTREATMENT void sSettings_DepthStencilReadMode(TextureSettingsClass::DepthStencilReadModeEnum depthStencilReadMode);

        DLLTREATMENT unsigned int gID();
        DLLTREATMENT void Delete();
        DLLTREATMENT void Bind(unsigned int textureInd = 0);
        DLLTREATMENT void Unbind();

#define CFAC_ClassName TextureClass
        CFAC_ClassConstructor(FullAccess,
            CFAC_FuncPtrConstr(SetData)
            CFAC_FuncPtrConstr(SetSubData)
            CFAC_FuncPtrConstr(GenerateMipmaps)
            CFAC_FuncPtrConstr(GetData)
            CFAC_FuncPtrConstr(sSettings_WrapTypeByX)
            CFAC_FuncPtrConstr(sSettings_WrapTypeByY)
            CFAC_FuncPtrConstr(sSettings_DownscalingFilt)
            CFAC_FuncPtrConstr(sSettings_UpscalingFilt)
            CFAC_FuncPtrConstr(sSettings_DepthStencilReadMode)
            CFAC_FuncPtrConstr(gID)
            CFAC_FuncPtrConstr(Bind)
            CFAC_FuncPtrConstr(Unbind)
        );
#undef CFAC_ClassName

        
    };



    /*class Texture1DClass {

        unsigned int ID;
        mutable bool Deleted = false;


        void _Constructor(unsigned int pixelsAmount, const void* data, const TextureDataSettingsClass& dataSets);

        void _UpdSettings_WrapTypeByX(TextureSettingsClass::WrapTypeEnum wrapTyp);
        void _UpdSettings_WrapTypeByY(TextureSettingsClass::WrapTypeEnum wrapTyp);
        void _UpdSettings_DownscalingFilt(TextureSettingsClass::DownscalingFilterFuncEnum filt);
        void _UpdSettings_UpscalingFilt(TextureSettingsClass::UpscalingFilterFuncEnum filt);
        void _UpdSettings_DepthStencilReadMode(TextureSettingsClass::DepthStencilReadModeEnum readMode);

        void _UpdateSettings(const TextureSettingsClass& sets);
    public:

        //data about data fed into texture
        class CPU_DataData_Struct {

            Vector1U Size;

            mutable bool Empty = true;
            void* Data;

            TextureSettingsClass Settings;
            TextureDataSettingsClass DataSettings;

        public:
            DLLTREATMENT CPU_DataData_Struct(Vector1U size, void* data, TextureSettingsClass settings, TextureDataSettingsClass dataSettings);
            DLLTREATMENT CPU_DataData_Struct();
            DLLTREATMENT CPU_DataData_Struct(const CPU_DataData_Struct& toCopy);
            DLLTREATMENT CPU_DataData_Struct(const CPU_DataData_Struct* toCopy);
            DLLTREATMENT void operator=(const CPU_DataData_Struct& toCopy);
            DLLTREATMENT void Delete();
            DLLTREATMENT ~CPU_DataData_Struct();
        };

        //if dataExportPtr is not nullptr, then data allocated to read image will not be freed
        DLLTREATMENT Texture1DClass(const char* filePath, const TextureSettingsClass& sets, CPU_DataData_Struct* dataExportPtr = nullptr);
        DLLTREATMENT Texture1DClass(unsigned int pixelsAmount, const void* data, const TextureSettingsClass& sets, const TextureDataSettingsClass& dataSets);
        DLLTREATMENT Texture1DClass(RespConstrFlag, const Texture1DClass& toCopy);
        DLLTREATMENT Texture1DClass(const Texture1DClass&& toCopy);
        DLLTREATMENT void operator=(const Texture1DClass&& toCopy);
        DLLTREATMENT ~Texture1DClass();

        DLLTREATMENT void SetData(unsigned int pixelsAmount, const void* data, const TextureDataSettingsClass& dataSets);
        DLLTREATMENT void SetSubData(unsigned int pixelsOffset, unsigned int pixelsAmount, const void* data,
            TextureDataSettingsClass::DataFormatOnCPU_Enum dataFormatOnCPU, TextureDataSettingsClass::DataTypeOnCPU_Enum dataTypeOnCPU);

        DLLTREATMENT void GenerateMipmaps();

        //this function is slow since it will get data from gpu to cpu
        //buffer should be not be nullptr, it should point to already allocated memory
        DLLTREATMENT void GetData(TextureDataSettingsClass::DataFormatOnCPU_Enum dataFormat, TextureDataSettingsClass::DataTypeOnCPU_Enum dataType, void* buffer);

        DLLTREATMENT void sSettings_WrapTypeByX(TextureSettingsClass::WrapTypeEnum wrapTypeByX);
        DLLTREATMENT void sSettings_WrapTypeByY(TextureSettingsClass::WrapTypeEnum wrapTypeByY);
        DLLTREATMENT void sSettings_DownscalingFilt(TextureSettingsClass::DownscalingFilterFuncEnum downscalingFilt);
        DLLTREATMENT void sSettings_UpscalingFilt(TextureSettingsClass::UpscalingFilterFuncEnum upscalingFilt);
        DLLTREATMENT void sSettings_DepthStencilReadMode(TextureSettingsClass::DepthStencilReadModeEnum depthStencilReadMode);

        DLLTREATMENT unsigned int gID();
        DLLTREATMENT void Delete();
        DLLTREATMENT void Bind(unsigned int textureInd = 0);
        DLLTREATMENT void Unbind();

#define CFAC_ClassName Texture1DClass
        CFAC_ClassConstructor(FullAccess,
            CFAC_FuncPtrConstr(SetData)
            CFAC_FuncPtrConstr(SetSubData)
            CFAC_FuncPtrConstr(GenerateMipmaps)
            CFAC_FuncPtrConstr(GetData)
            CFAC_FuncPtrConstr(sSettings_WrapTypeByX)
            CFAC_FuncPtrConstr(sSettings_WrapTypeByY)
            CFAC_FuncPtrConstr(sSettings_DownscalingFilt)
            CFAC_FuncPtrConstr(sSettings_UpscalingFilt)
            CFAC_FuncPtrConstr(sSettings_DepthStencilReadMode)
            CFAC_FuncPtrConstr(gID)
            CFAC_FuncPtrConstr(Bind)
            CFAC_FuncPtrConstr(Unbind)
        );
#undef CFAC_ClassName
    };


    class Texture2DClass {

        unsigned int ID;
        mutable bool Deleted = false;


    private:

        void _Constructor(Vector2U pixelsAmount, const void* data, const TextureDataSettingsClass& dataSets);

        void _UpdSettings_WrapTypeByX(TextureSettingsClass::WrapTypeEnum wrapTyp);
        void _UpdSettings_WrapTypeByY(TextureSettingsClass::WrapTypeEnum wrapTyp);
        void _UpdSettings_DownscalingFilt(TextureSettingsClass::DownscalingFilterFuncEnum filt);
        void _UpdSettings_UpscalingFilt(TextureSettingsClass::UpscalingFilterFuncEnum filt);
        void _UpdSettings_DepthStencilReadMode(TextureSettingsClass::DepthStencilReadModeEnum readMode);

        void _UpdateSettings(const TextureSettingsClass& sets);
    public:

        class CPU_DataData_Struct {

            Vector2U Size;

            mutable bool Empty = true;
            void* Data;

            TextureSettingsClass Settings;
            TextureDataSettingsClass DataSettings;

        public:
            DLLTREATMENT CPU_DataData_Struct(Vector2U size, void* data, TextureSettingsClass settings, TextureDataSettingsClass dataSettings);
            DLLTREATMENT CPU_DataData_Struct();
            DLLTREATMENT CPU_DataData_Struct(const CPU_DataData_Struct& toCopy);
            DLLTREATMENT CPU_DataData_Struct(const CPU_DataData_Struct* toCopy);
            DLLTREATMENT void operator=(const CPU_DataData_Struct& toCopy);
            DLLTREATMENT void Delete();
            DLLTREATMENT ~CPU_DataData_Struct();
        };

        DLLTREATMENT Texture2DClass(const char* filePath, const TextureSettingsClass& sets, CPU_DataData_Struct* dataExportPtr = nullptr);
        DLLTREATMENT Texture2DClass(Vector2U pixelsAmount, const void* data, const TextureSettingsClass& sets, const TextureDataSettingsClass& dataSets);
        DLLTREATMENT Texture2DClass(RespConstrFlag, const Texture2DClass& toCopy);
        DLLTREATMENT Texture2DClass(const Texture2DClass&& toCopy);
        DLLTREATMENT void operator=(const Texture2DClass&& toCopy);
        DLLTREATMENT ~Texture2DClass();

        DLLTREATMENT void SetData(Vector2U pixelsAmount, const void* data, const TextureDataSettingsClass& dataSets);
        DLLTREATMENT void SetSubData(Vector2U pixelsOffset, Vector2U pixelsAmount, const void* data,
            TextureDataSettingsClass::DataFormatOnCPU_Enum dataFormatOnCPU, TextureDataSettingsClass::DataTypeOnCPU_Enum dataTypeOnCPU);

        DLLTREATMENT void GenerateMipmaps();

		//this function is slow since it will get data from gpu to cpu
        //buffer should be not be nullptr, it should point to already allocated memory
        DLLTREATMENT void GetData(TextureDataSettingsClass::DataFormatOnCPU_Enum dataFormat, TextureDataSettingsClass::DataTypeOnCPU_Enum dataType, void* buffer);

        DLLTREATMENT void sSettings_WrapTypeByX(TextureSettingsClass::WrapTypeEnum wrapTypeByX);
        DLLTREATMENT void sSettings_WrapTypeByY(TextureSettingsClass::WrapTypeEnum wrapTypeByY);
        DLLTREATMENT void sSettings_DownscalingFilt(TextureSettingsClass::DownscalingFilterFuncEnum downscalingFilt);
        DLLTREATMENT void sSettings_UpscalingFilt(TextureSettingsClass::UpscalingFilterFuncEnum upscalingFilt);
        DLLTREATMENT void sSettings_DepthStencilReadMode(TextureSettingsClass::DepthStencilReadModeEnum depthStencilReadMode);

        DLLTREATMENT unsigned int gID();
        DLLTREATMENT void Delete();
        DLLTREATMENT void Bind(unsigned int textureInd = 0);
        DLLTREATMENT void Unbind();

#define CFAC_ClassName Texture2DClass
        CFAC_ClassConstructor(FullAccess,
            CFAC_FuncPtrConstr(SetData)
            CFAC_FuncPtrConstr(SetSubData)
            CFAC_FuncPtrConstr(GenerateMipmaps)
            CFAC_FuncPtrConstr(GetData)
            CFAC_FuncPtrConstr(sSettings_WrapTypeByX)
            CFAC_FuncPtrConstr(sSettings_WrapTypeByY)
            CFAC_FuncPtrConstr(sSettings_DownscalingFilt)
            CFAC_FuncPtrConstr(sSettings_UpscalingFilt)
            CFAC_FuncPtrConstr(sSettings_DepthStencilReadMode)
            CFAC_FuncPtrConstr(gID)
            CFAC_FuncPtrConstr(Bind)
            CFAC_FuncPtrConstr(Unbind)
        );
#undef CFAC_ClassName
    };*/
}
