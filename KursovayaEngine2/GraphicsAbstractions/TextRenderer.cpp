
#include"TextRenderer.h"
#include"Tools/DebuggingTools.h"
#include"GraphicsPrimitives/Renderer.h"
#include"Tools/GLDebug.h"
#include"FreeType/ft2build.h"
#include"Tools/BinarySearch.h"
#include FT_FREETYPE_H

using namespace GraphicsPrimitives;
using namespace GraphicsAbstractions;

void* TextRendererClass::FreeTypeLib = nullptr;

TextRendererClass::TextRendererClass(const wchar_t* vertexShaderDir, const wchar_t* fragmentShaderDir) :TextPreset(
    false, RenderingPresetEnumArgumentsNamespace::FaceCulling::FaceToCull::Back, RenderingPresetEnumArgumentsNamespace::FaceCulling::FaceDetermination::Clockwise,
    false, true, RenderingPresetEnumArgumentsNamespace::DepthTest::TypeOfComparison::LessOrEqual,
    false, 0, RenderingPresetEnumArgumentsNamespace::StencilTest::TypeOfComparison::Equal, 1, 255, RenderingPresetEnumArgumentsNamespace::StencilTest::Actions::Keep,
    RenderingPresetEnumArgumentsNamespace::StencilTest::Actions::Keep, RenderingPresetEnumArgumentsNamespace::StencilTest::Actions::Keep,
    true, 0, 0, 0, 0, RenderingPresetEnumArgumentsNamespace::Blending::FunctionForColor::SrcAlpha, RenderingPresetEnumArgumentsNamespace::Blending::FunctionForColor::OneMinusSrcAlpha,
    0.f, 0.f, 0.f
) {

    if (FreeTypeLib == nullptr) {
        FreeTypeLib = (void*)(new FT_Library);

        if (FT_Init_FreeType((FT_Library*)FreeTypeLib)) {
            DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical,"FAILED TO INITIALIZE FreeType LIBRARY",KURSAVAYAENGINE2_CORE_ERRORS::FAILED_TO_INITIALIZE_LIBRARY });
        }
    }

    {
        ShaderClass VS(vertexShaderDir, ShaderClass::TypesEnum::Vertex);
        VS.Compile();
        TEXT_SP.AttachShader(VS);

        ShaderClass FS(fragmentShaderDir, ShaderClass::TypesEnum::Fragment);
        FS.Compile();
        TEXT_SP.AttachShader(FS);

        TEXT_SP.LinkShaders();

        TEXT_SP.SetUniform1i("u_TextTexture", 0);
    }

    
    TEXT_VB.SetData(6 * 4 * sizeof(float), nullptr, VertexBufferClass::BufferDataUsageEnum::DynamicDraw);
    TEXT_VB.SetLayout(VertexBufferClass::BufferDataTypeEnum::Float, { 2,2 });
}

TextRendererClass::~TextRendererClass() {
    FT_Done_FreeType(*(FT_Library*)FreeTypeLib);
    delete (FT_Library*)FreeTypeLib;
}

StalkerClass TextRendererClass::AddFont(unsigned int characterSize, const char* fontDir, const wchar_t* characters) {
    Fonts.InsertByConstructor(Fonts.gLength(), characterSize, fontDir, characters);
    return StalkerClass(&Fonts, Fonts.gLength() - 1);
}

TextRendererClass::FontClass::FontClass(RespConstrFlag, const FontClass& toCopy) :
    Texture(RespConstrFlag(), toCopy.Texture), FreeTypeFace(toCopy.FreeTypeFace), Characters(toCopy.Characters) {
    toCopy.Deleted = true;
}

TextRendererClass::FontClass::~FontClass() {
    if (not Deleted) {
        Deleted = true;
        FT_Done_Face(*(FT_Face*)FreeTypeFace);
    }
}


