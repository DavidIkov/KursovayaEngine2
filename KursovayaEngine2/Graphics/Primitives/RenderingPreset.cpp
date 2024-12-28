#include"RenderingPreset.h"
#include"glad/glad.h"
#include"Tools/GLDebug.h"

using namespace KE2;
using namespace Graphics::Primitives;

RenderingPresetClass::RenderingPresetClass(
	bool f_FaceCullingEnabled,
	RenderingPresetEnumArgumentsNamespace::FaceCulling::FaceToCull f_FaceCulling_FaceToCull,
	RenderingPresetEnumArgumentsNamespace::FaceCulling::FaceDetermination f_FaceCulling_FaceDetermination,

	bool f_DepthTest_Enabled,
	bool f_DepthTest_WriteInBufferEnabled,
	RenderingPresetEnumArgumentsNamespace::DepthTest::TypeOfComparison f_DepthTest_TypeOfComparison,

	bool f_StencilTest_Enabled,
	unsigned int f_StencilTest_BaseMask,
	RenderingPresetEnumArgumentsNamespace::StencilTest::TypeOfComparison f_StencilTest_ComparisonType,
	unsigned int f_StencilTest_ReferenceValue,
	unsigned int f_StencilTest_Mask,
	RenderingPresetEnumArgumentsNamespace::StencilTest::Actions f_StencilTest_ActionOnSF,
	RenderingPresetEnumArgumentsNamespace::StencilTest::Actions f_StencilTest_ActionOnSPDF,
	RenderingPresetEnumArgumentsNamespace::StencilTest::Actions f_StencilTest_ActionOnSPDP,

	bool f_Blending_Enabled,
	float f_Blending_ConstR,
	float f_Blending_ConstG,
	float f_Blending_ConstB,
	float f_Blending_ConstA,
	RenderingPresetEnumArgumentsNamespace::Blending::FunctionForColor f_Blending_FuncForColor1,
	RenderingPresetEnumArgumentsNamespace::Blending::FunctionForColor f_Blending_FuncForColor2,

	bool f_ColorEnabledR,
	bool f_ColorEnabledG,
	bool f_ColorEnabledB,
	bool f_ColorEnabledA,

	float f_ClearR,
	float f_ClearG,
	float f_ClearB,
	float f_ClearA
) :
FaceCulling_Enabled(f_FaceCullingEnabled), FaceCulling_FaceToCull(f_FaceCulling_FaceToCull), FaceCulling_FaceDetermination(f_FaceCulling_FaceDetermination),
DepthTest_Enabled(f_DepthTest_Enabled), DepthTest_WriteInBufferEnabled(f_DepthTest_WriteInBufferEnabled), DepthTest_TypeOfComparison(f_DepthTest_TypeOfComparison),
StencilTest_Enabled(f_StencilTest_Enabled), StencilTest_BaseMask(f_StencilTest_BaseMask), StencilTest_ComparisonType(f_StencilTest_ComparisonType),
StencilTest_ReferenceValue(f_StencilTest_ReferenceValue), StencilTest_Mask(f_StencilTest_Mask), StencilTest_ActionOnSF(f_StencilTest_ActionOnSF),
StencilTest_ActionOnSPDF(f_StencilTest_ActionOnSPDF), StencilTest_ActionOnSPDP(f_StencilTest_ActionOnSPDP),
Blending_Enabled(f_Blending_Enabled), Blending_ConstR(f_Blending_ConstR), Blending_ConstG(f_Blending_ConstG), Blending_ConstB(f_Blending_ConstG), Blending_ConstA(f_Blending_ConstA),
Blending_FuncForColor1(f_Blending_FuncForColor1), Blending_FuncForColor2(f_Blending_FuncForColor2),
ColorEnabledR(f_ColorEnabledR), ColorEnabledG(f_ColorEnabledG), ColorEnabledB(f_ColorEnabledB), ColorEnabledA(f_ColorEnabledA),
ClearR(f_ClearR), ClearG(f_ClearG), ClearB(f_ClearB), ClearA(f_ClearA)
{ }

RenderingPresetClass::RenderingPresetClass(const RenderingPresetClass& toCopy) {
	memcpy(this, &toCopy, sizeof(RenderingPresetClass));
}
void RenderingPresetClass::operator=(const RenderingPresetClass& toCopy) {
	memcpy(this, &toCopy, sizeof(RenderingPresetClass));
}


