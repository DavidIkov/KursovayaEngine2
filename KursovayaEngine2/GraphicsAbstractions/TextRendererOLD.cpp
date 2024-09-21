#include"TextRenderer.h"
#include"Tools/ErrorCodes.h"
#include"Tools/DebuggingTools.h"
#include<string>
#include"GraphicsPrimitives/Renderer.h"
#include"Tools/GLDebug.h"
#include"FreeType/ft2build.h"
#include FT_FREETYPE_H


bool TextRenderer::First = false;

TextRenderer::CharacterClass::CharacterClass(Texture2DClass&& tex, unsigned int unicodeInd, Vector2I&& size, Vector2I&& bearing, unsigned int advance)
    :Tex(&tex), UnicodeInd(unicodeInd), Size(std::move(size)), Bearing(std::move(bearing)), Advance(advance) { }
//TextRenderer::CharacterClass& TextRenderer::CharacterClass::operator=(const CharacterClass& copy) {
//
//}



TextRenderer::CharacterClass::CharacterClass(const CharacterClass* toCopy) :
    Tex(&toCopy->Tex), UnicodeInd(toCopy->UnicodeInd), Size(toCopy->Size), Bearing(toCopy->Bearing), Advance(toCopy->Advance) {

}

TextRenderer::TextRenderer(const wchar_t* vShaderDir, const wchar_t* fShaderDir):
    TextPreset(
    false, RenderingPresetEnumArguments::FaceCulling::FaceToCull::Back, RenderingPresetEnumArguments::FaceCulling::FaceDetermination::Clockwise,
    false, true, RenderingPresetEnumArguments::DepthTest::TypeOfComparison::LessOrEqual,
    false, 0, RenderingPresetEnumArguments::StencilTest::TypeOfComparison::Equal, 1, 255, RenderingPresetEnumArguments::StencilTest::Actions::Keep,
    RenderingPresetEnumArguments::StencilTest::Actions::Keep, RenderingPresetEnumArguments::StencilTest::Actions::Keep,
    true, 0, 0, 0, 0, RenderingPresetEnumArguments::Blending::FunctionForColor::SrcAlpha, RenderingPresetEnumArguments::Blending::FunctionForColor::OneMinusSrcAlpha,
    0.f, 0.f, 0.f
){

    FreeTypeLib = (void*)(new FT_Library);

    if (FT_Init_FreeType((FT_Library*)FreeTypeLib)) {
        DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical,"FAILED TO INITIALIZE FreeType LIBRARY",KURSAVAYAENGINE2_CORE_ERRORS::FAILED_TO_INITIALIZE_LIBRARY });
    }


    {
        Shader VS(vShaderDir, Shader::TypesEnum::Vertex);
        VS.Compile();
        TEXT_SP.AttachShader(VS);

        Shader FS(fShaderDir, Shader::TypesEnum::Fragment);
        FS.Compile();
        TEXT_SP.AttachShader(FS);

        TEXT_SP.LinkShaders();

        TEXT_SP.SetUniform1i("u_TextTexture", 0);
    }

    TEXT_VB.SetData(6 * 4 * sizeof(float), nullptr, VertexBuffer::BufferDataUsage::DynamicDraw);
    TEXT_VB.SetLayout(VertexBuffer::BufferDataType::Float, { 2,2 });
}
TextRenderer::FontClass::FontClass() { }
TextRenderer::FontClass::FontClass(const FontClass* toCopy) {
    toCopy->Deleted = true;
    memcpy(this, toCopy, sizeof(FontClass));
}
TextRenderer::FontClass::~FontClass() {
    if (not Deleted) {
        FT_Done_Face(*(FT_Face*)FreeTypeFace);
        delete (FT_Face*)FreeTypeFace;
    }
}