TextRendererClass::FontClass::FontClass(unsigned int characterSize, const char* fontDir, const wchar_t* chars) :
    Texture(Vector2U(0, 0), nullptr, TextureSettingsClass{
                    TextureSettingsClass::WrapTypeEnum::ClampToBorder,TextureSettingsClass::WrapTypeEnum::ClampToBorder,
                    TextureSettingsClass::DownscalingFilterFuncEnum::Linear,TextureSettingsClass::UpscalingFilterFuncEnum::Linear,
                    TextureSettingsClass::DepthStencilReadModeEnum::Depth },
                    TextureDataSettingsClass{ TextureDataSettingsClass::DataFormatOnGPU_Enum::Red,
                    TextureDataSettingsClass::DataFormatOnCPU_Enum::Red,TextureDataSettingsClass::DataTypeOnCPU_Enum::UnsignedByte })
{
    FreeTypeFace = new FT_Face;

    if (FT_New_Face(*(FT_Library*)FreeTypeLib, fontDir, 0, (FT_Face*)FreeTypeFace)) {
        std::string errMsg;
        errMsg += "FreeType ERROR: FAILED TO OPEN FONT \"";
        errMsg += fontDir;
        errMsg += "\"";
        DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical,errMsg.c_str(),KURSAVAYAENGINE2_CORE_ERRORS::FAILED_THIRD_PARTY_FUNCTION });
    }

    FT_Set_Pixel_Sizes(*(FT_Face*)FreeTypeFace, 0, characterSize);


    unsigned int charsAmount = 0; while (chars[charsAmount] != L'\0') charsAmount++;

    Characters.reserve(charsAmount);
    std::vector<unsigned char*> buffers; buffers.reserve(charsAmount);


    unsigned int maxWidth = 0; unsigned int maxHeight = 0;
    unsigned int totalXSize = 0;

    for (unsigned int chi = 0; chi < charsAmount; chi++) {

        unsigned int unicodeInd = (unsigned int)chars[chi];


        unsigned int insertInd = (Characters.size() == 0) ? 0 : BinarySearch<CharacterClass, unsigned int>(&Characters[0], Characters.size(), unicodeInd,
            [](unsigned int v1, const CharacterClass& v2)->bool {return v2.UnicodeInd > v1; },
            [](unsigned int v1, const CharacterClass& v2)->bool {return v1 == v2.UnicodeInd; });

        if (insertInd<Characters.size() and Characters[insertInd].UnicodeInd == unicodeInd) continue;

        if (FT_Load_Char(*(FT_Face*)FreeTypeFace, unicodeInd, FT_LOAD_RENDER))
        {
            DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical,"FreeType ERROR: FAILED TO LOAD CHAR",KURSAVAYAENGINE2_CORE_ERRORS::FAILED_THIRD_PARTY_FUNCTION });
            continue;
        }
        FT_Face& face = *(FT_Face*)FreeTypeFace;

        unsigned char* newBuffer = new unsigned char[face->glyph->bitmap.width * face->glyph->bitmap.rows];
        memcpy(newBuffer, face->glyph->bitmap.buffer, sizeof(unsigned char) * face->glyph->bitmap.width * face->glyph->bitmap.rows);
        buffers.insert(buffers.begin() + insertInd, newBuffer);

        if (face->glyph->bitmap.width > maxWidth) maxWidth = face->glyph->bitmap.width;
        if (face->glyph->bitmap.rows > maxHeight) maxHeight = face->glyph->bitmap.rows;

        if (face->glyph->bitmap_top > MaxCharacterUp) MaxCharacterUp = face->glyph->bitmap_top;
        if ((int)face->glyph->bitmap.rows - face->glyph->bitmap_top > MaxCharacterDown) MaxCharacterDown = (int)face->glyph->bitmap.rows - face->glyph->bitmap_top;

        totalXSize += face->glyph->bitmap.width;

        Characters.insert(Characters.begin() + insertInd, CharacterClass{
            unicodeInd,
            Vector2U(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            Vector2I(face->glyph->bitmap_left, face->glyph->bitmap_top),
            (unsigned int)face->glyph->advance.x / 64,
            Vector2F(0,0),0
            });
    }

    totalXSize += charsAmount + 1;

    unsigned char* textureBuffer = new unsigned char[totalXSize * maxHeight] {0};
    
    {
        unsigned int xOffset = 1;
        for (unsigned int ci = 0; ci < Characters.size(); ci++) {
            CharacterClass& char_ = Characters[ci];
            char_.XOffsetInTexture = xOffset / (float)totalXSize;
            char_.SizeInTexture = Vector2F(char_.Size[0] / (float)totalXSize, char_.Size[1] / (float)maxHeight);
            xOffset += char_.Size[0] + 1;
        }
    }

    {
        unsigned int xOffset = 0;
        for (unsigned int ci = 0; ci < Characters.size(); ci++) {
            CharacterClass& char_ = Characters[ci];
            for (unsigned int x = 0; x < char_.Size[0]; x++) for (unsigned int y = 0; y < char_.Size[1]; y++) {
                textureBuffer[ci + 1 + y * totalXSize + xOffset + x] = buffers[ci][(char_.Size[1] - y - 1) * char_.Size[0] + x];
            }

            xOffset += char_.Size[0];
        }
    }

	glSC(glPixelStorei(GL_UNPACK_ALIGNMENT, 1)); // disable byte-alignment restriction

	Texture.SetData(Vector2U(totalXSize, maxHeight), textureBuffer, 
        TextureDataSettingsClass{ TextureDataSettingsClass::DataFormatOnGPU_Enum::Red,
        TextureDataSettingsClass::DataFormatOnCPU_Enum::Red,TextureDataSettingsClass::DataTypeOnCPU_Enum::UnsignedByte });

	glSC(glPixelStorei(GL_UNPACK_ALIGNMENT, 4)); // enable byte-alignment back
    

	delete[] textureBuffer;
    for (unsigned int bi = 0; bi < charsAmount; bi++) delete[] buffers[bi];


}

