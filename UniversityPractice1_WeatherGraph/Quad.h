#pragma once
#include"GraphicsPrimitives/VertexArray.h"
#include"GraphicsPrimitives/VertexBuffer.h"
#include"GraphicsPrimitives/ShaderProgram.h"
#include"GraphicsPrimitives/Texture.h"
#include"Maths/Vector2.h"
#include"Maths/Vector3.h"
class QuadsHandler {
	friend class Quad;
	VertexArray VA;
	VertexBuffer VB;
	ShaderProgram SP;
public:
	QuadsHandler(const char* vs, const char* fs);
	class Quad {
	protected:
		QuadsHandler& Handler;
	public:
		Vector2 Position = Vector2(0, 0);
		Vector2 Size = Vector2(1, 1);
		Vector2 LocalOffset = Vector2(0, 0);
		Vector3 Color = Vector3(0, 0, 0);
		Quad(QuadsHandler& handler);
		void Draw();
	};
	class ImageQuad: public Quad {
		Texture Tex;
	public:
		enum class TextureClampingType {
			FullScale,SquareByX,SquareByY,SquareByXY
		};
		TextureClampingType TexClampingType = TextureClampingType::FullScale;
		ImageQuad(QuadsHandler& handler, Texture&& tex);
		void Draw(unsigned int screenWidth, unsigned int screenHeight);
	};
};
