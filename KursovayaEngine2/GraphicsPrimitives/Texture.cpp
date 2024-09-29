#include"Texture.h"
#include"stb_image/stb_image.h"
#include"Tools/GLDebug.h"
#include"Tools/ReadFromFile.h"
#include"glad/glad.h"
#include"Tools/DebuggingTools.h"

using namespace GraphicsPrimitives;

static unsigned int _DataFormatOnGPU_SwitchCase(TextureDataSettingsClass::DataFormatOnGPU_Enum format) {
    switch (format) {
    case TextureDataSettingsClass::DataFormatOnGPU_Enum::DepthComponent: return GL_DEPTH_COMPONENT24;
    case TextureDataSettingsClass::DataFormatOnGPU_Enum::DepthStencil: return GL_DEPTH24_STENCIL8;
    case TextureDataSettingsClass::DataFormatOnGPU_Enum::Red: return GL_RED;
    case TextureDataSettingsClass::DataFormatOnGPU_Enum::RG: return GL_RG;
    case TextureDataSettingsClass::DataFormatOnGPU_Enum::RGB: return GL_RGB;
    case TextureDataSettingsClass::DataFormatOnGPU_Enum::RGBA: return GL_RGBA;
    default: return 0;
    }
}
static unsigned int _DataFormatOnCPU_SwitchCase(TextureDataSettingsClass::DataFormatOnCPU_Enum format) {
    switch (format) {
    case TextureDataSettingsClass::DataFormatOnCPU_Enum::Red: return GL_RED;
    case TextureDataSettingsClass::DataFormatOnCPU_Enum::RG: return GL_RG;
    case TextureDataSettingsClass::DataFormatOnCPU_Enum::RGB: return GL_RGB;
    case TextureDataSettingsClass::DataFormatOnCPU_Enum::BGR: return GL_BGR;
    case TextureDataSettingsClass::DataFormatOnCPU_Enum::RGBA: return GL_RGBA;
    case TextureDataSettingsClass::DataFormatOnCPU_Enum::BGRA: return GL_BGRA;
    case TextureDataSettingsClass::DataFormatOnCPU_Enum::RedInteger: return GL_RED_INTEGER;
    case TextureDataSettingsClass::DataFormatOnCPU_Enum::RG_Integer: return GL_RG_INTEGER;
    case TextureDataSettingsClass::DataFormatOnCPU_Enum::RGB_Integer: return GL_RGB_INTEGER;
    case TextureDataSettingsClass::DataFormatOnCPU_Enum::BGR_Integer: return GL_BGR_INTEGER;
    case TextureDataSettingsClass::DataFormatOnCPU_Enum::RGBA_Integer: return GL_RGBA_INTEGER;
    case TextureDataSettingsClass::DataFormatOnCPU_Enum::BGRA_Integer: return GL_BGRA_INTEGER;
    case TextureDataSettingsClass::DataFormatOnCPU_Enum::StencilIndex: return GL_STENCIL_INDEX;
    case TextureDataSettingsClass::DataFormatOnCPU_Enum::DepthComponent: return GL_DEPTH_COMPONENT;
    case TextureDataSettingsClass::DataFormatOnCPU_Enum::DepthStencil: return GL_DEPTH_STENCIL;
    default: return 0;
    }
}
static unsigned int _DataTypeOnCPU_SwitchCase(TextureDataSettingsClass::DataTypeOnCPU_Enum type) {
    switch (type) {
    case TextureDataSettingsClass::DataTypeOnCPU_Enum::UnsignedByte: return GL_UNSIGNED_BYTE;
    case TextureDataSettingsClass::DataTypeOnCPU_Enum::Byte: return GL_BYTE;
    case TextureDataSettingsClass::DataTypeOnCPU_Enum::UnsignedShort: return GL_UNSIGNED_SHORT;
    case TextureDataSettingsClass::DataTypeOnCPU_Enum::Short: return GL_SHORT;
    case TextureDataSettingsClass::DataTypeOnCPU_Enum::UnsignedInt: return GL_UNSIGNED_INT;
    case TextureDataSettingsClass::DataTypeOnCPU_Enum::Int: return GL_INT;
    case TextureDataSettingsClass::DataTypeOnCPU_Enum::Float: return GL_FLOAT;
    case TextureDataSettingsClass::DataTypeOnCPU_Enum::UnsignedInt_24_8: return GL_UNSIGNED_INT_24_8;
    default: return 0;
    }
}
static unsigned int _DataTypeOnCPU_Sizeof_SwitchCase(TextureDataSettingsClass::DataTypeOnCPU_Enum type) {
    switch (type) {
    case TextureDataSettingsClass::DataTypeOnCPU_Enum::UnsignedByte: return sizeof(unsigned char);
    case TextureDataSettingsClass::DataTypeOnCPU_Enum::Byte: return sizeof(unsigned char);
    case TextureDataSettingsClass::DataTypeOnCPU_Enum::UnsignedInt: return sizeof(unsigned int);
    case TextureDataSettingsClass::DataTypeOnCPU_Enum::Int: return sizeof(int);
    case TextureDataSettingsClass::DataTypeOnCPU_Enum::Float: return sizeof(float);
    default: return 0;
    }
}
static unsigned int _WrapType_SwitchCase(TextureSettingsClass::WrapTypeEnum wrapTyp) {
    switch (wrapTyp) {
    case TextureSettingsClass::WrapTypeEnum::ClampToEdge: return GL_CLAMP_TO_EDGE;
    case TextureSettingsClass::WrapTypeEnum::ClampToBorder: return GL_CLAMP_TO_BORDER;
    case TextureSettingsClass::WrapTypeEnum::MirroredRepeat: return GL_MIRRORED_REPEAT;
    case TextureSettingsClass::WrapTypeEnum::Repeat: return GL_REPEAT;
    case TextureSettingsClass::WrapTypeEnum::MirrorClampToEdge: return GL_MIRROR_CLAMP_TO_EDGE;
    default: return 0;
    }
}
static unsigned int _DownscalingFilterFunc_SwitchCase(TextureSettingsClass::DownscalingFilterFuncEnum filt) {
    switch (filt) {
    case TextureSettingsClass::DownscalingFilterFuncEnum::Nearest: return GL_NEAREST;
    case TextureSettingsClass::DownscalingFilterFuncEnum::Linear: return GL_LINEAR;
    case TextureSettingsClass::DownscalingFilterFuncEnum::NearestMipmapNearest: return GL_NEAREST_MIPMAP_NEAREST;
    case TextureSettingsClass::DownscalingFilterFuncEnum::NearestMipmapLinear: return GL_NEAREST_MIPMAP_LINEAR;
    case TextureSettingsClass::DownscalingFilterFuncEnum::LinearMipmapLinear: return GL_LINEAR_MIPMAP_LINEAR;
    case TextureSettingsClass::DownscalingFilterFuncEnum::LinearMipmapNearest: return GL_LINEAR_MIPMAP_NEAREST;
    default: return 0;
    }
}
static unsigned int _UpscalingFilterFunc_SwitchCase(TextureSettingsClass::UpscalingFilterFuncEnum filt) {
    switch (filt) {
    case TextureSettingsClass::UpscalingFilterFuncEnum::Nearest: return GL_NEAREST;
    case TextureSettingsClass::UpscalingFilterFuncEnum::Linear: return GL_LINEAR;
    default: return 0;
    }
}
static unsigned int _DepthStencilReadMode_SwitchCase(TextureSettingsClass::DepthStencilReadModeEnum readMode) {
    switch (readMode) {
    case TextureSettingsClass::DepthStencilReadModeEnum::Depth: return GL_DEPTH_COMPONENT;
    case TextureSettingsClass::DepthStencilReadModeEnum::Stencil: return GL_STENCIL_INDEX;
    default: return 0;
    }
}



