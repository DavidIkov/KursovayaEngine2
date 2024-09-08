#pragma once
#include"DLL.h"
#include"Maths/Vector.h"
#include"Tools/DebuggingTools.h"

enum class TextureTypeEnum :unsigned short int {
    Texture1D = 1, Texture2D = 2
};

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

    //number of color components in texture
    DataFormatOnGPU_Enum DataFormatOnGPU;
    DataFormatOnCPU_Enum DataFormatOnCPU;
    DataTypeOnCPU_Enum DataTypeOnCPU;
};

template<TextureTypeEnum TextureType>
class TextureClass {
    
	unsigned int ID;

	mutable bool Deleted = false;
    
    
private:
    
    DLLTREATMENT void _Constructor(Vector<(unsigned int)TextureType, unsigned int> dimensions, const void* data, const TextureDataSettingsClass& dataSets);

    DLLTREATMENT void _UpdSettings_WrapTypeByX(TextureSettingsClass::WrapTypeEnum wrapTyp);
    DLLTREATMENT void _UpdSettings_WrapTypeByY(TextureSettingsClass::WrapTypeEnum wrapTyp);
    DLLTREATMENT void _UpdSettings_DownscalingFilt(TextureSettingsClass::DownscalingFilterFuncEnum filt);
    DLLTREATMENT void _UpdSettings_UpscalingFilt(TextureSettingsClass::UpscalingFilterFuncEnum filt);
    DLLTREATMENT void _UpdSettings_DepthStencilReadMode(TextureSettingsClass::DepthStencilReadModeEnum readMode);

    void _UpdateSettings(const TextureSettingsClass& sets) {
        _UpdSettings_WrapTypeByX(sets.WrapTypeByX);
        _UpdSettings_WrapTypeByY(sets.WrapTypeByY);
        _UpdSettings_DownscalingFilt(sets.DownscalingFilt);
        _UpdSettings_UpscalingFilt(sets.UpscalingFilt);
        _UpdSettings_DepthStencilReadMode(sets.DepthStencilReadMode);
    }
public:
    DLLTREATMENT TextureClass(const char* filePath, const TextureSettingsClass& sets);
    TextureClass(Vector<(unsigned int)TextureType, unsigned int> dimensions, const void* data, const TextureSettingsClass& sets, const TextureDataSettingsClass& dataSets) {
        _Constructor(dimensions, data, dataSets);
        _UpdateSettings(sets);
    }
    TextureClass(const TextureClass* toCopy) {
        memcpy(this, toCopy, sizeof(TextureClass));
        toCopy->Deleted = true;
    }
    TextureClass(const TextureClass&& toCopy) {
        memcpy(this, &toCopy, sizeof(TextureClass));
        toCopy.Deleted = true;
    }
    void operator=(const TextureClass&& toCopy) {
        this->~TextureClass();
        memcpy(this, &toCopy, sizeof(TextureClass));
        toCopy.Deleted = true;
    }
    DLLTREATMENT ~TextureClass();

    DLLTREATMENT void SetData(Vector<(unsigned int)TextureType, unsigned int> dimensions, const void* data, const TextureDataSettingsClass& dataSets);
    DLLTREATMENT void SetSubData(Vector<(unsigned int)TextureType, unsigned int> offsets, Vector<(unsigned int)TextureType, unsigned int> dimensions, const void* data, 
        TextureDataSettingsClass::DataFormatOnCPU_Enum dataFormatOnCPU, TextureDataSettingsClass::DataTypeOnCPU_Enum dataTypeOnCPU);

    DLLTREATMENT void GenerateMipmaps();
    
    void sSettings_WrapTypeByX(TextureSettingsClass::WrapTypeEnum wrapTypeByX) { _UpdSettings_WrapTypeByX(wrapTypeByX); }
    void sSettings_WrapTypeByY(TextureSettingsClass::WrapTypeEnum wrapTypeByY) { _UpdSettings_WrapTypeByY(wrapTypeByY); }
    void sSettings_DownscalingFilt(TextureSettingsClass::DownscalingFilterFuncEnum downscalingFilt) { _UpdSettings_DownscalingFilt(downscalingFilt); }
    void sSettings_UpscalingFilt(TextureSettingsClass::UpscalingFilterFuncEnum upscalingFilt) { _UpdSettings_UpscalingFilt(upscalingFilt); }
    void sSettings_DepthStencilReadMode(TextureSettingsClass::DepthStencilReadModeEnum depthStencilReadMode) { _UpdSettings_DepthStencilReadMode(depthStencilReadMode); }

    unsigned int gID() const {
#if defined Debug
        if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "TEXTURE IS DELETED, ACCESING ITS ID MAY CAUSE ERRORS", KURSAVAYAENGINE2_CORE_ERRORS::ACCESSING_IMPOSSIBLE_TO_ACCESS_INSTANCE_DATA });