void RenderingPresetClass::sFaceCulling_Enabled(bool newVal, bool updOnGPU) { FaceCulling_Enabled = newVal; if (updOnGPU) UpdFaceCulling_Enabled(); }
void RenderingPresetClass::sFaceCulling_FaceToCull(RenderingPresetEnumArgumentsNamespace::FaceCulling::FaceToCull newVal, bool updOnGPU) { FaceCulling_FaceToCull = newVal; if (updOnGPU) UpdFaceCulling_FaceToCull(); }
void RenderingPresetClass::sFaceCulling_FaceDetermination(RenderingPresetEnumArgumentsNamespace::FaceCulling::FaceDetermination newVal, bool updOnGPU) { FaceCulling_FaceDetermination = newVal; if (updOnGPU) UpdFaceCulling_FaceDetermination(); }
void RenderingPresetClass::sDepthTest_Enabled(bool newVal, bool updOnGPU) { DepthTest_Enabled = newVal; if (updOnGPU) UpdDepthTest_Enabled(); }
void RenderingPresetClass::sDepthTest_WriteInBufferEnabled(bool newVal, bool updOnGPU) { DepthTest_WriteInBufferEnabled = newVal; if (updOnGPU) UpdDepthTest_WriteInBufferEnabled(); }
void RenderingPresetClass::sDepthTest_TypeOfComparison(RenderingPresetEnumArgumentsNamespace::DepthTest::TypeOfComparison newVal, bool updOnGPU) { DepthTest_TypeOfComparison = newVal; if (updOnGPU) UpdDepthTest_TypeOfComparison(); }
void RenderingPresetClass::sStencilTest_Enabled(bool newVal, bool updOnGPU) { StencilTest_Enabled = newVal; if (updOnGPU) UpdStencilTest_Enabled(); }
void RenderingPresetClass::sStencilTest_BaseMask(unsigned int newVal, bool updOnGPU) { StencilTest_BaseMask = newVal; if (updOnGPU) UpdStencilTest_BaseMask(); }
void RenderingPresetClass::sStencilTest_ComparisonType(RenderingPresetEnumArgumentsNamespace::StencilTest::TypeOfComparison newVal, bool updOnGPU) { StencilTest_ComparisonType = newVal; if (updOnGPU) UpdStencilTest_StencilFunc(); }
void RenderingPresetClass::sStencilTest_ReferenceValue(unsigned int newVal, bool updOnGPU) { StencilTest_ReferenceValue = newVal; if (updOnGPU) UpdStencilTest_StencilFunc(); }
void RenderingPresetClass::sStencilTest_Mask(unsigned int newVal, bool updOnGPU) { StencilTest_Mask = newVal; if (updOnGPU) UpdStencilTest_StencilFunc(); }
void RenderingPresetClass::sStencilTest_ActionOnSF(RenderingPresetEnumArgumentsNamespace::StencilTest::Actions newVal, bool updOnGPU) { StencilTest_ActionOnSF = newVal; if (updOnGPU) UpdStencilTest_StencilOpFunc(); }
void RenderingPresetClass::sStencilTest_ActionOnSPDF(RenderingPresetEnumArgumentsNamespace::StencilTest::Actions newVal, bool updOnGPU) { StencilTest_ActionOnSPDF = newVal; if (updOnGPU) UpdStencilTest_StencilOpFunc(); }
void RenderingPresetClass::sStencilTest_ActionOnSPDP(RenderingPresetEnumArgumentsNamespace::StencilTest::Actions newVal, bool updOnGPU) { StencilTest_ActionOnSPDP = newVal; if (updOnGPU) UpdStencilTest_StencilOpFunc(); }
void RenderingPresetClass::sBlending_Enabled(bool newVal, bool updOnGPU) { Blending_Enabled = newVal; if (updOnGPU) UpdBlending_Enabled(); }
void RenderingPresetClass::sBlending_ConstR(float newVal, bool updOnGPU) { Blending_ConstR = newVal; if (updOnGPU) UpdBlending_ConstRGBA(); }
void RenderingPresetClass::sBlending_ConstG(float newVal, bool updOnGPU) { Blending_ConstG = newVal; if (updOnGPU) UpdBlending_ConstRGBA(); }
void RenderingPresetClass::sBlending_ConstB(float newVal, bool updOnGPU) { Blending_ConstB = newVal; if (updOnGPU) UpdBlending_ConstRGBA(); }
void RenderingPresetClass::sBlending_ConstA(float newVal, bool updOnGPU) { Blending_ConstA = newVal; if (updOnGPU) UpdBlending_ConstRGBA(); }
void RenderingPresetClass::sBlending_FuncForColor1(RenderingPresetEnumArgumentsNamespace::Blending::FunctionForColor newVal, bool updOnGPU) { Blending_FuncForColor1 = newVal; if (updOnGPU) UpdBlending_FuncForColor(); }
void RenderingPresetClass::sBlending_FuncForColor2(RenderingPresetEnumArgumentsNamespace::Blending::FunctionForColor newVal, bool updOnGPU) { Blending_FuncForColor2 = newVal; if (updOnGPU) UpdBlending_FuncForColor(); }
void RenderingPresetClass::sColorEnabledR(bool newVal, bool updOnGPU) { ColorEnabledR = newVal; if (updOnGPU) UpdColorEnabledRGBA(); }
void RenderingPresetClass::sColorEnabledG(bool newVal, bool updOnGPU) { ColorEnabledG = newVal; if (updOnGPU) UpdColorEnabledRGBA(); }
void RenderingPresetClass::sColorEnabledB(bool newVal, bool updOnGPU) { ColorEnabledB = newVal; if (updOnGPU) UpdColorEnabledRGBA(); }
void RenderingPresetClass::sColorEnabledA(bool newVal, bool updOnGPU) { ColorEnabledA = newVal; if (updOnGPU) UpdColorEnabledRGBA(); }
void RenderingPresetClass::sClearR(float newVal, bool updOnGPU) { ClearR = newVal; if (updOnGPU) UpdClearRGBA(); }
void RenderingPresetClass::sClearG(float newVal, bool updOnGPU) { ClearG = newVal; if (updOnGPU) UpdClearRGBA(); }
void RenderingPresetClass::sClearB(float newVal, bool updOnGPU) { ClearB = newVal; if (updOnGPU) UpdClearRGBA(); }
void RenderingPresetClass::sClearA(float newVal, bool updOnGPU) { ClearA = newVal; if (updOnGPU) UpdClearRGBA(); }