void TextureClass::_Constructor(Vector3U pixelsAmount, const void* data, const TextureDataSettingsClass& dataSets) {

    switch (Dimensions) {
    case DimensionsEnum::One: GL_TexEnum = GL_TEXTURE_1D; break;
    case DimensionsEnum::Two: GL_TexEnum = GL_TEXTURE_2D; break;
    case DimensionsEnum::Three: GL_TexEnum = GL_TEXTURE_3D; break;
    }

    glSC(glGenTextures(1, &ID));
    glSC(glBindTexture(GL_TexEnum, ID));

    SetData(pixelsAmount, data, dataSets);
}

void TextureClass::_UpdSettings_WrapTypeByX(TextureSettingsClass::WrapTypeEnum wrapTyp) {
    Bind();
    glSC(glTexParameteri(GL_TexEnum, GL_TEXTURE_WRAP_S, _WrapType_SwitchCase(wrapTyp)));
}
void TextureClass::_UpdSettings_WrapTypeByY(TextureSettingsClass::WrapTypeEnum wrapTyp) {
    Bind();
    glSC(glTexParameteri(GL_TexEnum, GL_TEXTURE_WRAP_T, _WrapType_SwitchCase(wrapTyp)));
}
void TextureClass::_UpdSettings_DownscalingFilt(TextureSettingsClass::DownscalingFilterFuncEnum filt) {
    Bind();
    glSC(glTexParameteri(GL_TexEnum, GL_TEXTURE_MIN_FILTER, _DownscalingFilterFunc_SwitchCase(filt)));
}
void TextureClass::_UpdSettings_UpscalingFilt(TextureSettingsClass::UpscalingFilterFuncEnum filt) {
    Bind();
    glSC(glTexParameteri(GL_TexEnum, GL_TEXTURE_MAG_FILTER, _UpscalingFilterFunc_SwitchCase(filt)));
}
void TextureClass::_UpdSettings_DepthStencilReadMode(TextureSettingsClass::DepthStencilReadModeEnum readMode) {
    Bind();
    glSC(glTexParameteri(GL_TexEnum, GL_DEPTH_STENCIL_TEXTURE_MODE, _DepthStencilReadMode_SwitchCase(readMode)));
}

