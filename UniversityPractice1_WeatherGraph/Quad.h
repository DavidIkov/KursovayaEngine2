#pragma once
#include"GraphicsPrimitives/VertexArray.h"
#include"GraphicsPrimitives/VertexBuffer.h"
#include"GraphicsPrimitives/ShaderProgram.h"
#include"GraphicsPrimitives/Texture.h"
#include"Maths/Vector.h"
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
		Vector<2> Position = Vector<2>(0, 0);
		Vector<2> Size = Vector<2>(1, 1);
		Vector<2> LocalOffset = Vector<2>(0, 0);
		Vector<3> Color = Vector<3>(0, 0, 0);
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
