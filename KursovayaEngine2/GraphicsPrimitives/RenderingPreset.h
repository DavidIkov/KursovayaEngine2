#pragma once

namespace RenderingPresetEnumArguments {

	//used to not render vertexes which cant be seen since they facing away from the camera
	namespace FaceCulling {
		//describes which face to cull
		enum class FaceToCull {
			Back, Front
		};
		/*describe how to determine fs face front or back,
		to determine that, gpu takes position of 3 vertexes of triangle from camera perspective
		and then IF they positioned clockwise and setting is set to clockwise then its front face,
		and IF they positioned anticlockwise then its back face, if settings is set to anticlockwise then everything is same but reversed*/
		enum class FaceDetermination {
			Clockwise, AntiClockwise,
		};
	}

	//used to determine if object is behind some other object or nah
	namespace DepthTest {
		//gpu will compare two values, current value and stored value, if test passes then pixel will be drawn, otherwise it will be discarded
		enum class TypeOfComprasion {
			Never,Less,Equal,LessOrEqual,Greater,GreaterOrEqual,NotEqual,AlwaysPass
		};
	}

	//correspond to blending color, depending on alpha value of RGBA
	namespace Blending {
		/*Src-Source, Dst-Destination, Const-Constant(this is basically a blending color which you can set)
		this enum correspond which function gpu should use when it need to blend somnething,
		for more technical info visit "https://docs.gl/gl4/glBlendFunc" since there is a good explanation
		on this topic and i dont see why i should copy it in here since it will be same but with different names
		TODO: make thing named "dual source blending" since glBlendFunc have arguments where second source mentioned*/
		enum class FunctionForColor {
			Zero,One,SrcColor,OneMinusSrcColor,DstColor,OneMinusDstColor,SrcAlpha,OneMinusSrcAlpha,
			DstAlpha,OneMinusDstAlpha,ConstColor,OneMinusConstColor,ConstAlpha,OneMinusConstAlpha,
			SrcAlphaSaturate,
		};
	}


};

//this thing can save and quickly apply settings for rendering, such as cull faces, depth test, blending, etc...
class RenderingPreset {
	bool FaceCulling_Enabled;
	RenderingPresetEnumArguments::FaceCulling::FaceToCull FaceCulling_FaceToCull;
	RenderingPresetEnumArguments::FaceCulling::FaceDetermination FaceCulling_FaceDetermination;

	bool DepthTest_Enabled;
	bool DepthTest_WriteInBufferEnabled;
	RenderingPresetEnumArguments::DepthTest::TypeOfComprasion DepthTest_TypeOfComprasion;

	bool Blending_Enabled;
	//TODO:make RGBA class and make it work in here
	float Blending_ConstR = 0;
	float Blending_ConstG = 0;
	float Blending_ConstB = 0;
	float Blending_ConstA = 0;
	RenderingPresetEnumArguments::Blending::FunctionForColor Blending_FuncForColor1;
	RenderingPresetEnumArguments::Blending::FunctionForColor Blending_FuncForColor2;

public:
	RenderingPreset(
		bool f_FaceCullingEnabled,
		RenderingPresetEnumArguments::FaceCulling::FaceToCull f_FaceCulling_FaceToCull,
		RenderingPresetEnumArguments::FaceCulling::FaceDetermination f_FaceCulling_FaceDetermination,

		bool f_DepthTest_Enabled,
		bool f_DepthTest_WriteInBufferEnabled,
		RenderingPresetEnumArguments::DepthTest::TypeOfComprasion f_DepthTest_TypeOfComprasion,

		bool f_Blending_Enabled,
		float f_Blending_ConstR,
		float f_Blending_ConstG,
		float f_Blending_ConstB,
		float f_Blending_ConstA,
		RenderingPresetEnumArguments::Blending::FunctionForColor f_Blending_FuncForColor1,
		RenderingPresetEnumArguments::Blending::FunctionForColor f_Blending_FuncForColor2
		);
	void Bind();
	static void Unbind();
};