void TextureClass::_UpdateSettings(const TextureSettingsClass& sets) {
    _UpdSettings_WrapTypeByX(sets.WrapTypeByX);
    _UpdSettings_WrapTypeByY(sets.WrapTypeByY);
    _UpdSettings_DownscalingFilt(sets.DownscalingFilt);
    _UpdSettings_UpscalingFilt(sets.UpscalingFilt);
    _UpdSettings_DepthStencilReadMode(sets.DepthStencilReadMode);
}

TextureClass::TextureClass(DimensionsEnum dimensions, const char* filePath, const TextureSettingsClass& sets) :Dimensions(dimensions) {

    int width, height, textureChannelsAmount;
    unsigned char* textureData = stbi_load(filePath, &width, &height, &textureChannelsAmount, 0);
    if (textureData == nullptr) {
        DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical, "FAILED TO LOAD IMAGE", KURSAVAYAENGINE2_CORE_ERRORS::FAILED_THIRD_PARTY_FUNCTION });
    }
    TextureDataSettingsClass dataSettings;
    dataSettings.DataFormatOnGPU = TextureDataSettingsClass::DataFormatOnGPU_Enum::RGBA;
    dataSettings.DataFormatOnCPU = (textureChannelsAmount == 4) ? TextureDataSettingsClass::DataFormatOnCPU_Enum::RGBA : TextureDataSettingsClass::DataFormatOnCPU_Enum::RGB;
    dataSettings.DataTypeOnCPU = TextureDataSettingsClass::DataTypeOnCPU_Enum::UnsignedByte;

    _Constructor(Vector3U(width, height, 0), textureData, dataSettings);
    _UpdateSettings(sets);
    
    stbi_image_free(textureData);
}
TextureClass::TextureClass(DimensionsEnum dimensions, Vector3U pixelsAmount, const void* data, const TextureSettingsClass& sets, const TextureDataSettingsClass& dataSets) :Dimensions(dimensions) {
    _Constructor(pixelsAmount, data, dataSets);
    _UpdateSettings(sets);
}
TextureClass::TextureClass(RespConstrFlag, const TextureClass& toCopy) {
    memcpy(this, &toCopy, sizeof(TextureClass));
    toCopy.Deleted = true;
}
TextureClass::TextureClass(const TextureClass&& toCopy) {
    memcpy(this, &toCopy, sizeof(TextureClass));
    toCopy.Deleted = true;
}
void TextureClass::operator=(const TextureClass&& toCopy) {
    this->~TextureClass();
    memcpy(this, &toCopy, sizeof(TextureClass));
    toCopy.Deleted = true;
}
TextureClass::~TextureClass() {
    if (not Deleted) {
        glSC(glDeleteTextures(1, &ID));
        Deleted = true;
    }
}

void TextureClass::SetData(Vector3U pixelsAmount, const void* data, const TextureDataSettingsClass& dataSets) {
    Bind();
    unsigned int gl_dataFormatOnGPU = _DataFormatOnGPU_SwitchCase(dataSets.DataFormatOnGPU);
    unsigned int gl_dataFormatOnCPU = _DataFormatOnCPU_SwitchCase(dataSets.DataFormatOnCPU);
    unsigned int gl_dataTypeOnCPU = _DataTypeOnCPU_SwitchCase(dataSets.DataTypeOnCPU);
    switch (Dimensions) {
    case DimensionsEnum::One: glSC(glTexImage1D(GL_TexEnum, 0, gl_dataFormatOnGPU, pixelsAmount[0], 0, gl_dataFormatOnCPU, gl_dataTypeOnCPU, data)); return;
    case DimensionsEnum::Two: glSC(glTexImage2D(GL_TexEnum, 0, gl_dataFormatOnGPU, pixelsAmount[0], pixelsAmount[1], 0, gl_dataFormatOnCPU, gl_dataTypeOnCPU, data)); return;
    case DimensionsEnum::Three: glSC(glTexImage3D(GL_TexEnum, 0, gl_dataFormatOnGPU, pixelsAmount[0], pixelsAmount[1], pixelsAmount[2], 0, gl_dataFormatOnCPU, gl_dataTypeOnCPU, data)); return;
    }
}
void TextureClass::SetSubData(Vector3U pixelsOffset, Vector3U pixelsAmount, const void* data,
    TextureDataSettingsClass::DataFormatOnCPU_Enum dataFormatOnCPU, TextureDataSettingsClass::DataTypeOnCPU_Enum dataTypeOnCPU) {
    Bind();
    glSC(glTexSubImage2D(GL_TEXTURE_2D, 0, pixelsOffset[0], pixelsOffset[1], pixelsAmount[0], pixelsAmount[1],
        _DataFormatOnCPU_SwitchCase(dataFormatOnCPU), _DataTypeOnCPU_SwitchCase(dataTypeOnCPU), data));
    unsigned int gl_dataFormatOnCPU = _DataFormatOnCPU_SwitchCase(dataFormatOnCPU);
    unsigned int gl_dataTypeOnCPU = _DataTypeOnCPU_SwitchCase(dataTypeOnCPU);
    switch (Dimensions) {
    case DimensionsEnum::One: glSC(glTexSubImage1D(GL_TexEnum, 0, pixelsOffset[0], pixelsAmount[0], gl_dataFormatOnCPU, gl_dataTypeOnCPU, data)); return;
    case DimensionsEnum::Two: glSC(glTexSubImage2D(GL_TexEnum, 0, pixelsOffset[0], pixelsOffset[1], pixelsAmount[0], pixelsAmount[1], gl_dataFormatOnCPU, gl_dataTypeOnCPU, data)); return;
    case DimensionsEnum::Three: glSC(glTexSubImage3D(GL_TexEnum, 0, pixelsOffset[0], pixelsOffset[1], pixelsOffset[2], pixelsAmount[0], pixelsAmount[1], pixelsAmount[2], gl_dataFormatOnCPU, gl_dataTypeOnCPU, data)); return;
    }
}

