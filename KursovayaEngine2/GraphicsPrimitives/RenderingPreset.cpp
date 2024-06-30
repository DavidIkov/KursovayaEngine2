#include"RenderingPreset.h"
#include"glad/glad.h"
#include"Tools/GLDebug.h"

RenderingPreset::RenderingPreset(
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
) :
	FaceCulling_Enabled(f_FaceCullingEnabled), FaceCulling_FaceToCull(f_FaceCulling_FaceToCull), FaceCulling_FaceDetermination(f_FaceCulling_FaceDetermination),
	DepthTest_Enabled(f_DepthTest_Enabled), DepthTest_WriteInBufferEnabled(f_DepthTest_WriteInBufferEnabled), DepthTest_TypeOfComprasion(f_DepthTest_TypeOfComprasion),
	Blending_Enabled(f_Blending_Enabled), Blending_ConstR(f_Blending_ConstR), Blending_ConstG(f_Blending_ConstG), Blending_ConstB(f_Blending_ConstG), Blending_ConstA(f_Blending_ConstA),
	Blending_FuncForColor1(f_Blending_FuncForColor1), Blending_FuncForColor2(f_Blending_FuncForColor2)
{

}
void RenderingPreset::Bind() {
	
	switch (FaceCulling_Enabled) {
	case true: 
	{
		glSC(glEnable(GL_CULL_FACE));
		break;
	}
	case false: 
	{
		glSC(glDisable(GL_CULL_FACE));
		break;
	}
	}

	switch (FaceCulling_FaceToCull) {
	case RenderingPresetEnumArguments::FaceCulling::FaceToCull::Back: 
	{
		glSC(glCullFace(GL_BACK));
		break;
	}
	case RenderingPresetEnumArguments::FaceCulling::FaceToCull::Front:
	{
		glSC(glCullFace(GL_FRONT));
		break;
	}
	}

	switch (FaceCulling_FaceDetermination) {
	case RenderingPresetEnumArguments::FaceCulling::FaceDetermination::Clockwise:
	{
		glSC(glFrontFace(GL_CW));
		break;
	}
	case RenderingPresetEnumArguments::FaceCulling::FaceDetermination::AntiClockwise:
	{
		//counter clockwise
		glSC(glFrontFace(GL_CCW));
		break;
	}
	}

	switch (DepthTest_Enabled) {
	case true:
	{
		glSC(glEnable(GL_DEPTH_TEST));
		break;
	}
	case false:
	{
		glSC(glDisable(GL_DEPTH_TEST));
		break;
	}
	}

	switch (DepthTest_WriteInBufferEnabled) {
	case true:
	{
		glSC(glDepthMask(GL_TRUE));
		break;
	}
	case false:
	{
		glSC(glDepthMask(GL_FALSE));
		break;
	}
	}

	switch (DepthTest_TypeOfComprasion) {
	case RenderingPresetEnumArguments::DepthTest::TypeOfComprasion::Never:
	{
		glSC(glDepthFunc(GL_NEVER));
		break;
	}
	case RenderingPresetEnumArguments::DepthTest::TypeOfComprasion::Less:
	{
		glSC(glDepthFunc(GL_LESS));
		break;
	}
	case RenderingPresetEnumArguments::DepthTest::TypeOfComprasion::Equal:
	{
		glSC(glDepthFunc(GL_EQUAL));
		break;
	}
	case RenderingPresetEnumArguments::DepthTest::TypeOfComprasion::LessOrEqual:
	{
		glSC(glDepthFunc(GL_LEQUAL));
		break;
	}
	case RenderingPresetEnumArguments::DepthTest::TypeOfComprasion::Greater:
	{
		glSC(glDepthFunc(GL_GREATER));
		break;
	}
	case RenderingPresetEnumArguments::DepthTest::TypeOfComprasion::GreaterOrEqual:
	{
		glSC(glDepthFunc(GL_GEQUAL));
		break;
	}
	case RenderingPresetEnumArguments::DepthTest::TypeOfComprasion::NotEqual:
	{
		glSC(glDepthFunc(GL_NOTEQUAL));
		break;
	}
	case RenderingPresetEnumArguments::DepthTest::TypeOfComprasion::AlwaysPass:
	{
		glSC(glDepthFunc(GL_ALWAYS));
		break;
	}
	}

	switch (Blending_Enabled) {
	case true:
	{
		glSC(glEnable(GL_BLEND));
		break;
	}
	case false:
	{
		glSC(glDisable(GL_BLEND));
		break;
	}
	}

	glSC(glBlendColor(Blending_ConstR, Blending_ConstG, Blending_ConstB, Blending_ConstA));

	{
		int args[2] = { 0,0 };
		int checkVal[2] = { (int)Blending_FuncForColor1,(int)Blending_FuncForColor2 };
		for (unsigned int i = 0; i < 2; i++) {
			switch (checkVal[i]) {
			case (int)RenderingPresetEnumArguments::Blending::FunctionForColor::Zero:
			{
				args[i] = GL_ZERO;
				break;
			}
			case (int)RenderingPresetEnumArguments::Blending::FunctionForColor::One:
			{
				args[i] = GL_ONE;
				break;
			}
			case (int)RenderingPresetEnumArguments::Blending::FunctionForColor::SrcColor:
			{
				args[i] = GL_SRC_COLOR;
				break;
			}
			case (int)RenderingPresetEnumArguments::Blending::FunctionForColor::OneMinusSrcColor:
			{
				args[i] = GL_ONE_MINUS_SRC_COLOR;
				break;
			}
			case (int)RenderingPresetEnumArguments::Blending::FunctionForColor::DstColor:
			{
				args[i] = GL_DST_COLOR;
				break;
			}
			case (int)RenderingPresetEnumArguments::Blending::FunctionForColor::OneMinusDstColor:
			{
				args[i] = GL_ONE_MINUS_DST_COLOR;
				break;
			}
			case (int)RenderingPresetEnumArguments::Blending::FunctionForColor::SrcAlpha:
			{
				args[i] = GL_SRC_ALPHA;
				break;
			}
			case (int)RenderingPresetEnumArguments::Blending::FunctionForColor::OneMinusSrcAlpha:
			{
				args[i] = GL_ONE_MINUS_SRC_ALPHA;
				break;
			}
			case (int)RenderingPresetEnumArguments::Blending::FunctionForColor::DstAlpha:
			{
				args[i] = GL_DST_ALPHA;
				break;
			}
			case (int)RenderingPresetEnumArguments::Blending::FunctionForColor::OneMinusDstAlpha:
			{
				args[i] = GL_ONE_MINUS_DST_ALPHA;
				break;
			}
			case (int)RenderingPresetEnumArguments::Blending::FunctionForColor::ConstColor:
			{
				args[i] = GL_CONSTANT_COLOR;
				break;
			}
			case (int)RenderingPresetEnumArguments::Blending::FunctionForColor::OneMinusConstColor:
			{
				args[i] = GL_ONE_MINUS_CONSTANT_COLOR;
				break;
			}
			case (int)RenderingPresetEnumArguments::Blending::FunctionForColor::ConstAlpha:
			{
				args[i] = GL_CONSTANT_ALPHA;
				break;
			}
			case (int)RenderingPresetEnumArguments::Blending::FunctionForColor::OneMinusConstAlpha:
			{
				args[i] = GL_ONE_MINUS_CONSTANT_ALPHA;
				break;
			}
			case (int)RenderingPresetEnumArguments::Blending::FunctionForColor::SrcAlphaSaturate:
			{
				args[i] = GL_SRC_ALPHA_SATURATE;
				break;
			}
			}
		}
		glSC(glBlendFunc(args[0], args[1]));
	}

}
void RenderingPreset::Unbind() {
	glSC(glDisable(GL_CULL_FACE));
	glSC(glDisable(GL_BLEND));
	glSC(glDisable(GL_DEPTH_TEST))
}