
#include"TextRenderer.h"
#include"Graphics/Primitives/Renderer.h"
#include"Tools/GLDebug.h"
#include"FreeType/ft2build.h"
#include FT_FREETYPE_H
#include"Tools/BinarySearch.h"

using namespace KE2;
namespace GP = Graphics::Primitives;
namespace GA = Graphics::Abstractions;

void* GA::TextRendererClass::FreeTypeLib = nullptr;
unsigned short GA::TextRendererClass::AmountOfInstances = 0;

GA::TextRendererClass::TextRendererClass(const wchar_t* vertexShaderDir, const wchar_t* fragmentShaderDir) :TextPreset(
    false, GP::RenderingPresetEnumArgumentsNamespace::FaceCulling::FaceToCull::Back, GP::RenderingPresetEnumArgumentsNamespace::FaceCulling::FaceDetermination::Clockwise,
    false, true, GP::RenderingPresetEnumArgumentsNamespace::DepthTest::TypeOfComparison::LessOrEqual,
    false, 0, GP::RenderingPresetEnumArgumentsNamespace::StencilTest::TypeOfComparison::Equal, 1, 255, GP::RenderingPresetEnumArgumentsNamespace::StencilTest::Actions::Keep,
    GP::RenderingPresetEnumArgumentsNamespace::StencilTest::Actions::Keep, GP::RenderingPresetEnumArgumentsNamespace::StencilTest::Actions::Keep,
    true, 0, 0, 0, 0, GP::RenderingPresetEnumArgumentsNamespace::Blending::FunctionForColor::SrcAlpha, GP::RenderingPresetEnumArgumentsNamespace::Blending::FunctionForColor::OneMinusSrcAlpha,
    0.f, 0.f, 0.f, 1.f
) {

    AmountOfInstances++;

    if (FreeTypeLib == nullptr) {
        FreeTypeLib = (void*)(new FT_Library);

		int FT_InitErrorCode = FT_Init_FreeType((FT_Library*)FreeTypeLib);
		if (FT_InitErrorCode != 0)
			ErrorsSystemNamespace::SendError << "Failed to initialize FreeType library, FreeType returned error code :[" << std::to_string(FT_InitErrorCode) << "]" >> ErrorsEnumWrapperStruct(ErrorsEnum::FailedToInitializeFreeType);
    }

    {
        GP::ShaderClass VS(vertexShaderDir, GP::ShaderClass::TypesEnum::Vertex);
        VS.Compile();
        TEXT_SP.AttachShader(VS.gID());

        GP::ShaderClass FS(fragmentShaderDir, GP::ShaderClass::TypesEnum::Fragment);
        FS.Compile();
        TEXT_SP.AttachShader(FS.gID());

        TEXT_SP.LinkShaders();

        TEXT_SP.Bind();
        TEXT_SP.SetUniform1i(TEXT_SP.GetUniformIDByName("u_TextTexture"), 0);
    }

    TEXT_VA.Bind();
    TEXT_VB.Bind();
    TEXT_VA.SetAttributes(ArrayView<GP::VertexArrayClass::AttributeDataStruct>({
        GP::VertexArrayClass::AttributeDataStruct{0,TEXT_VB,false,0,2,0,sizeof(float) * (2 + 2),GP::VertexArrayClass::AttributeDataStruct::DataTypeInMemory_Enum::Float,GP::VertexArrayClass::AttributeDataStruct::DataTypeForReadingOnGPU_Enum::Float},
        GP::VertexArrayClass::AttributeDataStruct{1,TEXT_VB,false,0,2,sizeof(float) * 2,sizeof(float) * (2 + 2),GP::VertexArrayClass::AttributeDataStruct::DataTypeInMemory_Enum::Float,GP::VertexArrayClass::AttributeDataStruct::DataTypeForReadingOnGPU_Enum::Float},
        }));
}

GA::TextRendererClass::~TextRendererClass() {

    AmountOfInstances--;

    Fonts.clear();

    if (AmountOfInstances == 0) {
        FT_Done_FreeType(*(FT_Library*)FreeTypeLib);
        delete (FT_Library*)FreeTypeLib;
        FreeTypeLib = nullptr;
    }
}

const GA::TextRendererClass::FontStruct& GA::TextRendererClass::AddFont(unsigned int characterSize, const char* fontDir, const wchar_t* characters) {
    const FontStruct& insertedFont = Fonts.emplace_back(FontStruct::GuardFromUser(), characterSize, fontDir, characters);
    return insertedFont;
}

GA::TextRendererClass::FontStruct::~FontStruct() {
    if (FreeTypeFace != nullptr) {
        FT_Done_Face(*(FT_Face*)FreeTypeFace);
        FreeTypeFace = nullptr;
    }
}