static constexpr unsigned int MAX_TEXT_LEN = 100000;

void TextRendererClass::RenderText(const StalkerClass& fontStalker, const wchar_t* text, Vector2F pos, Vector2F localOffset, Vector2U pixelsInTexture,
    Vector2F boxSize, float lineSizeInBox, const wchar_t* dividingSymbols) {
    
    if (boxSize[0] == 0 and boxSize[1] == 0) return;//theres nothing we can do

	TextPreset.Bind();
    TEXT_SP.Bind();
    TEXT_VA.Bind();

    FontClass& font = fontStalker.GetTarget<FontClass>();

    font.Texture.Bind(0);

    unsigned int textLen = 0;
    while (textLen != MAX_TEXT_LEN and text[textLen] != L'\0') textLen++;
    if (textLen == MAX_TEXT_LEN) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning,"Text is too big OR no end symbol was found",KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_FUNCTION_WITH_INVALID_ARGUMENTS });

    std::vector<unsigned int> charsInds; charsInds.resize(textLen);//adding +1 to mark unexisting characters, ind=0 will mean that character not found
    for (unsigned int ci = 0; ci < textLen; ci++) {
        charsInds[ci] = BinarySearch<FontClass::CharacterClass, unsigned int>(
            &font.Characters[0], font.Characters.size(), (unsigned int)text[ci],
            [](unsigned int num, const FontClass::CharacterClass& ch)->bool {return num < ch.UnicodeInd; },
            [](unsigned int num, const FontClass::CharacterClass& ch)->bool {return num == ch.UnicodeInd; });
        if (charsInds[ci] >= font.Characters.size() or font.Characters[charsInds[ci]].UnicodeInd != (unsigned int)text[ci]) charsInds[ci] = 0;
		else charsInds[ci]++;
    }


    unsigned int fullLocalXSize = 0;
    int maxUp = 0; int maxDown = 0;
    for (unsigned int ci = 0; ci < textLen; ci++) {
        if (charsInds[ci] == 0) continue;
        FontClass::CharacterClass& char_ = font.Characters[charsInds[ci] - 1];
        fullLocalXSize += char_.Advance;
        int up = char_.Bearing[1]; int down = char_.Size[1] - up;
        if (maxUp < up) maxUp = up; if (maxDown < down) maxDown = down;
    }

    unsigned int fullLocalYSize = font.MaxCharacterUp + font.MaxCharacterDown;



    float localPixelsToTexPixels = 0;
    {
        float byX = (pixelsInTexture[0] * boxSize[0]) / fullLocalXSize;
        float byY = (pixelsInTexture[1] * boxSize[1] * lineSizeInBox) / fullLocalYSize;
        if (boxSize[0] != 0 and boxSize[1] == 0) localPixelsToTexPixels = byX;
        else if (boxSize[0] == 0 and boxSize[1] != 0) localPixelsToTexPixels = byY;
        else if (boxSize[0] != 0 and boxSize[1] != 0) localPixelsToTexPixels = (byX > byY) ? byY : byX;
    }
    
    float localPixelsToTexScaleByX = localPixelsToTexPixels / pixelsInTexture[0] * 2;
    float localPixelsToTexScaleByY = localPixelsToTexPixels / pixelsInTexture[1] * 2;

    Vector2F realTextBoxSize(fullLocalXSize * localPixelsToTexScaleByX, fullLocalYSize * localPixelsToTexScaleByY);

    Vector2F scaledLocalOffset = ((localOffset + 1) / 2) * -realTextBoxSize;

	float xOffset = 0;
    for (unsigned int ci = 0; ci < textLen; ci++) {
		if (charsInds[ci] == 0) continue;
        FontClass::CharacterClass& char_ = font.Characters[charsInds[ci] - 1];
        
        Vector2F lbp(pos[0] + xOffset + char_.Bearing[0] * localPixelsToTexScaleByX, 
            pos[1] + (font.MaxCharacterDown - ((int)char_.Size[1] - char_.Bearing[1])) * localPixelsToTexScaleByY);
        Vector2F rbp(lbp[0] + char_.Size[0] * localPixelsToTexScaleByX, lbp[1]);
        Vector2F ltp(lbp[0], lbp[1] + char_.Size[1] * localPixelsToTexScaleByY);
        Vector2F rtp(rbp[0], ltp[1]);

        Vector2F lbc(char_.XOffsetInTexture, 0);
        Vector2F rbc(lbc[0] + char_.SizeInTexture[0], 0);
        Vector2F ltc(lbc[0], char_.SizeInTexture[1]);
        Vector2F rtc(rbc[0], ltc[1]);


        lbp[0] += scaledLocalOffset[0];
        lbp[1] += scaledLocalOffset[1];
		rbp[0] += scaledLocalOffset[0];
        rbp[1] += scaledLocalOffset[1];
		ltp[0] += scaledLocalOffset[0];
        ltp[1] += scaledLocalOffset[1];
		rtp[0] += scaledLocalOffset[0];
        rtp[1] += scaledLocalOffset[1];


        float data[4 * 6] = {
            lbp[0],lbp[1],lbc[0],lbc[1],
            rbp[0],rbp[1],rbc[0],rbc[1],
            rtp[0],rtp[1],rtc[0],rtc[1],
            
            rtp[0],rtp[1],rtc[0],rtc[1],
            ltp[0],ltp[1],ltc[0],ltc[1],
            lbp[0],lbp[1],lbc[0],lbc[1],
        };

		TEXT_VB.SetSubData(0, 6 * 4 * sizeof(float), data);
        RendererNamespace::DrawArrays(RendererNamespace::PrimitivesEnum::Triangles, 0, 6);


        xOffset += char_.Advance * localPixelsToTexScaleByX;
   
    }
}







