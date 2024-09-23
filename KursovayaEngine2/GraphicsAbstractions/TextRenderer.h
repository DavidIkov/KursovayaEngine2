#pragma once
#include"DLL.h"
#include"Maths/Vector.h"
#include"GraphicsPrimitives/Texture.h"
#include"GraphicsPrimitives/ShaderProgram.h"
#include"GraphicsPrimitives/RenderingPreset.h"
#include"GraphicsPrimitives/VertexArray.h"
#include"GraphicsPrimitives/VertexBuffer.h"
#include"Tools/DynArr.h"

namespace GraphicsAbstractions {

    class TextRendererClass {

        GraphicsPrimitives::ShaderProgramClass TEXT_SP;
        GraphicsPrimitives::RenderingPresetClass TextPreset;
        GraphicsPrimitives::VertexArrayClass TEXT_VA;
        GraphicsPrimitives::VertexBufferClass TEXT_VB;

        struct FontClass {

            struct CharacterClass {
                unsigned int UnicodeInd;
                Vector2U Size;       // Size of glyph
                Vector2I Bearing;    // Offset from baseline to left/top of glyph
                unsigned int Advance; // Offset to advance to next glyph
                Vector2F SizeInTexture;
                float XOffsetInTexture;
            };
            mutable bool Deleted = false;
            GraphicsPrimitives::Texture2DClass Texture;
            void* FreeTypeFace;//FT_Face
            std::vector<CharacterClass> Characters;
            int MaxCharacterUp = 0; int MaxCharacterDown = 0;
            FontClass(unsigned int characterSize, const char* fontDir, const wchar_t* chars);
            FontClass(FontClass* toCopy);
            ~FontClass();
        };

        static void* FreeTypeLib;//FT_Library, nullptr

        DynArr<FontClass> Fonts;

    public:

        DLLTREATMENT TextRendererClass(const wchar_t* vertexShaderDir, const wchar_t* fragmentShaderDir);
        DLLTREATMENT ~TextRendererClass();
        DLLTREATMENT StalkerClass AddFont(unsigned int characterSize, const char* fontDir, const wchar_t* characters);

        //pixelsInTexture is amount of pixels in the texture you are rendering this text to
        //if scalingType is ByX/ByY you can keep y/x coordinate in size as 0
        //TODO add functionality for "dividingSymbols" and make so text will be separated in a few lines if possible
        DLLTREATMENT void RenderText(const StalkerClass& fontStaker, const wchar_t* text, Vector2F pos, Vector2F localOffset, Vector2U pixelsInTexture,
            Vector2F boxSize, float lineSizeInBox, const wchar_t* dividingSymbols = nullptr);


    };
}
