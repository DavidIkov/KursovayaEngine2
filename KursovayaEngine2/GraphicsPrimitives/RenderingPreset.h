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
		enum class TypeOfComparison {
			Never, Less, LessOrEqual, Greater, GreaterOrEqual, Equal, NotEqual, AlwaysPass
		};
	}

	//used to discard some elements from screen, can be used for outlining
	namespace StencilTest {
		//gpu will comparison two values, if comparison fails then pixel is discarder, othervise it wont
		enum class TypeOfComparison {
			Never, Less, LessOrEqual, Greater, GreaterOrEqual, Equal, NotEqual, AlwaysPass
		};
		//actions to take with current stencil buffer value on different events that may occur, for move info visit "https://docs.gl/gl3/glStencilOp"
		enum class Actions {
			Keep,Zero,Replace,Increase,IncreaseWrap,Decrease,DecreaseWrap,Invert
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
	RenderingPresetEnumArguments::DepthTest::TypeOfComparison DepthTest_TypeOfComparison;

	bool StencilTest_Enabled;
	unsigned int StencilTest_BaseMask;
	RenderingPresetEnumArguments::StencilTest::TypeOfComparison StencilTest_ComparisonType;
	unsigned int StencilTest_ReferenceValue;
	unsigned int StencilTest_Mask;
	RenderingPresetEnumArguments::StencilTest::Actions StencilTest_ActionOnSF;
	RenderingPresetEnumArguments::StencilTest::Actions StencilTest_ActionOnSPDF;
	RenderingPresetEnumArguments::StencilTest::Actions StencilTest_ActionOnSPDP;

	bool Blending_Enabled;
	//TODO:make RGBA class and make it work in here
	float Blending_ConstR;
	float Blending_ConstG;
	float Blending_ConstB;
	float Blending_ConstA;
	RenderingPresetEnumArguments::Blending::FunctionForColor Blending_FuncForColor1;
	RenderingPresetEnumArguments::Blending::FunctionForColor Blending_FuncForColor2;

	float ClearR;
	float ClearG;
	float ClearB;

	void UpdFaceCulling_Enabled();
	void UpdFaceCulling_FaceToCull();
	void UpdFaceCulling_FaceDetermination();

	void UpdDepthTest_Enabled();
	void UpdDepthTest_WriteInBufferEnabled();
	void UpdDepthTest_TypeOfComparison();

	void UpdStencilTest_Enabled();
	void UpdStencilTest_BaseMask();
	void UpdStencilTest_StencilFunc();
	void UpdStencilTest_StencilOpFunc();

	void UpdBlending_Enabled();
	void UpdBlending_ConstRGBA();
	void UpdBlending_FuncForColor();

	void UpdClearRGB();

public:

	void sFaceCulling_Enabled(bool newVal);
	void sFaceCulling_FaceToCull(RenderingPresetEnumArguments::FaceCulling::FaceToCull newVal);
	void sFaceCulling_FaceDetermination(RenderingPresetEnumArguments::FaceCulling::FaceDetermination newVal);

	void sDepthTest_Enabled(bool newVal);
	void sDepthTest_WriteInBufferEnabled(bool newVal);
	void sDepthTest_TypeOfComparison(RenderingPresetEnumArguments::DepthTest::TypeOfComparison newVal);

	void sStencilTest_Enabled(bool newVal);
	void sStencilTest_BaseMask(unsigned int newVal);
	void sStencilTest_ComparisonType(RenderingPresetEnumArguments::StencilTest::TypeOfComparison newVal);
	void sStencilTest_ReferenceValue(unsigned int newVal);
	void sStencilTest_Mask(unsigned int newVal);
	void sStencilTest_ActionOnSF(RenderingPresetEnumArguments::StencilTest::Actions newVal);
	void sStencilTest_ActionOnSPDF(RenderingPresetEnumArguments::StencilTest::Actions newVal);
	void sStencilTest_ActionOnSPDP(RenderingPresetEnumArguments::StencilTest::Actions newVal);

	void sBlending_Enabled(bool newVal);
	void sBlending_ConstR(float newVal);
	void sBlending_ConstG(float newVal);
	void sBlending_ConstB(float newVal);
	void sBlending_ConstA(float newVal);
	void sBlending_FuncForColor1(RenderingPresetEnumArguments::Blending::FunctionForColor newVal);
	void sBlending_FuncForColor2(RenderingPresetEnumArguments::Blending::FunctionForColor newVal);

	void sClearR(float newVal);
	void sClearG(float newVal);
	void sClearB(float newVal);




	RenderingPreset(
		bool f_FaceCullingEnabled,
		RenderingPresetEnumArguments::FaceCulling::FaceToCull f_FaceCulling_FaceToCull,
		RenderingPresetEnumArguments::FaceCulling::FaceDetermination f_FaceCulling_FaceDetermination,

		bool f_DepthTest_Enabled,
		bool f_DepthTest_WriteInBufferEnabled,
		RenderingPresetEnumArguments::DepthTest::TypeOfComparison f_DepthTest_TypeOfComparison,

		bool f_StencilTest_Enabled,
		unsigned int f_StencilTest_BaseMask,
		RenderingPresetEnumArguments::StencilTest::TypeOfComparison f_StencilTest_ComparisonType,
		unsigned int f_StencilTest_ReferenceValue,
		unsigned int f_StencilTest_Mask,
		RenderingPresetEnumArguments::StencilTest::Actions f_StencilTest_ActionOnSF,
		RenderingPresetEnumArguments::StencilTest::Actions f_StencilTest_ActionOnSPDF,
		RenderingPresetEnumArguments::StencilTest::Actions f_StencilTest_ActionOnSPDP,

		bool f_Blending_Enabled,
		float f_Blending_ConstR,
		float f_Blending_ConstG,
		float f_Blending_ConstB,
		float f_Blending_ConstA,
		RenderingPresetEnumArguments::Blending::FunctionForColor f_Blending_FuncForColor1,
		RenderingPresetEnumArguments::Blending::FunctionForColor f_Blending_FuncForColor2,

		float f_ClearR,
		float f_ClearG,
		float f_ClearB
		);
	void Bind();
	static void Unbind();
};