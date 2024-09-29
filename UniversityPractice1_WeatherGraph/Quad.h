#pragma once
#include"Graphics/Primitives/VertexArray.h"
#include"Graphics/Primitives/VertexBuffer.h"
#include"Graphics/Primitives/ShaderProgram.h"
#include"Graphics/Primitives/Texture.h"
#include"Maths/Vector.h"

using namespace Graphics::Primitives;

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
		TextureClass Tex;
	public:
		enum class TextureClampingType {
			FullScale,SquareByX,SquareByY,SquareByXY
		};
		TextureClampingType TexClampingType = TextureClampingType::FullScale;
		ImageQuad(QuadsHandler& handler, TextureClass&& tex);
		void Draw(unsigned int screenWidth, unsigned int screenHeight);
	};
};
