#include"RenderingPreset.h"
#include"glad/glad.h"
#include"Tools/GLDebug.h"

RenderingPreset::RenderingPreset(
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
) :
FaceCulling_Enabled(f_FaceCullingEnabled), FaceCulling_FaceToCull(f_FaceCulling_FaceToCull), FaceCulling_FaceDetermination(f_FaceCulling_FaceDetermination),
DepthTest_Enabled(f_DepthTest_Enabled), DepthTest_WriteInBufferEnabled(f_DepthTest_WriteInBufferEnabled), DepthTest_TypeOfComparison(f_DepthTest_TypeOfComparison),
StencilTest_Enabled(f_StencilTest_Enabled), StencilTest_BaseMask(f_StencilTest_BaseMask), StencilTest_ComparisonType(f_StencilTest_ComparisonType),
StencilTest_ReferenceValue(f_StencilTest_ReferenceValue), StencilTest_Mask(f_StencilTest_Mask), StencilTest_ActionOnSF(f_StencilTest_ActionOnSF),
StencilTest_ActionOnSPDF(f_StencilTest_ActionOnSPDF), StencilTest_ActionOnSPDP(f_StencilTest_ActionOnSPDP),
Blending_Enabled(f_Blending_Enabled), Blending_ConstR(f_Blending_ConstR), Blending_ConstG(f_Blending_ConstG), Blending_ConstB(f_Blending_ConstG), Blending_ConstA(f_Blending_ConstA),
Blending_FuncForColor1(f_Blending_FuncForColor1), Blending_FuncForColor2(f_Blending_FuncForColor2),
ClearR(f_ClearR), ClearG(f_ClearG), ClearB(f_ClearB)
{

}


void RenderingPreset::sFaceCulling_Enabled(bool newVal) { FaceCulling_Enabled = newVal; UpdFaceCulling_Enabled(); }
void RenderingPreset::sFaceCulling_FaceToCull(RenderingPresetEnumArguments::FaceCulling::FaceToCull newVal) { FaceCulling_FaceToCull = newVal; UpdFaceCulling_FaceToCull(); }
void RenderingPreset::sFaceCulling_FaceDetermination(RenderingPresetEnumArguments::FaceCulling::FaceDetermination newVal) { FaceCulling_FaceDetermination = newVal; UpdFaceCulling_FaceDetermination(); }
void RenderingPreset::sDepthTest_Enabled(bool newVal) { DepthTest_Enabled = newVal; UpdDepthTest_Enabled(); }
void RenderingPreset::sDepthTest_WriteInBufferEnabled(bool newVal) { DepthTest_WriteInBufferEnabled = newVal; UpdDepthTest_WriteInBufferEnabled(); }
void RenderingPreset::sDepthTest_TypeOfComparison(RenderingPresetEnumArguments::DepthTest::TypeOfComparison newVal) { DepthTest_TypeOfComparison = newVal; UpdDepthTest_TypeOfComparison(); }
void RenderingPreset::sStencilTest_Enabled(bool newVal) { StencilTest_Enabled = newVal; UpdStencilTest_Enabled(); }
void RenderingPreset::sStencilTest_BaseMask(unsigned int newVal) { StencilTest_BaseMask = newVal; UpdStencilTest_BaseMask(); }
void RenderingPreset::sStencilTest_ComparisonType(RenderingPresetEnumArguments::StencilTest::TypeOfComparison newVal) { StencilTest_ComparisonType = newVal; UpdStencilTest_StencilFunc(); }
void RenderingPreset::sStencilTest_ReferenceValue(unsigned int newVal) { StencilTest_ReferenceValue = newVal; UpdStencilTest_StencilFunc(); }
void RenderingPreset::sStencilTest_Mask(unsigned int newVal) { StencilTest_Mask = newVal; UpdStencilTest_StencilFunc(); }
void RenderingPreset::sStencilTest_ActionOnSF(RenderingPresetEnumArguments::StencilTest::Actions newVal) { StencilTest_ActionOnSF = newVal; UpdStencilTest_StencilOpFunc(); }
void RenderingPreset::sStencilTest_ActionOnSPDF(RenderingPresetEnumArguments::StencilTest::Actions newVal) { StencilTest_ActionOnSPDF = newVal; UpdStencilTest_StencilOpFunc(); }
void RenderingPreset::sStencilTest_ActionOnSPDP(RenderingPresetEnumArguments::StencilTest::Actions newVal) { StencilTest_ActionOnSPDP = newVal; UpdStencilTest_StencilOpFunc(); }
void RenderingPreset::sBlending_Enabled(bool newVal) { Blending_Enabled = newVal; UpdBlending_Enabled(); }
void RenderingPreset::sBlending_ConstR(float newVal) { Blending_ConstR = newVal; UpdBlending_ConstRGBA(); }
void RenderingPreset::sBlending_ConstG(float newVal) { Blending_ConstG = newVal; UpdBlending_ConstRGBA(); }
void RenderingPreset::sBlending_ConstB(float newVal) { Blending_ConstB = newVal; UpdBlending_ConstRGBA(); }
void RenderingPreset::sBlending_ConstA(float newVal) { Blending_ConstA = newVal; UpdBlending_ConstRGBA(); }
void RenderingPreset::sBlending_FuncForColor1(RenderingPresetEnumArguments::Blending::FunctionForColor newVal) { Blending_FuncForColor1 = newVal; UpdBlending_FuncForColor(); }
void RenderingPreset::sBlending_FuncForColor2(RenderingPresetEnumArguments::Blending::FunctionForColor newVal) { Blending_FuncForColor2 = newVal; UpdBlending_FuncForColor(); }
void RenderingPreset::sClearR(float newVal) { ClearR = newVal; UpdClearRGB(); }
void RenderingPreset::sClearG(float newVal) { ClearG = newVal; UpdClearRGB(); }
void RenderingPreset::sClearB(float newVal) { ClearB = newVal; UpdClearRGB(); }


