#include"Texture.h"
#include"stb_image/stb_image.h"
#include"Tools/GLDebug.h"
#include"Tools/DebuggingTools.h"
#include"Tools/ReadFromFile.h"
#include"glad/glad.h"
#include"Tools/ErrorCodes.h"


//TODO make so you can save depth/stencil components into texture, also make same in frame buffer
Texture::Texture(const char* filePath, TexParameters&& parameters) :Parameters(parameters) {
    int width, height, textureChannelsAmount;
    unsigned char* textureData = stbi_load(filePath, &width, &height, &textureChannelsAmount, 0);
    Width = width; Height = height;
    if (textureData == nullptr) {
        DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical, "FAILED TO LOAD IMAGE", KURSAVAYAENGINE2_CORE_ERRORS::FAILED_THIRD_PARTY_FUNCTION });
    }

    glSC(glGenTextures(1, &ID));
    glSC(glBindTexture(GL_TEXTURE_2D, ID));

    UpdateAllParameters();

    StorageType = (textureChannelsAmount == 4) ? TextureStorageType::RGBA : TextureStorageType::RGB;
    
    glSC(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, (textureChannelsAmount==4) ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, textureData));

    stbi_image_free(textureData);
}
Texture::Texture(unsigned int Width, unsigned int Height, TextureStorageType storageTyp, TexParameters&& parameters) :Parameters(parameters), Width(Width), Height(Height) {
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    UpdateAllParameters();

    StorageType = storageTyp;

    int glStorageInternalFormat = 0;
    int glStorageFormat = 0;
    int glStorageData = 0;
    switch (storageTyp) {
    case TextureStorageType::RGB:
    {
        glStorageInternalFormat = GL_RGB;
        glStorageFormat = GL_RGB;
        glStorageData = GL_UNSIGNED_BYTE;
        break;
    }
    case TextureStorageType::RGBA:
    {
        glStorageInternalFormat = GL_RGBA;
        glStorageFormat = GL_RGBA;
        glStorageData = GL_UNSIGNED_BYTE;
        break;
    }
    case TextureStorageType::Depth:
    {
        glStorageInternalFormat = GL_DEPTH_COMPONENT24;
        glStorageFormat = GL_DEPTH_COMPONENT;
        glStorageData = GL_FLOAT;
        break;
    }
    case TextureStorageType::DepthStencil:
    {
        glStorageInternalFormat = GL_DEPTH24_STENCIL8;
        glStorageFormat = GL_DEPTH_STENCIL;
        glStorageData = GL_UNSIGNED_INT_24_8;
        break;
    }
    case TextureStorageType::SingleChannel:
    {
        glStorageInternalFormat = GL_RED;
        glStorageFormat = GL_RED;
        glStorageData = GL_UNSIGNED_BYTE;
        break;
    }
    }

    glTexImage2D(GL_TEXTURE_2D, 0, glStorageInternalFormat, Width, Height, 0, glStorageFormat, glStorageData, nullptr);
}
Texture::Texture(unsigned int Width, unsigned int Height, unsigned int channelsAmount, const unsigned char* data, TexParameters&& parameters) :Parameters(parameters), Width(Width), Height(Height) {

    glSC(glGenTextures(1, &ID));
    glSC(glBindTexture(GL_TEXTURE_2D, ID));
    
    UpdateAllParameters();

    int glTexFormat = 0;
    switch (channelsAmount) {
    case 1:
    {
        StorageType = TextureStorageType::SingleChannel;
        glTexFormat = GL_RED;
        break;
    }
    case 3:
    {
        StorageType = TextureStorageType::RGB;
        glTexFormat = GL_RGB;
        break;
    }
    case 4:
    {
        StorageType = TextureStorageType::RGBA;
        glTexFormat = GL_RGBA;
        break;
    }
    }

    glSC(glTexImage2D(GL_TEXTURE_2D, 0, glTexFormat, Width, Height, 0, glTexFormat, GL_UNSIGNED_BYTE, data));
}
Texture::Texture(Texture&& tempTex) {
    memcpy(this, &tempTex, sizeof(tempTex));
    tempTex.Deleted = true;
}
Texture::~Texture() {
    if (not Deleted) {
        glSC(glDeleteTextures(1, &ID));
        Deleted = true;
    }
}
TextureStorageType Texture::gStorageType() const {
    return StorageType;
}

