#include"Texture.h"
#include"stb_image/stb_image.h"
#include"Tools/GLDebug.h"
#include"Tools/DebuggingTools.h"
#include"Tools/ReadFromFile.h"
#include"glad/glad.h"

void TextureClass::_Constructor(unsigned int width, unsigned int height, const void* data, const DataSettingsClass& dataSets) {

    glSC(glGenTextures(1, &ID));
    glSC(glBindTexture(GL_TEXTURE_2D, ID));

    unsigned int gl_internalFormat = 0; unsigned int gl_format = 0; unsigned int gl_type = 0;
    switch (dataSets.DataFormatOnGPU) {
    case DataSettingsClass::DataFormatOnGPU_Enum::DepthComponent: gl_internalFormat = GL_DEPTH_COMPONENT24; break;
    case DataSettingsClass::DataFormatOnGPU_Enum::DepthStencil: gl_internalFormat = GL_DEPTH24_STENCIL8; break;
    case DataSettingsClass::DataFormatOnGPU_Enum::Red: gl_internalFormat = GL_RED; break;
    case DataSettingsClass::DataFormatOnGPU_Enum::RG: gl_internalFormat = GL_RG; break;
    case DataSettingsClass::DataFormatOnGPU_Enum::RGB: gl_internalFormat = GL_RGB; break;
    case DataSettingsClass::DataFormatOnGPU_Enum::RGBA: gl_internalFormat = GL_RGBA; break;
    }
    switch (dataSets.DataFormatOnCPU) {
    case DataSettingsClass::DataFormatOnCPU_Enum::Red: gl_format = GL_RED; break;
    case DataSettingsClass::DataFormatOnCPU_Enum::RG: gl_format = GL_RG; break;
    case DataSettingsClass::DataFormatOnCPU_Enum::RGB: gl_format = GL_RGB; break;
    case DataSettingsClass::DataFormatOnCPU_Enum::BGR: gl_format = GL_BGR; break;
    case DataSettingsClass::DataFormatOnCPU_Enum::RGBA: gl_format = GL_RGBA; break;
    case DataSettingsClass::DataFormatOnCPU_Enum::BGRA: gl_format = GL_BGRA; break;
    case DataSettingsClass::DataFormatOnCPU_Enum::RedInteger: gl_format = GL_RED_INTEGER; break;
    case DataSettingsClass::DataFormatOnCPU_Enum::RG_Integer: gl_format = GL_RG_INTEGER; break;
    case DataSettingsClass::DataFormatOnCPU_Enum::RGB_Integer: gl_format = GL_RGB_INTEGER; break;
    case DataSettingsClass::DataFormatOnCPU_Enum::BGR_Integer: gl_format = GL_BGR_INTEGER; break;
    case DataSettingsClass::DataFormatOnCPU_Enum::RGBA_Integer: gl_format = GL_RGBA_INTEGER; break;
    case DataSettingsClass::DataFormatOnCPU_Enum::BGRA_Integer: gl_format = GL_BGRA_INTEGER; break;
    case DataSettingsClass::DataFormatOnCPU_Enum::StencilIndex: gl_format = GL_STENCIL_INDEX; break;
    case DataSettingsClass::DataFormatOnCPU_Enum::DepthComponent: gl_format = GL_DEPTH_COMPONENT; break;
    case DataSettingsClass::DataFormatOnCPU_Enum::DepthStencil: gl_format = GL_DEPTH_STENCIL; break;
    }
    switch (dataSets.DataTypeOnCPU) {
    case DataSettingsClass::DataTypeOnCPU_Enum::UnsignedByte: gl_type = GL_UNSIGNED_BYTE; break;
    case DataSettingsClass::DataTypeOnCPU_Enum::Byte: gl_type = GL_BYTE; break;
    case DataSettingsClass::DataTypeOnCPU_Enum::UnsignedShort: gl_type = GL_UNSIGNED_SHORT; break;
    case DataSettingsClass::DataTypeOnCPU_Enum::Short: gl_type = GL_SHORT; break;
    case DataSettingsClass::DataTypeOnCPU_Enum::UnsignedInt: gl_type = GL_UNSIGNED_INT; break;
    case DataSettingsClass::DataTypeOnCPU_Enum::Int: gl_type = GL_INT; break;
    case DataSettingsClass::DataTypeOnCPU_Enum::Float: gl_type = GL_FLOAT; break;
    case DataSettingsClass::DataTypeOnCPU_Enum::UnsignedInt_24_8: gl_type = GL_UNSIGNED_INT_24_8; break;
    }
    glSC(glTexImage2D(GL_TEXTURE_2D, 0, gl_internalFormat, width, height, 0, gl_format, gl_type, data));
}
TextureClass::TextureClass(const char* filePath, const SettingsClass& sets) {

    int width, height, textureChannelsAmount;
    unsigned char* textureData = stbi_load(filePath, &width, &height, &textureChannelsAmount, 0);
    if (textureData == nullptr) {
        DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical, "FAILED TO LOAD IMAGE", KURSAVAYAENGINE2_CORE_ERRORS::FAILED_THIRD_PARTY_FUNCTION });
    }
    DataSettingsClass dataSettings;
    dataSettings.DataFormatOnGPU = DataSettingsClass::DataFormatOnGPU_Enum::RGBA;
    dataSettings.DataFormatOnCPU = (textureChannelsAmount == 4) ? DataSettingsClass::DataFormatOnCPU_Enum::RGBA : DataSettingsClass::DataFormatOnCPU_Enum::RGB;
    dataSettings.DataTypeOnCPU = DataSettingsClass::DataTypeOnCPU_Enum::UnsignedByte;

    _Constructor(width, height, textureData, dataSettings);
    _UpdateSettings(sets);

    stbi_image_free(textureData);
}
TextureClass::TextureClass(unsigned int width, unsigned int height, const void* data, const SettingsClass& sets, const DataSettingsClass& dataSets){
    _Constructor(width, height, data, dataSets);
    _UpdateSettings(sets);
}
TextureClass::TextureClass(const TextureClass* toCopy) {
    memcpy(this, toCopy, sizeof(TextureClass));
    toCopy->Deleted = true;
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

void TextureClass::_UpdSettings_WrapTypeByX(SettingsClass::WrapTypeEnum wrapTyp) {

    Bind();

    unsigned int gl_wrapTyp = 0;
    switch (wrapTyp) {
    case SettingsClass::WrapTypeEnum::ClampToEdge: gl_wrapTyp = GL_CLAMP_TO_EDGE; break;
    case SettingsClass::WrapTypeEnum::ClampToBorder: gl_wrapTyp = GL_CLAMP_TO_BORDER; break;
    case SettingsClass::WrapTypeEnum::MirroredRepeat: gl_wrapTyp = GL_MIRRORED_REPEAT; break;
    case SettingsClass::WrapTypeEnum::Repeat: gl_wrapTyp = GL_REPEAT; break;
    case SettingsClass::WrapTypeEnum::MirrorClampToEdge: gl_wrapTyp = GL_MIRROR_CLAMP_TO_EDGE; break;
    }

    glSC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, gl_wrapTyp));
}
void TextureClass::_UpdSettings_WrapTypeByY(SettingsClass::WrapTypeEnum wrapTyp) {

    Bind();

    unsigned int gl_wrapTyp = 0;
    switch (wrapTyp) {
    case SettingsClass::WrapTypeEnum::ClampToEdge: gl_wrapTyp = GL_CLAMP_TO_EDGE; break;
    case SettingsClass::WrapTypeEnum::ClampToBorder: gl_wrapTyp = GL_CLAMP_TO_BORDER; break;
    case SettingsClass::WrapTypeEnum::MirroredRepeat: gl_wrapTyp = GL_MIRRORED_REPEAT; break;
    case SettingsClass::WrapTypeEnum::Repeat: gl_wrapTyp = GL_REPEAT; break;
    case SettingsClass::WrapTypeEnum::MirrorClampToEdge: gl_wrapTyp = GL_MIRROR_CLAMP_TO_EDGE; break;
    }

    glSC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, gl_wrapTyp));
}
void TextureClass::_UpdSettings_DownscalingFilt(SettingsClass::DownscalingFilterFuncEnum filt) {

    Bind();

    unsigned int gl_filt = 0;
    switch (filt) {
    case SettingsClass::DownscalingFilterFuncEnum::Nearest: gl_filt = GL_NEAREST; break;
    case SettingsClass::DownscalingFilterFuncEnum::Linear: gl_filt = GL_LINEAR; break;
    case SettingsClass::DownscalingFilterFuncEnum::NearestMipmapNearest: gl_filt = GL_NEAREST_MIPMAP_NEAREST; break;
    case SettingsClass::DownscalingFilterFuncEnum::NearestMipmapLinear: gl_filt = GL_NEAREST_MIPMAP_LINEAR; break;
    case SettingsClass::DownscalingFilterFuncEnum::LinearMipmapLinear: gl_filt = GL_LINEAR_MIPMAP_LINEAR; break;
    case SettingsClass::DownscalingFilterFuncEnum::LinearMipmapNearest: gl_filt = GL_LINEAR_MIPMAP_NEAREST; break;
    }
    glSC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gl_filt));
}
void TextureClass::_UpdSettings_UpscalingFilt(SettingsClass::UpscalingFilterFuncEnum filt) {

    Bind();

    unsigned int gl_filt = 0;
    switch (filt) {
    case SettingsClass::UpscalingFilterFuncEnum::Nearest: gl_filt = GL_NEAREST; break;
    case SettingsClass::UpscalingFilterFuncEnum::Linear: gl_filt = GL_LINEAR; break;
    }
    glSC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gl_filt));
}
void TextureClass::_UpdSettings_DepthStencilReadMode(SettingsClass::DepthStencilReadModeEnum readMode) {

    Bind();

    unsigned int gl_readMode = 0;
    switch (readMode) {
    case SettingsClass::DepthStencilReadModeEnum::Depth: gl_readMode = GL_DEPTH_COMPONENT; break;
    case SettingsClass::DepthStencilReadModeEnum::Stencil: gl_readMode = GL_STENCIL_INDEX; break;
    }
    glSC(glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_STENCIL_TEXTURE_MODE, gl_readMode));
}