void RenderingPresetClass::UpdFaceCulling_Enabled() {
	switch (FaceCulling_Enabled) {
	case true: glSC(glEnable(GL_CULL_FACE)); break;
	case false: glSC(glDisable(GL_CULL_FACE)); break;
	}
}
void RenderingPresetClass::UpdFaceCulling_FaceToCull() {
	switch (FaceCulling_FaceToCull) {
	case RenderingPresetEnumArgumentsNamespace::FaceCulling::FaceToCull::Back: glSC(glCullFace(GL_BACK)); break;
	case RenderingPresetEnumArgumentsNamespace::FaceCulling::FaceToCull::Front: glSC(glCullFace(GL_FRONT)); break;
	}
}
void RenderingPresetClass::UpdFaceCulling_FaceDetermination() {
	switch (FaceCulling_FaceDetermination) {
	case RenderingPresetEnumArgumentsNamespace::FaceCulling::FaceDetermination::Clockwise: glSC(glFrontFace(GL_CW)); break;
	case RenderingPresetEnumArgumentsNamespace::FaceCulling::FaceDetermination::AntiClockwise:  glSC(glFrontFace(GL_CCW)); break;
	}
}

void RenderingPresetClass::UpdDepthTest_Enabled() {
	switch (DepthTest_Enabled) {
	case true: glSC(glEnable(GL_DEPTH_TEST)); break;
	case false: glSC(glDisable(GL_DEPTH_TEST)); break;
	}
}
void RenderingPresetClass::UpdDepthTest_WriteInBufferEnabled() {
	switch (DepthTest_WriteInBufferEnabled) {
	case true: glSC(glDepthMask(GL_TRUE)); break;
	case false: glSC(glDepthMask(GL_FALSE)); break;
	}
}
void RenderingPresetClass::UpdDepthTest_TypeOfComparison() {
	switch (DepthTest_TypeOfComparison) {
	case RenderingPresetEnumArgumentsNamespace::DepthTest::TypeOfComparison::Never: glSC(glDepthFunc(GL_NEVER)); break;
	case RenderingPresetEnumArgumentsNamespace::DepthTest::TypeOfComparison::Less: glSC(glDepthFunc(GL_LESS)); break;
	case RenderingPresetEnumArgumentsNamespace::DepthTest::TypeOfComparison::LessOrEqual: glSC(glDepthFunc(GL_LEQUAL)); break;
	case RenderingPresetEnumArgumentsNamespace::DepthTest::TypeOfComparison::Greater: glSC(glDepthFunc(GL_GREATER)); break;
	case RenderingPresetEnumArgumentsNamespace::DepthTest::TypeOfComparison::GreaterOrEqual: glSC(glDepthFunc(GL_GEQUAL)); break;
	case RenderingPresetEnumArgumentsNamespace::DepthTest::TypeOfComparison::Equal: glSC(glDepthFunc(GL_EQUAL)); break;
	case RenderingPresetEnumArgumentsNamespace::DepthTest::TypeOfComparison::NotEqual: glSC(glDepthFunc(GL_NOTEQUAL)); break;
	case RenderingPresetEnumArgumentsNamespace::DepthTest::TypeOfComparison::AlwaysPass: glSC(glDepthFunc(GL_ALWAYS)); break;
	}
}