void TextureClass::GenerateMipmaps() {
    glBindTexture(GL_TexEnum, ID);
    glSC(glGenerateMipmap(GL_TexEnum));
}

void TextureClass::GetData(TextureDataSettingsClass::DataFormatOnCPU_Enum dataFormat, TextureDataSettingsClass::DataTypeOnCPU_Enum dataType, void* buffer) {
#if defined Debug
    if (buffer == nullptr) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical,"buffer shouldnt be nullptr",KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_FUNCTION_WITH_INVALID_ARGUMENTS });
#endif
    glSC(glGetTexImage(GL_TexEnum, 0, _DataFormatOnCPU_SwitchCase(dataFormat), _DataTypeOnCPU_SwitchCase(dataType), buffer));
}

void TextureClass::sSettings_WrapTypeByX(TextureSettingsClass::WrapTypeEnum wrapTypeByX) { _UpdSettings_WrapTypeByX(wrapTypeByX); }
void TextureClass::sSettings_WrapTypeByY(TextureSettingsClass::WrapTypeEnum wrapTypeByY) { _UpdSettings_WrapTypeByY(wrapTypeByY); }
void TextureClass::sSettings_DownscalingFilt(TextureSettingsClass::DownscalingFilterFuncEnum downscalingFilt) { _UpdSettings_DownscalingFilt(downscalingFilt); }
void TextureClass::sSettings_UpscalingFilt(TextureSettingsClass::UpscalingFilterFuncEnum upscalingFilt) { _UpdSettings_UpscalingFilt(upscalingFilt); }
void TextureClass::sSettings_DepthStencilReadMode(TextureSettingsClass::DepthStencilReadModeEnum depthStencilReadMode) { _UpdSettings_DepthStencilReadMode(depthStencilReadMode); }

unsigned int TextureClass::gID()  {
#if defined Debug
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "TEXTURE IS DELETED, ACCESING ITS ID MAY CAUSE ERRORS", KURSAVAYAENGINE2_CORE_ERRORS::ACCESSING_IMPOSSIBLE_TO_ACCESS_INSTANCE_DATA });
#endif
    return ID;
}
void TextureClass::Delete() {
#if defined Debug
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "TEXTURE IS ALREADY DELETED", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_UNNECESARY_FUNCTION });
    else
#endif
        this->~TextureClass();
}

void TextureClass::Bind(unsigned int textureInd){
#if defined Debug
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "TEXTURE IS DELETED, YOU CANT BIND IT", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION });
    else
#endif
    {
        glSC(glActiveTexture(GL_TEXTURE0 + textureInd));
        glSC(glBindTexture(GL_TexEnum, ID));
    }
}
void TextureClass::Unbind() {
    glSC(glBindTexture(GL_TexEnum, 0));
}






