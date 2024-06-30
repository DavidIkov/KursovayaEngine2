#include"Texture.h"
#include"stb_image/stb_image.h"
#include"Tools/GLDebug.h"
#include"Tools/DebuggingTools.h"
#include"Tools/ReadFromFile.h"
#include"glad/glad.h"
#include"Tools/ErrorCodes.h"

Texture::Texture(const char* filePath) {
    int width, height, textureChannelsAmount;
    unsigned char* textureData = stbi_load(filePath, &width, &height, &textureChannelsAmount, 0);
    if (textureData == nullptr) {
        DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical, "FAILED TO LOAD IMAGE", KURSAVAYAENGINE2_CORE_ERRORS::STB_IMAGE_FAILED_TO_READ_FILE });
    }

    glSC(glGenTextures(1, &ID));
    glSC(glBindTexture(GL_TEXTURE_2D, ID));

    glSC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    glSC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

    glSC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    glSC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    glSC(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, (textureChannelsAmount==4) ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, textureData));
    glSC(glGenerateMipmap(GL_TEXTURE_2D));

    stbi_image_free(textureData);
}
Texture::Texture(unsigned int Width, unsigned int Height) {
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
Texture::~Texture() {
    if (not Deleted) {
        glSC(glDeleteTextures(1, &ID));
        Deleted = true;
    }
}
void Texture::gWidthAndHeight(unsigned int& width, unsigned int& height) const {
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Managable, "TEXTURE IS DELETED, ACCESING ITS WIDTH AND HEIGHT MAY CAUSE ERRORS", KURSAVAYAENGINE2_CORE_ERRORS::ACCESSING_DELETED_TEXTURE_DATA });
    width = Width;
    height = Height;
}
unsigned int Texture::gID() const {
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Managable, "TEXTURE IS DELETED, ACCESING ITS ID MAY CAUSE ERRORS", KURSAVAYAENGINE2_CORE_ERRORS::ACCESSING_DELETED_TEXTURE_DATA });
    return ID;
}
void Texture::Delete() {
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Managable, "TEXTURE IS ALREADY DELETED", KURSAVAYAENGINE2_CORE_ERRORS::ATTEMPING_TO_DELETE_ALREADY_DELETED_TEXTURE });
    else this->~Texture();
}
void Texture::Bind(unsigned int textureInd) const {
    if (Deleted) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Managable, "TEXTURE IS DELETED, YOU CANT BIND IT", KURSAVAYAENGINE2_CORE_ERRORS::ATTEMPING_TO_BIND_ALREADY_DELETED_TEXTURE });
    else {
        glSC(glActiveTexture(GL_TEXTURE0 + textureInd));
        glSC(glBindTexture(GL_TEXTURE_2D, ID));
    }
}
void Texture::Unbind() {
    glSC(glBindTexture(GL_TEXTURE_2D, 0));
}