GA::TextRendererClass::FontStruct::FontStruct(GuardFromUser, unsigned int characterSize, const char* fontDir, const wchar_t* chars) :
    Texture(GA::TextureClass::DimensionsEnum::Two, Vector3U(0, 0, 0), nullptr, 0, GA::TextureClass::SettingsStruct{
                    GA::TextureClass::SettingsStruct::WrapTypeEnum::ClampToBorder,GA::TextureClass::SettingsStruct::WrapTypeEnum::ClampToBorder,
                    GA::TextureClass::SettingsStruct::DownscalingFilterFuncEnum::Linear,GA::TextureClass::SettingsStruct::UpscalingFilterFuncEnum::Linear,
                    GA::TextureClass::SettingsStruct::DepthStencilReadModeEnum::Depth },
                    GA::TextureClass::DataSettingsStruct{ GA::TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::Red,
                    GA::TextureClass::DataSettingsStruct::DataFormatOnCPU_Enum::Red,GA::TextureClass::DataSettingsStruct::DataTypeOnCPU_Enum::UnsignedByte })
{ 
    FreeTypeFace = new FT_Face;

    {
        int FT_FontReadingError = FT_New_Face(*(FT_Library*)TextRendererClass::FreeTypeLib, fontDir, 0, (FT_Face*)FreeTypeFace);
        if (FT_FontReadingError != 0)
            ErrorsSystemNamespace::SendError << "FreeType failed to read font file and returned error code: [" << std::to_string(FT_FontReadingError) << "]" >> ErrorsEnumWrapperStruct(ErrorsEnum::FreeTypeError_FailedToReadFontFile);
    }

    FT_Set_Pixel_Sizes(*(FT_Face*)FreeTypeFace, 0, characterSize);


    unsigned int charsAmount = 0; while (chars[charsAmount] != L'\0') charsAmount++;

    Characters.ChangeCapacity(charsAmount);
    DynArr<DynArr<unsigned char>> buffers; buffers.ChangeCapacity(charsAmount);
    DynArr<unsigned int> nums; nums.ChangeCapacity(charsAmount);


    unsigned int maxWidth = 0; unsigned int maxHeight = 0;
    unsigned int totalXSize = 0;

    for (unsigned int chi = 0; chi < charsAmount; chi++) {

        unsigned int unicodeInd = (unsigned int)chars[chi];


        size_t insertInd = (Characters.gLen() == 0) ? 0 : BinarySearch(*Characters, Characters.gLen(), unicodeInd,
            +[](const unsigned int& v1, const CharacterStruct& v2)->bool {return v2.UnicodeInd > v1; },
            +[](const unsigned int& v1, const CharacterStruct& v2)->bool {return v1 == v2.UnicodeInd; });

        if (insertInd<Characters.gLen() and Characters[insertInd].UnicodeInd == unicodeInd) continue;

        {
            int FT_CharLoadingError = FT_Load_Char(*(FT_Face*)FreeTypeFace, unicodeInd, FT_LOAD_RENDER);
            if (FT_CharLoadingError != 0) {
                ErrorsSystemNamespace::SendError << "FreeType failed to load charater with unicode index of: [" << std::to_string(unicodeInd) <<
                    "] and returned error code: [" << std::to_string(FT_CharLoadingError) << "]" >> ErrorsEnumWrapperStruct(ErrorsEnum::FreeTypeError_FailedToLoadChar);
                continue;
            }
        }

        FT_Face& face = *(FT_Face*)FreeTypeFace;

        buffers.InsertAtIndex(insertInd, DynArr<unsigned char>(face->glyph->bitmap.width * face->glyph->bitmap.rows, face->glyph->bitmap.buffer));

        if (face->glyph->bitmap.width > maxWidth) maxWidth = face->glyph->bitmap.width;
        if (face->glyph->bitmap.rows > maxHeight) maxHeight = face->glyph->bitmap.rows;

        if (face->glyph->bitmap_top > MaxCharacterUp) MaxCharacterUp = face->glyph->bitmap_top;
        if ((int)face->glyph->bitmap.rows - face->glyph->bitmap_top > MaxCharacterDown) MaxCharacterDown = (int)face->glyph->bitmap.rows - face->glyph->bitmap_top;

        totalXSize += face->glyph->bitmap.width;

        Characters.InsertAtIndex(insertInd, CharacterStruct{
            unicodeInd,
            Vector2U(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            Vector2I(face->glyph->bitmap_left, face->glyph->bitmap_top),
            (unsigned int)face->glyph->advance.x / 64,
            Vector2F(0.f,0.f),0
            });
    }

    totalXSize += charsAmount + 1;

    DynArr<unsigned char> textureBuffer(totalXSize * maxHeight, (unsigned char)0);
    
    {
        unsigned int xOffset = 1;
        for (unsigned int ci = 0; ci < Characters.gLen(); ci++) {
            CharacterStruct& char_ = Characters[ci];
            char_.XOffsetInTexture = xOffset / (float)totalXSize;
            char_.SizeInTexture = Vector2F(char_.Size[0] / (float)totalXSize, char_.Size[1] / (float)maxHeight);
            xOffset += char_.Size[0] + 1;
        }
    }

    {
        unsigned int xOffset = 0;
        for (unsigned int ci = 0; ci < Characters.gLen(); ci++) {
            CharacterStruct& char_ = Characters[ci];
            for (unsigned int x = 0; x < char_.Size[0]; x++) for (unsigned int y = 0; y < char_.Size[1]; y++) {
                textureBuffer[ci + 1 + y * totalXSize + xOffset + x] = buffers[ci][(char_.Size[1] - y - 1) * char_.Size[0] + x];
            }

            xOffset += char_.Size[0];
        }
    }

	glSC(glPixelStorei(GL_UNPACK_ALIGNMENT, 1)); // disable byte-alignment restriction

    Texture = TextureClass(TextureClass::DimensionsEnum::Two, Vector3U(totalXSize, maxHeight, 0u), textureBuffer, 0, Texture.gSettings(), Texture.gDataSettings());
    
	glSC(glPixelStorei(GL_UNPACK_ALIGNMENT, 4)); // enable byte-alignment back

}

static constexpr unsigned int MAX_TEXT_LEN = 100000;

void GA::TextRendererClass::RenderText(const FontStruct& font, const wchar_t* text, Vector2F pos, Vector2F localOffset, Vector2U pixelsInTexture,
    Vector2F boxSize, float lineSizeInBox, const wchar_t* dividingSymbols) {
    
    if (boxSize[0] == 0 and boxSize[1] == 0) return;//theres nothing we can do

	TextPreset.Bind();
    TEXT_SP.Bind();
    TEXT_VA.Bind();
    TEXT_VB.Bind();

    font.Texture.Bind(0);

    unsigned int textLen = 0;
    while (textLen != MAX_TEXT_LEN and text[textLen] != L'\0') textLen++;
    if (textLen == MAX_TEXT_LEN) ErrorsSystemNamespace::SendError << "Provided text is too big or end symbol was not found, current limit on symbols is: [" <<
        std::to_string(MAX_TEXT_LEN) << "]" >> ErrorsEnumWrapperStruct(ErrorsEnum::TextTooBigOrNoEndFound);

    DynArr<size_t> charsInds; charsInds.ChangeLen(textLen);//adding +1 to mark unexisting characters, ind=0 will mean that character not found
    for (unsigned int ci = 0; ci < textLen; ci++) {
        charsInds[ci] = BinarySearch(
            *font.Characters, font.Characters.gLen(), (unsigned int)text[ci],
            +[](const unsigned int& num, const FontStruct::CharacterStruct& ch)->bool {return num < ch.UnicodeInd; },
            +[](const unsigned int& num, const FontStruct::CharacterStruct& ch)->bool {return num == ch.UnicodeInd; });
        if (charsInds[ci] >= font.Characters.gLen() or font.Characters[charsInds[ci]].UnicodeInd != (unsigned int)text[ci]) charsInds[ci] = 0;
		else charsInds[ci]++;
    }


    unsigned int fullLocalXSize = 0;
    int maxUp = 0; int maxDown = 0;
    for (unsigned int ci = 0; ci < textLen; ci++) {
        if (charsInds[ci] == 0) continue;
        const FontStruct::CharacterStruct& char_ = font.Characters[charsInds[ci] - 1];
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
    DynArr<float> VertexBufferData; VertexBufferData.ChangeLen(6 * (2 + 2) * textLen); unsigned int realTextLen = 0;
    for (unsigned int ci = 0; ci < textLen; ci++) {
		if (charsInds[ci] == 0) continue;

        const FontStruct::CharacterStruct& char_ = font.Characters[charsInds[ci] - 1];
        
        Vector2F lbp(pos[0] + xOffset + char_.Bearing[0] * localPixelsToTexScaleByX, 
            pos[1] + (font.MaxCharacterDown - ((int)char_.Size[1] - char_.Bearing[1])) * localPixelsToTexScaleByY);
        Vector2F rbp(lbp[0] + char_.Size[0] * localPixelsToTexScaleByX, lbp[1]);
        Vector2F ltp(lbp[0], lbp[1] + char_.Size[1] * localPixelsToTexScaleByY);
        Vector2F rtp(rbp[0], ltp[1]);

        Vector2F lbc(char_.XOffsetInTexture, 0.f);
        Vector2F rbc(lbc[0] + char_.SizeInTexture[0], 0.f);
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

        for (unsigned int i = 0; i < 6 * 4; i++) VertexBufferData[realTextLen * 4 * 6 + i] = data[i];



        xOffset += char_.Advance * localPixelsToTexScaleByX;

        realTextLen++;
    }

    VertexBufferData.CutDeadLength(realTextLen * 6 * 4);
    TEXT_VB.SetData(ArrayView<void>(VertexBufferData, realTextLen * 6 * 4 * sizeof(float)), GP::VertexBufferClass::BufferReadWriteModeEnum::StreamDraw);
    GP::RendererNamespace::DrawArrays(GP::RendererNamespace::PrimitivesEnum::Triangles, 0, 6 * realTextLen);


    TEXT_VA.Unbind();
}







