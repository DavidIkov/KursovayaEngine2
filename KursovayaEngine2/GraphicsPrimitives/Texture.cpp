#include"Texture.h"
#include"stb_image/stb_image.h"
#include"Tools/GLDebug.h"
#include"Tools/DebuggingTools.h"
#include"Tools/ReadFromFile.h"
#include"glad/glad.h"
#include"Tools/ErrorCodes.h"
//TODO make so you can save depth/stencil components into texture, also make same in frame buffer
Texture::Texture(const char* filePath) {
    int width, height, textureChannelsAmount;
    unsigned char* textureData = stbi_load(filePath, &width, &height, &textureChannelsAmount, 0);
    if (textureData == nullptr) {
        DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical, "FAILED TO LOAD IMAGE", KURSAVAYAENGINE2_CORE_ERRORS::FAILED_THIRD_PARTY_FUNCTION });
    }

    glSC(glGenTextures(1, &ID));
    glSC(glBindTexture(GL_TEXTURE_2D, ID));

    glSC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    glSC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

    glSC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    glSC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    StorageType = (textureChannelsAmount == 4) ? TextureStorageType::RGBA : TextureStorageType::RGB;

    glSC(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, (textureChannelsAmount==4) ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, textureData));
    glSC(glGenerateMipmap(GL_TEXTURE_2D));

    stbi_image_free(textureData);
}
Texture::Texture(unsigned int Width, unsigned int Height, TextureStorageType storageTyp) {
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

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
    }

    glTexImage2D(GL_TEXTURE_2D, 0, glStorageInternalFormat, Width, Height, 0, glStorageFormat, glStorageData, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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
void Texture::gWidthAndHeight(unsigned int& width, unsigned int& height) const {
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "TEXTURE IS DELETED, ACCESING ITS WIDTH AND HEIGHT MAY CAUSE ERRORS", KURSAVAYAENGINE2_CORE_ERRORS::ACCESSING_IMPOSSIBLE_TO_ACCESS_INSTANCE_DATA });
    width = Width;
    height = Height;
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