#endif
        return ID;
    }
    void Delete() {
#if defined Debug
        if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "TEXTURE IS ALREADY DELETED", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_UNNECESARY_FUNCTION });
        else 
#endif
            this->~TextureClass();
    }
    DLLTREATMENT void Bind(unsigned int textureInd = 0) const;
    DLLTREATMENT void Unbind();
};










/*
class TextureClass {

    unsigned int ID;

    mutable bool Deleted = false;

public:
    struct SettingsClass {
        enum class WrapTypeEnum :unsigned short int {
            ClampToEdge, ClampToBorder, MirroredRepeat, Repeat, MirrorClampToEdge
        };
        WrapTypeEnum WrapTypeByX;
        WrapTypeEnum WrapTypeByY;
        enum class DownscalingFilterFuncEnum :unsigned short int {
            Nearest, Linear, NearestMipmapNearest, NearestMipmapLinear, LinearMipmapLinear, LinearMipmapNearest
        };
        DownscalingFilterFuncEnum DownscalingFilt;
        enum class UpscalingFilterFuncEnum :unsigned short int {
            Nearest, Linear
        };
        UpscalingFilterFuncEnum UpscalingFilt;
        enum class DepthStencilReadModeEnum :unsigned short int {
            Depth, Stencil
        };
        DepthStencilReadModeEnum DepthStencilReadMode;
    };
    struct DataSettingsClass {
        //number of color components in texture
        enum class DataFormatOnGPU_Enum :unsigned short int {
            DepthComponent, DepthStencil, Red, RG, RGB, RGBA
        };
        DataFormatOnGPU_Enum DataFormatOnGPU;
        enum class DataFormatOnCPU_Enum :unsigned short int {
            Red, RG, RGB, BGR, RGBA, BGRA, RedInteger, RG_Integer, RGB_Integer, BGR_Integer, RGBA_Integer, BGRA_Integer, StencilIndex,
            DepthComponent, DepthStencil
        };
        DataFormatOnCPU_Enum DataFormatOnCPU;
        enum class DataTypeOnCPU_Enum :unsigned short int {
            UnsignedByte, Byte, UnsignedShort, Short, UnsignedInt, Int, Float, UnsignedInt_24_8
        };
        DataTypeOnCPU_Enum DataTypeOnCPU;
    };
    enum class TypeEnum :unsigned short int {
        Texture2D, Texture1D
    };
private:

    unsigned int GL_TextureType;

    void _Constructor(unsigned int width, unsigned int height, const void* data, TypeEnum type, const DataSettingsClass& dataSets);

    void _UpdSettings_WrapTypeByX(SettingsClass::WrapTypeEnum wrapTyp);
    void _UpdSettings_WrapTypeByY(SettingsClass::WrapTypeEnum wrapTyp);
    void _UpdSettings_DownscalingFilt(SettingsClass::DownscalingFilterFuncEnum filt);
    void _UpdSettings_UpscalingFilt(SettingsClass::UpscalingFilterFuncEnum filt);
    void _UpdSettings_DepthStencilReadMode(SettingsClass::DepthStencilReadModeEnum readMode);

    void _UpdateSettings(const SettingsClass& sets);
public:
    DLLTREATMENT TextureClass(const char* filePath, TypeEnum type, const SettingsClass& sets);
    DLLTREATMENT TextureClass(unsigned int width, unsigned int height, const void* data, TypeEnum type, const SettingsClass& sets, const DataSettingsClass& dataSets);
    DLLTREATMENT TextureClass(const TextureClass* toCopy);
    DLLTREATMENT TextureClass(const TextureClass&& toCopy);
    DLLTREATMENT void operator=(const TextureClass&& toCopy);
    DLLTREATMENT ~TextureClass();

    DLLTREATMENT void GenerateMipmaps();

    DLLTREATMENT void sSettings_WrapTypeByX(SettingsClass::WrapTypeEnum wrapTypeByX);
    DLLTREATMENT void sSettings_WrapTypeByY(SettingsClass::WrapTypeEnum wrapTypeByY);
    DLLTREATMENT void sSettings_DownscalingFilt(SettingsClass::DownscalingFilterFuncEnum downscalingFilt);
    DLLTREATMENT void sSettings_UpscalingFilt(SettingsClass::UpscalingFilterFuncEnum upscalingFilt);
    DLLTREATMENT void sSettings_DepthStencilReadMode(SettingsClass::DepthStencilReadModeEnum depthStencilReadMode);

    DLLTREATMENT unsigned int gID() const;
    DLLTREATMENT void Delete();
    DLLTREATMENT void Bind(unsigned int textureInd = 0) const;
    DLLTREATMENT void Unbind();
};
*/