TextRenderer::~TextRenderer() {
    FT_Done_FreeType(*(FT_Library*)FreeTypeLib);
    delete (FT_Library*)FreeTypeLib;
}
std::string TextRenderer::LoadFont(const char* fontDir, unsigned int charsSize) {

    FontClass font;
    font.FreeTypeFace = new FT_Face;

    if (FT_New_Face(*(FT_Library*)FreeTypeLib, fontDir, 0, (FT_Face*)font.FreeTypeFace)) {
        std::string errMsg;
        errMsg += "FreeType ERROR: FAILED TO OPEN FONT \"";
        errMsg += fontDir;
        errMsg += "\"";
        DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical,errMsg.c_str(),KURSAVAYAENGINE2_CORE_ERRORS::FAILED_THIRD_PARTY_FUNCTION });
    }

    FT_Set_Pixel_Sizes(*(FT_Face*)font.FreeTypeFace, 0, charsSize);//charsSize=48

    {//name
        std::string dir(fontDir);
        size_t lastSlash = dir.find_last_of('/');
        size_t dot = dir.find_first_of('.');
        if (lastSlash == dir.npos) font.FontName = dir.substr(0, dot);
        else font.FontName = dir.substr(lastSlash + 1, dot - lastSlash - 1);
    }

    Fonts.InsertByResponsibilityConstructor(Fonts.gLength(), font);

    return font.FontName;
}
void TextRenderer::LoadCharacters(const std::string& fontName, const wchar_t* chars) {
    for (unsigned int fi = 0; fi < Fonts.gLength(); fi++) {
        if (Fonts[fi].FontName == fontName) {

            FontClass& font = Fonts[fi];
            
            unsigned int charsAmount = 0; while (chars[charsAmount] != L'\0') charsAmount++;
            
            font.Characters.Resize(font.Characters.gLength() + charsAmount);

            glSC(glPixelStorei(GL_UNPACK_ALIGNMENT, 1)); // disable byte-alignment restriction

            for (unsigned int ci = 0; ci < charsAmount; ci++) {

                unsigned int unicodeInd = (unsigned int)chars[ci];


                unsigned int insertInd = font.Characters.gLength();
                for (unsigned int cci = 0; cci < font.Characters.gLength(); cci++) {
                    if (font.Characters[cci].UnicodeInd == unicodeInd) { goto continueStop; }
                    else if (font.Characters[cci].UnicodeInd > unicodeInd) { insertInd = cci; break; }
                }
                goto skipContinue;
            continueStop:
                continue;
            skipContinue:

                

                

                
                if (FT_Load_Char(*(FT_Face*)font.FreeTypeFace, unicodeInd, FT_LOAD_RENDER))
                {
                    DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical,"FreeType ERROR: FAILED TO LOAD CHAR",KURSAVAYAENGINE2_CORE_ERRORS::FAILED_THIRD_PARTY_FUNCTION });
                    continue;
                }
                FT_Face& face = *(FT_Face*)font.FreeTypeFace;

                
                font.Characters.InsertByConstructor(insertInd,
                    Texture2DClass{ Vector2U(face->glyph->bitmap.width, face->glyph->bitmap.rows) ,face->glyph->bitmap.buffer,
                    TextureSettingsClass{
                    TextureSettingsClass::WrapTypeEnum::ClampToEdge,TextureSettingsClass::WrapTypeEnum::ClampToEdge,
                    TextureSettingsClass::DownscalingFilterFuncEnum::Linear,TextureSettingsClass::UpscalingFilterFuncEnum::Linear,
                    TextureSettingsClass::DepthStencilReadModeEnum::Depth},
                    TextureDataSettingsClass{TextureDataSettingsClass::DataFormatOnGPU_Enum::Red,
                    TextureDataSettingsClass::DataFormatOnCPU_Enum::Red,TextureDataSettingsClass::DataTypeOnCPU_Enum::UnsignedByte} }, unicodeInd,
                    Vector2I(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    Vector2I(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    (unsigned int)face->glyph->advance.x / 64
                );
            }

            glSC(glPixelStorei(GL_UNPACK_ALIGNMENT, 4));

            break;
        }
    }
}