void RenderingPresetClass::UpdStencilTest_Enabled() {
	switch (StencilTest_Enabled) {
	case true: glSC(glEnable(GL_STENCIL_TEST)); break;
	case false: glSC(glDisable(GL_STENCIL_TEST)); break;
	}
}
void RenderingPresetClass::UpdStencilTest_BaseMask() {
	glSC(glStencilMask(StencilTest_BaseMask));
}
void RenderingPresetClass::UpdStencilTest_StencilFunc() {
	int func = 0;
	switch (StencilTest_ComparisonType) {
	case RenderingPresetEnumArgumentsNamespace::StencilTest::TypeOfComparison::Never: func = GL_NEVER; break;
	case RenderingPresetEnumArgumentsNamespace::StencilTest::TypeOfComparison::Less: func = GL_LESS; break;
	case RenderingPresetEnumArgumentsNamespace::StencilTest::TypeOfComparison::LessOrEqual: func = GL_LEQUAL; break;
	case RenderingPresetEnumArgumentsNamespace::StencilTest::TypeOfComparison::Greater: func = GL_GREATER; break;
	case RenderingPresetEnumArgumentsNamespace::StencilTest::TypeOfComparison::GreaterOrEqual: func = GL_GEQUAL; break;
	case RenderingPresetEnumArgumentsNamespace::StencilTest::TypeOfComparison::Equal: func = GL_EQUAL; break;
	case RenderingPresetEnumArgumentsNamespace::StencilTest::TypeOfComparison::NotEqual: func = GL_NOTEQUAL; break;
	case RenderingPresetEnumArgumentsNamespace::StencilTest::TypeOfComparison::AlwaysPass: func = GL_ALWAYS; break;
	}
	glSC(glStencilFunc(func, StencilTest_ReferenceValue, StencilTest_Mask));

}
void RenderingPresetClass::UpdStencilTest_StencilOpFunc() {
	int args[3] = { 0,0,0 };
	int checkVal[3] = { (int)StencilTest_ActionOnSF,(int)StencilTest_ActionOnSPDF,(int)StencilTest_ActionOnSPDP };
	for (unsigned int i = 0; i < 3; i++) {
		switch (checkVal[i]) {
		case (int)RenderingPresetEnumArgumentsNamespace::StencilTest::Actions::Keep: args[i] = GL_KEEP; break;
		case (int)RenderingPresetEnumArgumentsNamespace::StencilTest::Actions::Zero: args[i] = GL_ZERO; break;
		case (int)RenderingPresetEnumArgumentsNamespace::StencilTest::Actions::Replace: args[i] = GL_REPLACE; break;
		case (int)RenderingPresetEnumArgumentsNamespace::StencilTest::Actions::Increase: args[i] = GL_INCR; break;
		case (int)RenderingPresetEnumArgumentsNamespace::StencilTest::Actions::IncreaseWrap: args[i] = GL_INCR_WRAP; break;
		case (int)RenderingPresetEnumArgumentsNamespace::StencilTest::Actions::Decrease: args[i] = GL_DECR; break;
		case (int)RenderingPresetEnumArgumentsNamespace::StencilTest::Actions::DecreaseWrap: args[i] = GL_DECR_WRAP; break;
		case (int)RenderingPresetEnumArgumentsNamespace::StencilTest::Actions::Invert: args[i] = GL_INVERT; break;
		}
	} glSC(glStencilOp(args[0], args[1], args[2]));
}

