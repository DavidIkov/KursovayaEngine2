#pragma once
#include"DLL.h"
#include"Maths/Vector.h"
#include"Graphics/Primitives/ShaderProgram.h"
#include"Graphics/Primitives/RenderingPreset.h"
#include"Graphics/Primitives/VertexArray.h"
#include"Graphics/Primitives/VertexBuffer.h"
#include"Texture.h"
#include"Tools/DynArr.h"

namespace Graphics::Abstractions {
	class TextRendererClass {
		struct FontStruct {

			struct CharacterStruct {
				unsigned int UnicodeInd;
				Vector2U Size;       // Size of glyph
				Vector2I Bearing;    // Offset from baseline to left/top of glyph
				unsigned int Advance; // Offset to advance to next glyph
				Vector2F SizeInTexture;
				float XOffsetInTexture;
			};
			mutable bool Deleted = false;
			TextureClass Texture;
			void* FreeTypeFace;//FT_Face
			DynArr<CharacterStruct> Characters;
			int MaxCharacterUp = 0; int MaxCharacterDown = 0;

			FontStruct(unsigned int characterSize, const char* fontDir, const wchar_t* chars);
			FontStruct(const FontStruct&& toCopy);
			~FontStruct();
		};

		DynArr<FontStruct> Fonts;

		Graphics::Primitives::ShaderProgramClass TEXT_SP;
		Graphics::Primitives::RenderingPresetClass TextPreset;
		Graphics::Primitives::VertexArrayClass TEXT_VA;
		Graphics::Primitives::VertexBufferClass TEXT_VB;

		static void* FreeTypeLib;//FT_Library, nullptr

	public:

		struct ErrorsEnumWrapperStruct :KE2::ErrorsSystemNamespace::ErrorBase {
			enum ErrorsEnum {
				FailedToInitializeFreeType,
				TextTooBigOrNoEndFound,
				FreeTypeError_FailedToLoadChar,
				FreeTypeError_FailedToReadFontFile,
			}; ErrorsEnum Error;
			inline ErrorsEnumWrapperStruct(ErrorsEnum error) :Error(error) {};
		}; using ErrorsEnum = ErrorsEnumWrapperStruct; using AnyError = ErrorsEnumWrapperStruct;

		DLLTREATMENT TextRendererClass(const wchar_t* vertexShaderDir, const wchar_t* fragmentShaderDir);
		DLLTREATMENT ~TextRendererClass();
		DLLTREATMENT StalkerClass AddFont(unsigned int characterSize, const char* fontDir, const wchar_t* characters);

		//pixelsInTexture is amount of pixels in the texture you are rendering this text to
		//TODO add functionality for "dividingSymbols" and make so text will be separated in a few lines if possible
		DLLTREATMENT void RenderText(const StalkerClass& fontStaker, const wchar_t* text, Vector2F pos, Vector2F localOffset, Vector2U pixelsInTexture,
			Vector2F boxSize, float lineSizeInBox, const wchar_t* dividingSymbols = nullptr);


	};
}