void RenderingPreset::UpdFaceCulling_Enabled() {
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
}
void RenderingPreset::UpdFaceCulling_FaceToCull() {
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
}
void RenderingPreset::UpdFaceCulling_FaceDetermination() {
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
}

void RenderingPreset::UpdDepthTest_Enabled() {
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
}
void RenderingPreset::UpdDepthTest_WriteInBufferEnabled() {
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
}
void RenderingPreset::UpdDepthTest_TypeOfComparison() {
	switch (DepthTest_TypeOfComparison) {
	case RenderingPresetEnumArguments::DepthTest::TypeOfComparison::Never:
	{
		glSC(glDepthFunc(GL_NEVER));
		break;
	}
	case RenderingPresetEnumArguments::DepthTest::TypeOfComparison::Less:
	{
		glSC(glDepthFunc(GL_LESS));
		break;
	}
	case RenderingPresetEnumArguments::DepthTest::TypeOfComparison::LessOrEqual:
	{
		glSC(glDepthFunc(GL_LEQUAL));
		break;
	}
	case RenderingPresetEnumArguments::DepthTest::TypeOfComparison::Greater:
	{
		glSC(glDepthFunc(GL_GREATER));
		break;
	}
	case RenderingPresetEnumArguments::DepthTest::TypeOfComparison::GreaterOrEqual:
	{
		glSC(glDepthFunc(GL_GEQUAL));
		break;
	}
	case RenderingPresetEnumArguments::DepthTest::TypeOfComparison::Equal:
	{
		glSC(glDepthFunc(GL_EQUAL));
		break;
	}
	case RenderingPresetEnumArguments::DepthTest::TypeOfComparison::NotEqual:
	{
		glSC(glDepthFunc(GL_NOTEQUAL));
		break;
	}
	case RenderingPresetEnumArguments::DepthTest::TypeOfComparison::AlwaysPass:
	{
		glSC(glDepthFunc(GL_ALWAYS));
		break;
	}
	}
}