void TextureClass::sSettings_WrapTypeByX(SettingsClass::WrapTypeEnum wrapTypeByX) { _UpdSettings_WrapTypeByX(wrapTypeByX); }
void TextureClass::sSettings_WrapTypeByY(SettingsClass::WrapTypeEnum wrapTypeByY) { _UpdSettings_WrapTypeByY(wrapTypeByY); }
void TextureClass::sSettings_DownscalingFilt(SettingsClass::DownscalingFilterFuncEnum downscalingFilt) { _UpdSettings_DownscalingFilt(downscalingFilt); }
void TextureClass::sSettings_UpscalingFilt(SettingsClass::UpscalingFilterFuncEnum upscalingFilt) { _UpdSettings_UpscalingFilt(upscalingFilt); }
void TextureClass::sSettings_DepthStencilReadMode(SettingsClass::DepthStencilReadModeEnum depthStencilReadMode) { _UpdSettings_DepthStencilReadMode(depthStencilReadMode); }
void TextureClass::GenerateMipmaps() {
    glBindTexture(GL_TEXTURE_2D, ID);
    glSC(glGenerateMipmap(GL_TEXTURE_2D));
}
void TextureClass::_UpdateSettings(const SettingsClass& sets) {
    
    _UpdSettings_WrapTypeByX(sets.WrapTypeByX);
    _UpdSettings_WrapTypeByY(sets.WrapTypeByY);
    _UpdSettings_DownscalingFilt(sets.DownscalingFilt);
    _UpdSettings_UpscalingFilt(sets.UpscalingFilt);
    _UpdSettings_DepthStencilReadMode(sets.DepthStencilReadMode);

}
unsigned int TextureClass::gID() const {
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "TEXTURE IS DELETED, ACCESING ITS ID MAY CAUSE ERRORS", KURSAVAYAENGINE2_CORE_ERRORS::ACCESSING_IMPOSSIBLE_TO_ACCESS_INSTANCE_DATA });
    return ID;
}
void TextureClass::Delete() {
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "TEXTURE IS ALREADY DELETED", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_UNNECESARY_FUNCTION });
    else this->~TextureClass();
}
void TextureClass::Bind(unsigned int textureInd) const {
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "TEXTURE IS DELETED, YOU CANT BIND IT", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION });
    else {
        glSC(glActiveTexture(GL_TEXTURE0 + textureInd));
        glSC(glBindTexture(GL_TEXTURE_2D, ID));
    }
}
void TextureClass::Unbind() {
    glSC(glBindTexture(GL_TEXTURE_2D, 0));
}