/*void Texture1DClass::_Constructor(unsigned int pixelsAmount, const void* data, const TextureDataSettingsClass& dataSets) {
    glSC(glGenTextures(1, &ID));
    glSC(glBindTexture(GL_TEXTURE_1D, ID));

    glSC(glTexImage1D(GL_TEXTURE_1D, 0, _DataFormatOnGPU_SwitchCase(dataSets.DataFormatOnGPU), pixelsAmount, 0,
        _DataFormatOnCPU_SwitchCase(dataSets.DataFormatOnCPU), _DataTypeOnCPU_SwitchCase(dataSets.DataTypeOnCPU), data));
}

void Texture1DClass::_UpdSettings_WrapTypeByX(TextureSettingsClass::WrapTypeEnum wrapTyp) {
    Bind();
    glSC(glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, _WrapType_SwitchCase(wrapTyp)));
}
void Texture1DClass::_UpdSettings_WrapTypeByY(TextureSettingsClass::WrapTypeEnum wrapTyp) {
    Bind();
    glSC(glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, _WrapType_SwitchCase(wrapTyp)));
}
void Texture1DClass::_UpdSettings_DownscalingFilt(TextureSettingsClass::DownscalingFilterFuncEnum filt) {
    Bind();
    glSC(glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, _DownscalingFilterFunc_SwitchCase(filt)));
}
void Texture1DClass::_UpdSettings_UpscalingFilt(TextureSettingsClass::UpscalingFilterFuncEnum filt) {
    Bind();
    glSC(glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, _UpscalingFilterFunc_SwitchCase(filt)));
}
void Texture1DClass::_UpdSettings_DepthStencilReadMode(TextureSettingsClass::DepthStencilReadModeEnum readMode) {
    Bind();
    glSC(glTexParameteri(GL_TEXTURE_1D, GL_DEPTH_STENCIL_TEXTURE_MODE, _DepthStencilReadMode_SwitchCase(readMode)));
}

void Texture1DClass::_UpdateSettings(const TextureSettingsClass& sets) {
    _UpdSettings_WrapTypeByX(sets.WrapTypeByX);
    _UpdSettings_WrapTypeByY(sets.WrapTypeByY);
    _UpdSettings_DownscalingFilt(sets.DownscalingFilt);
    _UpdSettings_UpscalingFilt(sets.UpscalingFilt);
    _UpdSettings_DepthStencilReadMode(sets.DepthStencilReadMode);
}

Texture1DClass::Texture1DClass(const char* filePath, const TextureSettingsClass& sets, CPU_DataData_Struct* dataExportPtr) {

    int width, height, textureChannelsAmount;
    unsigned char* textureData = stbi_load(filePath, &width, &height, &textureChannelsAmount, 0);
    if (textureData == nullptr) {
        DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical, "FAILED TO LOAD IMAGE", KURSAVAYAENGINE2_CORE_ERRORS::FAILED_THIRD_PARTY_FUNCTION });
    }
    TextureDataSettingsClass dataSettings;
    dataSettings.DataFormatOnGPU = TextureDataSettingsClass::DataFormatOnGPU_Enum::RGBA;
    dataSettings.DataFormatOnCPU = (textureChannelsAmount == 4) ? TextureDataSettingsClass::DataFormatOnCPU_Enum::RGBA : TextureDataSettingsClass::DataFormatOnCPU_Enum::RGB;
    dataSettings.DataTypeOnCPU = TextureDataSettingsClass::DataTypeOnCPU_Enum::UnsignedByte;

    _Constructor(width * height , textureData, dataSettings);
    _UpdateSettings(sets);

    stbi_image_free(textureData);
}
Texture1DClass::Texture1DClass(unsigned int pixelsAmount, const void* data, const TextureSettingsClass& sets, const TextureDataSettingsClass& dataSets) {
    _Constructor(pixelsAmount, data, dataSets);
    _UpdateSettings(sets);
}
Texture1DClass::Texture1DClass(RespConstrFlag, const Texture1DClass& toCopy) {
    memcpy(this, &toCopy, sizeof(Texture1DClass));
    toCopy.Deleted = true;
}
Texture1DClass::Texture1DClass(const Texture1DClass&& toCopy) {
    memcpy(this, &toCopy, sizeof(Texture1DClass));
    toCopy.Deleted = true;
}
void Texture1DClass::operator=(const Texture1DClass&& toCopy){
    this->~Texture1DClass();
    memcpy(this, &toCopy, sizeof(Texture1DClass));
    toCopy.Deleted = true;
}
Texture1DClass::~Texture1DClass() {
    if (not Deleted) {
        glSC(glDeleteTextures(1, &ID));
        Deleted = true;
    }
}

void Texture1DClass::SetData(unsigned int pixelsAmount, const void* data, const TextureDataSettingsClass& dataSets) {
    Bind();
    glSC(glTexImage1D(GL_TEXTURE_1D, 0, _DataFormatOnGPU_SwitchCase(dataSets.DataFormatOnGPU), pixelsAmount, 0,
        _DataFormatOnCPU_SwitchCase(dataSets.DataFormatOnCPU), _DataTypeOnCPU_SwitchCase(dataSets.DataTypeOnCPU), data));
}
void Texture1DClass::SetSubData(unsigned int pixelsOffset, unsigned int pixelsAmount, const void* data,
    TextureDataSettingsClass::DataFormatOnCPU_Enum dataFormatOnCPU, TextureDataSettingsClass::DataTypeOnCPU_Enum dataTypeOnCPU) {
    Bind();
    glSC(glTexSubImage1D(GL_TEXTURE_1D, 0, pixelsOffset, pixelsAmount,
        _DataFormatOnCPU_SwitchCase(dataFormatOnCPU), _DataTypeOnCPU_SwitchCase(dataTypeOnCPU), data));
}

void Texture1DClass::GenerateMipmaps() {
    glBindTexture(GL_TEXTURE_1D, ID);
    glSC(glGenerateMipmap(GL_TEXTURE_1D));
}
void Texture1DClass::GetData(TextureDataSettingsClass::DataFormatOnCPU_Enum dataFormat, TextureDataSettingsClass::DataTypeOnCPU_Enum dataType, void* buffer) {
#if defined Debug
    if (buffer == nullptr) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical,"buffer shouldnt be nullptr",KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_FUNCTION_WITH_INVALID_ARGUMENTS });
#endif
    glSC(glGetTexImage(GL_TEXTURE_1D, 0, _DataFormatOnCPU_SwitchCase(dataFormat), _DataTypeOnCPU_SwitchCase(dataType), buffer));
}

void Texture1DClass::sSettings_WrapTypeByX(TextureSettingsClass::WrapTypeEnum wrapTypeByX) { _UpdSettings_WrapTypeByX(wrapTypeByX); }
void Texture1DClass::sSettings_WrapTypeByY(TextureSettingsClass::WrapTypeEnum wrapTypeByY) { _UpdSettings_WrapTypeByY(wrapTypeByY); }
void Texture1DClass::sSettings_DownscalingFilt(TextureSettingsClass::DownscalingFilterFuncEnum downscalingFilt) { _UpdSettings_DownscalingFilt(downscalingFilt); }
void Texture1DClass::sSettings_UpscalingFilt(TextureSettingsClass::UpscalingFilterFuncEnum upscalingFilt) { _UpdSettings_UpscalingFilt(upscalingFilt); }
void Texture1DClass::sSettings_DepthStencilReadMode(TextureSettingsClass::DepthStencilReadModeEnum depthStencilReadMode) { _UpdSettings_DepthStencilReadMode(depthStencilReadMode); }

unsigned int Texture1DClass::gID() {
#if defined Debug
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "TEXTURE IS DELETED, ACCESING ITS ID MAY CAUSE ERRORS", KURSAVAYAENGINE2_CORE_ERRORS::ACCESSING_IMPOSSIBLE_TO_ACCESS_INSTANCE_DATA });
#endif
    return ID;
}
void Texture1DClass::Delete() {
#if defined Debug
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "TEXTURE IS ALREADY DELETED", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_UNNECESARY_FUNCTION });
    else
#endif
        this->~Texture1DClass();
}

void Texture1DClass::Bind(unsigned int textureInd) {
#if defined Debug
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "TEXTURE IS DELETED, YOU CANT BIND IT", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION });
    else 
#endif
    {
        glSC(glActiveTexture(GL_TEXTURE0 + textureInd));
        glSC(glBindTexture(GL_TEXTURE_1D, ID));
    }
}
void Texture1DClass::Unbind() {
    glSC(glBindTexture(GL_TEXTURE_1D, 0));
}



















void Texture2DClass::_Constructor(Vector2U pixelsAmount, const void* data, const TextureDataSettingsClass& dataSets) {
    glSC(glGenTextures(1, &ID));
    glSC(glBindTexture(GL_TEXTURE_2D, ID));

    glSC(glTexImage2D(GL_TEXTURE_2D, 0, _DataFormatOnGPU_SwitchCase(dataSets.DataFormatOnGPU), pixelsAmount[0], pixelsAmount[1], 0,
        _DataFormatOnCPU_SwitchCase(dataSets.DataFormatOnCPU), _DataTypeOnCPU_SwitchCase(dataSets.DataTypeOnCPU), data));
}

void Texture2DClass::_UpdSettings_WrapTypeByX(TextureSettingsClass::WrapTypeEnum wrapTyp) {
    Bind();
    glSC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _WrapType_SwitchCase(wrapTyp)));
}
void Texture2DClass::_UpdSettings_WrapTypeByY(TextureSettingsClass::WrapTypeEnum wrapTyp) {
    Bind();
    glSC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _WrapType_SwitchCase(wrapTyp)));
}
void Texture2DClass::_UpdSettings_DownscalingFilt(TextureSettingsClass::DownscalingFilterFuncEnum filt) {
    Bind();
    glSC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _DownscalingFilterFunc_SwitchCase(filt)));
}
void Texture2DClass::_UpdSettings_UpscalingFilt(TextureSettingsClass::UpscalingFilterFuncEnum filt) {
    Bind();
    glSC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _UpscalingFilterFunc_SwitchCase(filt)));
}
void Texture2DClass::_UpdSettings_DepthStencilReadMode(TextureSettingsClass::DepthStencilReadModeEnum readMode) {
    Bind();
    glSC(glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_STENCIL_TEXTURE_MODE, _DepthStencilReadMode_SwitchCase(readMode)));
}

void Texture2DClass::_UpdateSettings(const TextureSettingsClass& sets) {
    _UpdSettings_WrapTypeByX(sets.WrapTypeByX);
    _UpdSettings_WrapTypeByY(sets.WrapTypeByY);
    _UpdSettings_DownscalingFilt(sets.DownscalingFilt);
    _UpdSettings_UpscalingFilt(sets.UpscalingFilt);
    _UpdSettings_DepthStencilReadMode(sets.DepthStencilReadMode);
}

Texture2DClass::Texture2DClass(const char* filePath, const TextureSettingsClass& sets, CPU_DataData_Struct* dataExportPtr) {

    int width, height, textureChannelsAmount;
    unsigned char* textureData = stbi_load(filePath, &width, &height, &textureChannelsAmount, 0);
    if (textureData == nullptr) {
        DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical, "FAILED TO LOAD IMAGE", KURSAVAYAENGINE2_CORE_ERRORS::FAILED_THIRD_PARTY_FUNCTION });
    }
    TextureDataSettingsClass dataSettings;
    dataSettings.DataFormatOnGPU = TextureDataSettingsClass::DataFormatOnGPU_Enum::RGBA;
    dataSettings.DataFormatOnCPU = (textureChannelsAmount == 4) ? TextureDataSettingsClass::DataFormatOnCPU_Enum::RGBA : TextureDataSettingsClass::DataFormatOnCPU_Enum::RGB;
    dataSettings.DataTypeOnCPU = TextureDataSettingsClass::DataTypeOnCPU_Enum::UnsignedByte;

    _Constructor(Vector2U(width, height), textureData, dataSettings);
    _UpdateSettings(sets);
    
    stbi_image_free(textureData);
}
Texture2DClass::Texture2DClass(Vector2U pixelsAmount, const void* data, const TextureSettingsClass& sets, const TextureDataSettingsClass& dataSets) {
    _Constructor(pixelsAmount, data, dataSets);
    _UpdateSettings(sets);
}
Texture2DClass::Texture2DClass(RespConstrFlag, const Texture2DClass& toCopy) {
    memcpy(this, &toCopy, sizeof(Texture2DClass));
    toCopy.Deleted = true;
}
Texture2DClass::Texture2DClass(const Texture2DClass&& toCopy) {
    memcpy(this, &toCopy, sizeof(Texture2DClass));
    toCopy.Deleted = true;
}
void Texture2DClass::operator=(const Texture2DClass&& toCopy) {
    this->~Texture2DClass();
    memcpy(this, &toCopy, sizeof(Texture2DClass));
    toCopy.Deleted = true;
}
Texture2DClass::~Texture2DClass() {
    if (not Deleted) {
        glSC(glDeleteTextures(1, &ID));
        Deleted = true;
    }
}

void Texture2DClass::SetData(Vector2U pixelsAmount, const void* data, const TextureDataSettingsClass& dataSets) {
    Bind();
    glSC(glTexImage2D(GL_TEXTURE_2D, 0, _DataFormatOnGPU_SwitchCase(dataSets.DataFormatOnGPU), pixelsAmount[0], pixelsAmount[1], 0,
        _DataFormatOnCPU_SwitchCase(dataSets.DataFormatOnCPU), _DataTypeOnCPU_SwitchCase(dataSets.DataTypeOnCPU), data));
}
void Texture2DClass::SetSubData(Vector2U pixelsOffset, Vector2U pixelsAmount, const void* data,
    TextureDataSettingsClass::DataFormatOnCPU_Enum dataFormatOnCPU, TextureDataSettingsClass::DataTypeOnCPU_Enum dataTypeOnCPU) {
    Bind();
    glSC(glTexSubImage2D(GL_TEXTURE_2D, 0, pixelsOffset[0], pixelsOffset[1], pixelsAmount[0], pixelsAmount[1],
        _DataFormatOnCPU_SwitchCase(dataFormatOnCPU), _DataTypeOnCPU_SwitchCase(dataTypeOnCPU), data));
}

void Texture2DClass::GenerateMipmaps() {
    glBindTexture(GL_TEXTURE_2D, ID);
    glSC(glGenerateMipmap(GL_TEXTURE_2D));
}

void Texture2DClass::GetData(TextureDataSettingsClass::DataFormatOnCPU_Enum dataFormat, TextureDataSettingsClass::DataTypeOnCPU_Enum dataType, void* buffer) {
#if defined Debug
    if (buffer == nullptr) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical,"buffer shouldnt be nullptr",KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_FUNCTION_WITH_INVALID_ARGUMENTS });
#endif
    glSC(glGetTexImage(GL_TEXTURE_2D, 0, _DataFormatOnCPU_SwitchCase(dataFormat), _DataTypeOnCPU_SwitchCase(dataType), buffer));
}

void Texture2DClass::sSettings_WrapTypeByX(TextureSettingsClass::WrapTypeEnum wrapTypeByX) { _UpdSettings_WrapTypeByX(wrapTypeByX); }
void Texture2DClass::sSettings_WrapTypeByY(TextureSettingsClass::WrapTypeEnum wrapTypeByY) { _UpdSettings_WrapTypeByY(wrapTypeByY); }
void Texture2DClass::sSettings_DownscalingFilt(TextureSettingsClass::DownscalingFilterFuncEnum downscalingFilt) { _UpdSettings_DownscalingFilt(downscalingFilt); }
void Texture2DClass::sSettings_UpscalingFilt(TextureSettingsClass::UpscalingFilterFuncEnum upscalingFilt) { _UpdSettings_UpscalingFilt(upscalingFilt); }
void Texture2DClass::sSettings_DepthStencilReadMode(TextureSettingsClass::DepthStencilReadModeEnum depthStencilReadMode) { _UpdSettings_DepthStencilReadMode(depthStencilReadMode); }

unsigned int Texture2DClass::gID()  {
#if defined Debug
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "TEXTURE IS DELETED, ACCESING ITS ID MAY CAUSE ERRORS", KURSAVAYAENGINE2_CORE_ERRORS::ACCESSING_IMPOSSIBLE_TO_ACCESS_INSTANCE_DATA });
#endif
    return ID;
}
void Texture2DClass::Delete() {
#if defined Debug
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "TEXTURE IS ALREADY DELETED", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_UNNECESARY_FUNCTION });
    else
#endif
        this->~Texture2DClass();
}

void Texture2DClass::Bind(unsigned int textureInd){
#if defined Debug
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "TEXTURE IS DELETED, YOU CANT BIND IT", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION });
    else
#endif
    {
        glSC(glActiveTexture(GL_TEXTURE0 + textureInd));
        glSC(glBindTexture(GL_TEXTURE_2D, ID));
    }
}
void Texture2DClass::Unbind() {
    glSC(glBindTexture(GL_TEXTURE_2D, 0));
}










Texture1DClass::CPU_DataData_Struct::CPU_DataData_Struct(Vector1U size, void* data, TextureSettingsClass settings, TextureDataSettingsClass dataSettings) :
    Size(size), Data(data), Settings(settings), DataSettings(dataSettings), Empty(false) {
    
}
Texture1DClass::CPU_DataData_Struct::CPU_DataData_Struct() {}
Texture1DClass::CPU_DataData_Struct::CPU_DataData_Struct(const CPU_DataData_Struct& toCopy) {
    memcpy(this, &toCopy, sizeof(CPU_DataData_Struct));
    if (not toCopy.Empty) {
        unsigned int dataSize = _DataTypeOnCPU_Sizeof_SwitchCase(DataSettings.DataTypeOnCPU);
        Data = new unsigned char[dataSize * Size[0]];
        memcpy(Data, toCopy.Data, _DataTypeOnCPU_Sizeof_SwitchCase(toCopy.DataSettings.DataTypeOnCPU) * Size[0]);
    }
}
Texture1DClass::CPU_DataData_Struct::CPU_DataData_Struct(const CPU_DataData_Struct* toCopy) {
	memcpy(this, toCopy, sizeof(CPU_DataData_Struct));
    toCopy->Empty = true;
}
void Texture1DClass::CPU_DataData_Struct::operator=(const CPU_DataData_Struct& toCopy) {
    if (not Empty) delete[] Data;
	memcpy(this, &toCopy, sizeof(CPU_DataData_Struct));
    if (not toCopy.Empty) {
        unsigned int dataSize = _DataTypeOnCPU_Sizeof_SwitchCase(DataSettings.DataTypeOnCPU);
        Data = new unsigned char[dataSize * Size[0]];
        memcpy(Data, toCopy.Data, _DataTypeOnCPU_Sizeof_SwitchCase(toCopy.DataSettings.DataTypeOnCPU) * Size[0]);
    }
}
void Texture1DClass::CPU_DataData_Struct::Delete() {
    this->~CPU_DataData_Struct();
}
Texture1DClass::CPU_DataData_Struct::~CPU_DataData_Struct() {
    if (not Empty) {
        delete[] Data;
        Empty = true;
    }
}



Texture2DClass::CPU_DataData_Struct::CPU_DataData_Struct(Vector2U size, void* data, TextureSettingsClass settings, TextureDataSettingsClass dataSettings) :
    Size(size), Data(data), Settings(settings), DataSettings(dataSettings), Empty(false) {
    
}
Texture2DClass::CPU_DataData_Struct::CPU_DataData_Struct() {}
Texture2DClass::CPU_DataData_Struct::CPU_DataData_Struct(const CPU_DataData_Struct& toCopy) {
    memcpy(this, &toCopy, sizeof(CPU_DataData_Struct));
    if (not toCopy.Empty) {
        unsigned int dataSize = _DataTypeOnCPU_Sizeof_SwitchCase(DataSettings.DataTypeOnCPU);
        Data = new unsigned char[dataSize * Size[0]];
        memcpy(Data, toCopy.Data, _DataTypeOnCPU_Sizeof_SwitchCase(toCopy.DataSettings.DataTypeOnCPU) * Size[0]);
    }
}
Texture2DClass::CPU_DataData_Struct::CPU_DataData_Struct(const CPU_DataData_Struct* toCopy) {
	memcpy(this, toCopy, sizeof(CPU_DataData_Struct));
    toCopy->Empty = true;
}
void Texture2DClass::CPU_DataData_Struct::operator=(const CPU_DataData_Struct& toCopy) {
    if (not Empty) delete[] Data;
	memcpy(this, &toCopy, sizeof(CPU_DataData_Struct));
    if (not toCopy.Empty) {
        unsigned int dataSize = _DataTypeOnCPU_Sizeof_SwitchCase(DataSettings.DataTypeOnCPU);
        Data = new unsigned char[dataSize * Size[0]];
        memcpy(Data, toCopy.Data, _DataTypeOnCPU_Sizeof_SwitchCase(toCopy.DataSettings.DataTypeOnCPU) * Size[0]);
    }
}
void Texture2DClass::CPU_DataData_Struct::Delete() {
    this->~CPU_DataData_Struct();
}
Texture2DClass::CPU_DataData_Struct::~CPU_DataData_Struct() {
    if (not Empty) {
        delete[] Data;
        Empty = true;
    }
}*/