void TextRenderer::DrawText(
    const std::string& fontName, std::wstring text,
    unsigned int screenWidth, unsigned int screenHeight,
    float textScale,
    float posX, float posY,
    float localOffsetX, float localOffsetY,
    float xSize, ClampingFuncs xClampFunc,
    float ySize, bool ignoreBottomPartOfSymbol, ClampingFuncs yClampFunc
) {
    TextPreset.Bind();
    TEXT_SP.Bind();
    TEXT_SP.SetUniform2f("u_PosOnScreen", 0.f, 0.f);

    TEXT_VA.Bind();

    float totalXSize = 0;
    
    float maxYUp = 0;
    float maxYDown = 0;

    FontClass* font = nullptr;
    for (unsigned int fi = 0; fi < Fonts.gLength(); fi++) if (Fonts[fi].FontName == fontName) { font = &Fonts[fi]; break; }

    std::vector<CharacterClass*> chars; chars.reserve(text.size());

    for (unsigned int i = 0; i < text.size(); i++) {
        CharacterClass* charData = nullptr;
        {
            unsigned int unicodeInd = (unsigned int)text[i];
            for (unsigned int ci = 0; ci < font->Characters.gLength(); ci++) 
                if (font->Characters[ci].UnicodeInd == unicodeInd) { charData = &font->Characters[ci]; break; }
        }

        if (charData == nullptr) {
            std::string errMsg("Could not display symbol " + std::to_string((unsigned int)text[i]));
            DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning,errMsg.c_str(),KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_FUNCTION_WITH_INVALID_ARGUMENTS});
            continue;
        }

        chars.push_back(charData);
        
        totalXSize += charData->Advance;
        float charYSize = (float)(charData->Bearing[1] + (ignoreBottomPartOfSymbol ? 0 : (charData->Size[1] - charData->Bearing[1])));
        
        if (not ignoreBottomPartOfSymbol and maxYDown < charData->Size[1] - charData->Bearing[1]) maxYDown = (float)(charData->Size[1] - charData->Bearing[1]);
        if (maxYUp < charData->Bearing[1]) maxYUp = (float)charData->Bearing[1];

    }
    float totalYSize = maxYUp + maxYDown;
    totalXSize *= textScale / (float)screenWidth;
    totalYSize *= textScale / (float)screenHeight;
    maxYDown *= textScale / (float)screenHeight;
    {
        float mult = 1;
        bool xpass = xClampFunc == ClampingFuncs::AlwaysScaleTillClamp or xClampFunc == ClampingFuncs::ClampIfBigger and totalXSize > xSize;
        bool ypass = yClampFunc == ClampingFuncs::AlwaysScaleTillClamp or yClampFunc == ClampingFuncs::ClampIfBigger and totalYSize > ySize;
        if (xClampFunc != ClampingFuncs::None and yClampFunc == ClampingFuncs::None) {
            if (xpass) mult = xSize / totalXSize;
        }
        else if (xClampFunc == ClampingFuncs::None and yClampFunc != ClampingFuncs::None) {
            if (ypass)  mult = ySize / totalYSize;
        }
        else if (xClampFunc != ClampingFuncs::None and yClampFunc != ClampingFuncs::None) {
            if (xpass and not ypass) mult = xSize / totalXSize;
            else if (not xpass and ypass) mult = ySize / totalYSize;
            else if (xpass and ypass) mult = fminf(xSize / totalXSize, ySize / totalYSize);
        }
        textScale *= mult;
        totalXSize *= mult;
        totalYSize *= mult;
        maxYDown *= mult;

    }

    localOffsetX = (localOffsetX / 2 + 0.5f) * totalXSize * 2;
    localOffsetY = (localOffsetY / 2 + 0.5f) * totalYSize * 2;

    for (unsigned int i = 0; i < chars.size(); i++) {
        CharacterClass* charData = chars[i];

        charData->Tex.Bind(0);
        float xpos = posX + charData->Bearing[0] * textScale / screenWidth * 2 - localOffsetX;
        float ypos = posY + (charData->Bearing[1] - charData->Size[1]) * textScale / screenHeight * 2 - localOffsetY + maxYDown * 2;

        float w = charData->Size[0] * textScale / screenWidth * 2;
        float h = charData->Size[1] * textScale / screenHeight * 2;
        // update VBO for each character
        float vertices[6 * 4] = {
             xpos,     ypos + h,   0.0f, 0.0f ,
             xpos,     ypos,       0.0f, 1.0f ,
             xpos + w, ypos,       1.0f, 1.0f ,

             xpos,     ypos + h,   0.0f, 0.0f ,
             xpos + w, ypos,       1.0f, 1.0f ,
             xpos + w, ypos + h,   1.0f, 0.0f
        };

        TEXT_VB.SetSubData(0, 6 * 4 * sizeof(float), vertices);
        Renderer::DrawArrays(Renderer::PrimitivesEnum::Triangles, 0, 6);
        posX += (charData->Advance) * textScale / screenWidth * 2;
    }

#undef getCharData
}