void RenderingPreset::UpdStencilTest_Enabled() {
	switch (StencilTest_Enabled) {
	case true:
	{
		glSC(glEnable(GL_STENCIL_TEST));
		break;
	}
	case false:
	{
		glSC(glDisable(GL_STENCIL_TEST));
		break;
	}
	}
}
void RenderingPreset::UpdStencilTest_BaseMask() {
	glSC(glStencilMask(StencilTest_BaseMask));
}
void RenderingPreset::UpdStencilTest_StencilFunc() {
	int func = 0;
	switch (StencilTest_ComparisonType) {
	case RenderingPresetEnumArguments::StencilTest::TypeOfComparison::Never:
	{
		func = GL_NEVER;
		break;
	}
	case RenderingPresetEnumArguments::StencilTest::TypeOfComparison::Less:
	{
		func = GL_LESS;
		break;
	}
	case RenderingPresetEnumArguments::StencilTest::TypeOfComparison::LessOrEqual:
	{
		func = GL_LEQUAL;
		break;
	}
	case RenderingPresetEnumArguments::StencilTest::TypeOfComparison::Greater:
	{
		func = GL_GREATER;
		break;
	}
	case RenderingPresetEnumArguments::StencilTest::TypeOfComparison::GreaterOrEqual:
	{
		func = GL_GEQUAL;
		break;
	}
	case RenderingPresetEnumArguments::StencilTest::TypeOfComparison::Equal:
	{
		func = GL_EQUAL;
		break;
	}
	case RenderingPresetEnumArguments::StencilTest::TypeOfComparison::NotEqual:
	{
		func = GL_NOTEQUAL;
		break;
	}
	case RenderingPresetEnumArguments::StencilTest::TypeOfComparison::AlwaysPass:
	{
		func = GL_ALWAYS;
		break;
	}
	}
	glSC(glStencilFunc(func, StencilTest_ReferenceValue, StencilTest_Mask));

}
void RenderingPreset::UpdStencilTest_StencilOpFunc() {
	int args[3] = { 0,0,0 };
	int checkVal[3] = { (int)StencilTest_ActionOnSF,(int)StencilTest_ActionOnSPDF,(int)StencilTest_ActionOnSPDP };
	for (unsigned int i = 0; i < 3; i++) {
		switch (checkVal[i]) {
		case (int)RenderingPresetEnumArguments::StencilTest::Actions::Keep:
		{
			args[i] = GL_KEEP;
			break;
		}
		case (int)RenderingPresetEnumArguments::StencilTest::Actions::Zero:
		{
			args[i] = GL_ZERO;
			break;
		}
		case (int)RenderingPresetEnumArguments::StencilTest::Actions::Replace:
		{
			args[i] = GL_REPLACE;
			break;
		}
		case (int)RenderingPresetEnumArguments::StencilTest::Actions::Increase:
		{
			args[i] = GL_INCR;
			break;
		}
		case (int)RenderingPresetEnumArguments::StencilTest::Actions::IncreaseWrap:
		{
			args[i] = GL_INCR_WRAP;
			break;
		}
		case (int)RenderingPresetEnumArguments::StencilTest::Actions::Decrease:
		{
			args[i] = GL_DECR;
			break;
		}
		case (int)RenderingPresetEnumArguments::StencilTest::Actions::DecreaseWrap:
		{
			args[i] = GL_DECR_WRAP;
			break;
		}
		case (int)RenderingPresetEnumArguments::StencilTest::Actions::Invert:
		{
			args[i] = GL_INVERT;
			break;
		}
		}
	}
	glSC(glStencilOp(args[0], args[1], args[2]));
}

void RenderingPreset::UpdBlending_Enabled() {
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
}
void RenderingPreset::UpdBlending_ConstRGBA() {
	glSC(glBlendColor(Blending_ConstR, Blending_ConstG, Blending_ConstB, Blending_ConstA));
}
void RenderingPreset::UpdBlending_FuncForColor() {
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

void RenderingPreset::UpdClearRGB() {
	glSC(glClearColor(ClearR, ClearG, ClearB, 1.f));
}


void RenderingPreset::Bind() {
	
	UpdFaceCulling_Enabled();
	UpdFaceCulling_FaceToCull();
	UpdFaceCulling_FaceDetermination();

	UpdDepthTest_Enabled();
	UpdDepthTest_WriteInBufferEnabled();
	UpdDepthTest_TypeOfComparison();

	UpdStencilTest_Enabled();
	UpdStencilTest_BaseMask();
	UpdStencilTest_StencilFunc();
	UpdStencilTest_StencilOpFunc();

	UpdBlending_Enabled();
	UpdBlending_ConstRGBA();
	UpdBlending_FuncForColor();

	UpdClearRGB();

	glSC(glClear(GL_COLOR_BUFFER_BIT));
	if (DepthTest_Enabled) { glSC(glClear(GL_DEPTH_BUFFER_BIT)); }
	if (StencilTest_Enabled) { glSC(glClear(GL_STENCIL_BUFFER_BIT)); }
	
}
void RenderingPreset::Unbind() {
	glSC(glDisable(GL_CULL_FACE));
	glSC(glDisable(GL_BLEND));
	glSC(glDisable(GL_DEPTH_TEST));
	glSC(glDisable(GL_STENCIL_TEST));
}