void Texture::UpdParameters_WrapTypeByX() {

    glBindTexture(GL_TEXTURE_2D, ID);

    switch (Parameters.WrapTypeByX) {
    case TextureWrapType::ClampToEdge:
    {
        glSC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        break;
    }
    case TextureWrapType::ClampToBorder:
    {
        glSC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
        break;
    }
    case TextureWrapType::MirroredRepeat:
    {
        glSC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT));
        break;
    }
    case TextureWrapType::Repeat:
    {
        glSC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        break;
    }
    case TextureWrapType::MirrorClampToEdge:
    {
        glSC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRROR_CLAMP_TO_EDGE));
        break;
    }
    }
}
void Texture::UpdParameters_WrapTypeByY() {

    glBindTexture(GL_TEXTURE_2D, ID);

    switch (Parameters.WrapTypeByX) {
    case TextureWrapType::ClampToEdge:
    {
        glSC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        break;
    }
    case TextureWrapType::ClampToBorder:
    {
        glSC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
        break;
    }
    case TextureWrapType::MirroredRepeat:
    {
        glSC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT));
        break;
    }
    case TextureWrapType::Repeat:
    {
        glSC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
        break;
    }
    case TextureWrapType::MirrorClampToEdge:
    {
        glSC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRROR_CLAMP_TO_EDGE));
        break;
    }
    }
}
void Texture::UpdParameters_DownscalingFilt() {

    glBindTexture(GL_TEXTURE_2D, ID);

    switch (Parameters.DownscalingFilt) {
    case TextureDownscalingFilterFunc::Nearest:
    {
        glSC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        break;
    }
    case TextureDownscalingFilterFunc::Linear:
    {
        glSC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        break;
    }
    case TextureDownscalingFilterFunc::NearestMipmapNearest:
    {
        glSC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST));
        break;
    }
    case TextureDownscalingFilterFunc::NearestMipmapLinear:
    {
        glSC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR));
        break;
    }
    case TextureDownscalingFilterFunc::LinearMipmapLinear:
    {
        glSC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
        break;
    }
    case TextureDownscalingFilterFunc::LinearMipmapNearest:
    {
        glSC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST));
        break;
    }
    }
}
void Texture::UpdParameters_UpscalingFilt() {

    glBindTexture(GL_TEXTURE_2D, ID);

    switch (Parameters.UpscalingFilt) {
    case TextureUpscalingFilterFunc::Nearest:
    {
        glSC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        break;
    }
    case TextureUpscalingFilterFunc::Linear:
    {
        glSC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        break;
    }
    }
}
void Texture::UpdParameters_DepthStencilReadMode() {

    glBindTexture(GL_TEXTURE_2D, ID);

    switch (Parameters.DepthStencilReadMode)
    {
    case TextureDepthStencilReadMode::Depth:
    {
        glSC(glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_STENCIL_TEXTURE_MODE, GL_DEPTH_COMPONENT));
        break;
    }
    case TextureDepthStencilReadMode::Stencil:
    {
        glSC(glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_STENCIL_TEXTURE_MODE, GL_STENCIL_INDEX));
        break;
    }
    }
}

void Texture::sParameters_WrapTypeByX(TextureWrapType wrapTypeByX) {
    Parameters.WrapTypeByX = wrapTypeByX;
    UpdParameters_WrapTypeByX();
}
void Texture::sParameters_WrapTypeByY(TextureWrapType wrapTypeByY) {
    Parameters.WrapTypeByY = wrapTypeByY;
    UpdParameters_WrapTypeByY();
}
void Texture::sParameters_DownscalingFilt(TextureDownscalingFilterFunc downscalingFilt) {
    Parameters.DownscalingFilt = downscalingFilt;
    UpdParameters_DownscalingFilt();
}
void Texture::sParameters_UpscalingFilt(TextureUpscalingFilterFunc upscalingFilt) {
    Parameters.UpscalingFilt = upscalingFilt;
    UpdParameters_UpscalingFilt();
}
void Texture::sParameters_DepthStencilReadMode(TextureDepthStencilReadMode depthStencilReadMode) {
    Parameters.DepthStencilReadMode = depthStencilReadMode;
    UpdParameters_DepthStencilReadMode();
}
void Texture::GenerateMipmaps() {
    glBindTexture(GL_TEXTURE_2D, ID);
    glSC(glGenerateMipmap(GL_TEXTURE_2D));
}
void Texture::UpdateAllParameters() {

    glBindTexture(GL_TEXTURE_2D, ID);
    
    UpdParameters_WrapTypeByX();
    UpdParameters_WrapTypeByY();

    UpdParameters_DownscalingFilt();
    UpdParameters_UpscalingFilt();

    UpdParameters_DepthStencilReadMode();

}
unsigned int Texture::gWidth() const {
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "TEXTURE IS DELETED, ACCESING ITS WIDTH MAY CAUSE ERRORS", KURSAVAYAENGINE2_CORE_ERRORS::ACCESSING_IMPOSSIBLE_TO_ACCESS_INSTANCE_DATA });
    return Width;
}
unsigned int Texture::gHeight() const {
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "TEXTURE IS DELETED, ACCESING ITS HEIGHT MAY CAUSE ERRORS", KURSAVAYAENGINE2_CORE_ERRORS::ACCESSING_IMPOSSIBLE_TO_ACCESS_INSTANCE_DATA });
    return Height;
}
Vector<2> Texture::gWidthAndHeight() const {
    return Vector<2>((float)gWidth(),(float)gHeight());
}
unsigned int Texture::gID() const {
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "TEXTURE IS DELETED, ACCESING ITS ID MAY CAUSE ERRORS", KURSAVAYAENGINE2_CORE_ERRORS::ACCESSING_IMPOSSIBLE_TO_ACCESS_INSTANCE_DATA });
    return ID;
}
void Texture::Delete() {
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "TEXTURE IS ALREADY DELETED", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_UNNECESARY_FUNCTION });
    else this->~Texture();
}
void Texture::Bind(unsigned int textureInd) const {
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "TEXTURE IS DELETED, YOU CANT BIND IT", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION });
    else {
        glSC(glActiveTexture(GL_TEXTURE0 + textureInd));
        glSC(glBindTexture(GL_TEXTURE_2D, ID));
    }
}
void Texture::Unbind() {
    glSC(glBindTexture(GL_TEXTURE_2D, 0));
}