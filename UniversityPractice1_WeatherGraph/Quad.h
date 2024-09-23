#pragma once
#include"GraphicsPrimitives/VertexArray.h"
#include"GraphicsPrimitives/VertexBuffer.h"
#include"GraphicsPrimitives/ShaderProgram.h"
#include"GraphicsPrimitives/Texture.h"
#include"Maths/Vector.h"

using namespace GraphicsPrimitives;

class QuadsHandler {
	friend class Quad;
	VertexArrayClass VA;
	VertexBufferClass VB;
	ShaderProgramClass SP;
public:
	QuadsHandler(const wchar_t* vs, const wchar_t* fs);
	class Quad {
	protected:
		QuadsHandler& Handler;
	public:
		Vector2F Position = Vector2F(0, 0);
		Vector2F Size = Vector2F(1, 1);
		Vector2F LocalOffset = Vector2F(0, 0);
		Vector3F Color = Vector3F(0, 0, 0);
		Quad(QuadsHandler& handler);
		void Draw();
	};
	class ImageQuad: public Quad {
		Texture2DClass Tex;
	public:
		enum class TextureClampingType {
			FullScale,SquareByX,SquareByY,SquareByXY
		};
		TextureClampingType TexClampingType = TextureClampingType::FullScale;
		ImageQuad(QuadsHandler& handler, Texture2DClass&& tex);
		void Draw(unsigned int screenWidth, unsigned int screenHeight);
	};
};
