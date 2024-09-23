#pragma once
#include"Tools/DynArr.h"
#include"GraphicsPrimitives/Texture.h"
#include"Maths/Vector.h"

namespace GraphicsAbstractions {
	class TexturesManagerClass {

		typedef GraphicsPrimitives::Texture1DClass::CPU_DataData_Struct Texture1DData;
		typedef GraphicsPrimitives::Texture2DClass::CPU_DataData_Struct Texture2DData;

		class Texture2DClass {
			GraphicsPrimitives::Texture2DClass Texture;//graphical representation
			Texture2DData Data;
		};
		class Texture1DClass {
			GraphicsPrimitives::Texture1DClass Texture;//graphical representation
			Texture1DData Data;
		};

		DynArr<Texture1DClass> Textures1D;
		DynArr<Texture2DClass> Textures2D;

	};
}