void RenderingPresetClass::UpdBlending_Enabled() {
	switch (Blending_Enabled) {
	case true: glSC(glEnable(GL_BLEND)); break;
	case false: glSC(glDisable(GL_BLEND)); break;
	}
}
void RenderingPresetClass::UpdBlending_ConstRGBA() {
	glSC(glBlendColor(Blending_ConstR, Blending_ConstG, Blending_ConstB, Blending_ConstA));
}
void RenderingPresetClass::UpdBlending_FuncForColor() {
	int args[2] = { 0,0 };
	int checkVal[2] = { (int)Blending_FuncForColor1,(int)Blending_FuncForColor2 };
	for (unsigned int i = 0; i < 2; i++) {
		switch (checkVal[i]) {
		case (int)RenderingPresetEnumArgumentsNamespace::Blending::FunctionForColor::Zero: args[i] = GL_ZERO; break;
		case (int)RenderingPresetEnumArgumentsNamespace::Blending::FunctionForColor::One: args[i] = GL_ONE; break;
		case (int)RenderingPresetEnumArgumentsNamespace::Blending::FunctionForColor::SrcColor: args[i] = GL_SRC_COLOR; break;
		case (int)RenderingPresetEnumArgumentsNamespace::Blending::FunctionForColor::OneMinusSrcColor: args[i] = GL_ONE_MINUS_SRC_COLOR; break;
		case (int)RenderingPresetEnumArgumentsNamespace::Blending::FunctionForColor::DstColor: args[i] = GL_DST_COLOR; break;
		case (int)RenderingPresetEnumArgumentsNamespace::Blending::FunctionForColor::OneMinusDstColor: args[i] = GL_ONE_MINUS_DST_COLOR; break;
		case (int)RenderingPresetEnumArgumentsNamespace::Blending::FunctionForColor::SrcAlpha: args[i] = GL_SRC_ALPHA; break;
		case (int)RenderingPresetEnumArgumentsNamespace::Blending::FunctionForColor::OneMinusSrcAlpha: args[i] = GL_ONE_MINUS_SRC_ALPHA; break;
		case (int)RenderingPresetEnumArgumentsNamespace::Blending::FunctionForColor::DstAlpha: args[i] = GL_DST_ALPHA; break;
		case (int)RenderingPresetEnumArgumentsNamespace::Blending::FunctionForColor::OneMinusDstAlpha: args[i] = GL_ONE_MINUS_DST_ALPHA; break;
		case (int)RenderingPresetEnumArgumentsNamespace::Blending::FunctionForColor::ConstColor: args[i] = GL_CONSTANT_COLOR; break;
		case (int)RenderingPresetEnumArgumentsNamespace::Blending::FunctionForColor::OneMinusConstColor: args[i] = GL_ONE_MINUS_CONSTANT_COLOR; break;
		case (int)RenderingPresetEnumArgumentsNamespace::Blending::FunctionForColor::ConstAlpha: args[i] = GL_CONSTANT_ALPHA; break;
		case (int)RenderingPresetEnumArgumentsNamespace::Blending::FunctionForColor::OneMinusConstAlpha: args[i] = GL_ONE_MINUS_CONSTANT_ALPHA; break;
		case (int)RenderingPresetEnumArgumentsNamespace::Blending::FunctionForColor::SrcAlphaSaturate: args[i] = GL_SRC_ALPHA_SATURATE; break;
		}
	}
	glSC(glBlendFunc(args[0], args[1]));
}
void RenderingPresetClass::UpdColorEnabledRGBA() {
	glSC(glColorMask(ColorEnabledR, ColorEnabledG, ColorEnabledB, ColorEnabledA));
}
void RenderingPresetClass::UpdClearRGBA() {
	glSC(glClearColor(ClearR, ClearG, ClearB, ClearA));
}

void RenderingPresetClass::Bind() {
	
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

	UpdClearRGBA();

}
void RenderingPresetClass::Unbind() {
	glSC(glDisable(GL_CULL_FACE));
	glSC(glDisable(GL_BLEND));
	glSC(glDisable(GL_DEPTH_TEST));
	glSC(glDisable(GL_STENCIL_TEST));
}