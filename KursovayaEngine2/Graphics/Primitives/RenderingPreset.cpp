#include"RenderingPreset.h"
#include"glad/glad.h"
#include"Tools/GLDebug.h"

using namespace KE2;
using namespace Graphics::Primitives;


void RenderingPresetS::ColorsS::UpdEnabledRGBA(Vector4B colorEnabledRGBA) {
	glSC(glColorMask(colorEnabledRGBA[0], colorEnabledRGBA[1], colorEnabledRGBA[2], colorEnabledRGBA[3]));
}
void RenderingPresetS::ColorsS::UpdClearRGBA(Vector4F clearRGBA) {
	glSC(glClearColor(clearRGBA[0], clearRGBA[1], clearRGBA[2], clearRGBA[3]));
}
void RenderingPresetS::ColorsS::UpdColorsOutputs(const ArrayView<unsigned>& colorsOutputs) {
	std::vector<unsigned> buffs = colorsOutputs;
	for (unsigned& elem : buffs) {
		if (elem == 0) elem = GL_NONE;
		else elem += GL_COLOR_ATTACHMENT0 - 1;
	}
	glSC(glDrawBuffers((int)buffs.size(), &*buffs.begin()));
}
void RenderingPresetS::BlendingS::UpdEnabled(bool enabled) {
	if (enabled) { glSC(glEnable(GL_BLEND)); }
	else { glSC(glDisable(GL_BLEND)); }
}
void RenderingPresetS::BlendingS::UpdConstRGBA(Vector4F constRGBA) {
	glSC(glBlendColor(constRGBA[0], constRGBA[1], constRGBA[2], constRGBA[3]));
}
void RenderingPresetS::BlendingS::UpdBlendingFunc(BlendingFuncValE srcVal, BlendingFuncValE dstVal) {
	unsigned args[2] = { 0u,0u };
	BlendingFuncValE checkVal[2] = { srcVal,dstVal };
	for (unsigned int i = 0; i < 2; i++) {
		switch (checkVal[i]) {
		case BlendingFuncValE::Zero: args[i] = GL_ZERO; break;
		case BlendingFuncValE::One: args[i] = GL_ONE; break;
		case BlendingFuncValE::Src0Color: args[i] = GL_SRC_COLOR; break;
		case BlendingFuncValE::OneMinusSrc0Color: args[i] = GL_ONE_MINUS_SRC_COLOR; break;
		case BlendingFuncValE::DstColor: args[i] = GL_DST_COLOR; break;
		case BlendingFuncValE::OneMinusDstColor: args[i] = GL_ONE_MINUS_DST_COLOR; break;
		case BlendingFuncValE::Src0Alpha: args[i] = GL_SRC_ALPHA; break;
		case BlendingFuncValE::OneMinusSrc0Alpha: args[i] = GL_ONE_MINUS_SRC_ALPHA; break;
		case BlendingFuncValE::DstAlpha: args[i] = GL_DST_ALPHA; break;
		case BlendingFuncValE::OneMinusDstAlpha: args[i] = GL_ONE_MINUS_DST_ALPHA; break;
		case BlendingFuncValE::ConstColor: args[i] = GL_CONSTANT_COLOR; break;
		case BlendingFuncValE::OneMinusConstColor: args[i] = GL_ONE_MINUS_CONSTANT_COLOR; break;
		case BlendingFuncValE::ConstAlpha: args[i] = GL_CONSTANT_ALPHA; break;
		case BlendingFuncValE::OneMinusConstAlpha: args[i] = GL_ONE_MINUS_CONSTANT_ALPHA; break;
		case BlendingFuncValE::SrcAlphaSaturate: args[i] = GL_SRC_ALPHA_SATURATE; break;
		case BlendingFuncValE::Src1Color: args[i] = GL_SRC1_COLOR; break;
		case BlendingFuncValE::OneMinusSrc1Color: args[i] = GL_ONE_MINUS_SRC1_COLOR; break;
		case BlendingFuncValE::Src1Alpha: args[i] = GL_SRC1_ALPHA; break;
		case BlendingFuncValE::OneMinusSrc1Alpha: args[i] = GL_ONE_MINUS_SRC1_ALPHA; break;
		}
	}
	glSC(glBlendFunc(args[0], args[1]));
}

void RenderingPresetS::FaceCullingS::UpdEnabled(bool enabled) {
	if (enabled) { glSC(glEnable(GL_CULL_FACE)); }
	else { glSC(glDisable(GL_CULL_FACE)); }
}
void RenderingPresetS::FaceCullingS::UpdFaceToCull(FaceToCullE faceToCull) {
	switch (faceToCull) {
	case FaceToCullE::Back: glSC(glCullFace(GL_BACK)); break;
	case FaceToCullE::Front: glSC(glCullFace(GL_FRONT)); break;
	}
}
void RenderingPresetS::FaceCullingS::UpdFaceDetermination(FaceDeterminationE faceDetermination) {
	switch (faceDetermination) {
	case FaceDeterminationE::Clockwise: glSC(glFrontFace(GL_CW)); break;
	case FaceDeterminationE::AntiClockwise:  glSC(glFrontFace(GL_CCW)); break;
	}
}

void RenderingPresetS::DepthTestS::UpdEnabled(bool enabled) {
	if (enabled) { glSC(glEnable(GL_DEPTH_TEST)); }
	else {glSC(glDisable(GL_DEPTH_TEST)) }
}
void RenderingPresetS::DepthTestS::UpdWriteInBufferEnabled(bool writeInBufferEnabled) {
	if (writeInBufferEnabled) { glSC(glDepthMask(GL_TRUE)); }
	else { glSC(glDepthMask(GL_FALSE)); }
}
void RenderingPresetS::DepthTestS::UpdTypeOfComparison(TypeOfComparisonE typeOfComparison) {
	switch (typeOfComparison) {
	case TypeOfComparisonE::Never: glSC(glDepthFunc(GL_NEVER)); break;
	case TypeOfComparisonE::Less: glSC(glDepthFunc(GL_LESS)); break;
	case TypeOfComparisonE::LessOrEqual: glSC(glDepthFunc(GL_LEQUAL)); break;
	case TypeOfComparisonE::Greater: glSC(glDepthFunc(GL_GREATER)); break;
	case TypeOfComparisonE::GreaterOrEqual: glSC(glDepthFunc(GL_GEQUAL)); break;
	case TypeOfComparisonE::Equal: glSC(glDepthFunc(GL_EQUAL)); break;
	case TypeOfComparisonE::NotEqual: glSC(glDepthFunc(GL_NOTEQUAL)); break;
	case TypeOfComparisonE::AlwaysPass: glSC(glDepthFunc(GL_ALWAYS)); break;
	}
}

void RenderingPresetS::StencilTestS::UpdEnabled(bool enabled) {
	if (enabled) { glSC(glEnable(GL_STENCIL_TEST)); }
	else { glSC(glDisable(GL_STENCIL_TEST)); }
}
void RenderingPresetS::StencilTestS::UpdBaseMask(unsigned int baseMask) {
	glSC(glStencilMask(baseMask));
}
void RenderingPresetS::StencilTestS::UpdComparisonFunc(ComparisonTypeE comparisonType, unsigned int referenceValue, unsigned int mask) {
	unsigned func = 0u;
	switch (comparisonType) {
	case ComparisonTypeE::Never: func = GL_NEVER; break;
	case ComparisonTypeE::Less: func = GL_LESS; break;
	case ComparisonTypeE::LessOrEqual: func = GL_LEQUAL; break;
	case ComparisonTypeE::Greater: func = GL_GREATER; break;
	case ComparisonTypeE::GreaterOrEqual: func = GL_GEQUAL; break;
	case ComparisonTypeE::Equal: func = GL_EQUAL; break;
	case ComparisonTypeE::NotEqual: func = GL_NOTEQUAL; break;
	case ComparisonTypeE::AlwaysPass: func = GL_ALWAYS; break;
	}
	glSC(glStencilFunc(func, referenceValue, mask));
}
void RenderingPresetS::StencilTestS::UpdActionFunc(ActionsE actionOnSF, ActionsE actionOnSPDF, ActionsE actionOnSPDP) {
	int args[3] = { 0,0,0 };
	ActionsE checkVal[3] = { actionOnSF,actionOnSPDF,actionOnSPDP };
	for (unsigned i = 0; i < 3; i++) {
		switch (checkVal[i]) {
		case ActionsE::Keep: args[i] = GL_KEEP; break;
		case ActionsE::Zero: args[i] = GL_ZERO; break;
		case ActionsE::Replace: args[i] = GL_REPLACE; break;
		case ActionsE::Increase: args[i] = GL_INCR; break;
		case ActionsE::IncreaseWrap: args[i] = GL_INCR_WRAP; break;
		case ActionsE::Decrease: args[i] = GL_DECR; break;
		case ActionsE::DecreaseWrap: args[i] = GL_DECR_WRAP; break;
		case ActionsE::Invert: args[i] = GL_INVERT; break;
		}
	} glSC(glStencilOp(args[0], args[1], args[2]));
}
void RenderingPresetS::LogicalOperationS::UpdEnabled(bool enabled) {
	if (enabled) { glSC(glEnable(GL_COLOR_LOGIC_OP)); }
	else { glSC(glDisable(GL_COLOR_LOGIC_OP)); }
}
void RenderingPresetS::LogicalOperationS::UpdOper(OperE oper) {
	switch (oper) {
	case OperE::Zero: glSC(glLogicOp(GL_CLEAR)); break;
	case OperE::One: glSC(glLogicOp(GL_SET)); break;
	case OperE::S: glSC(glLogicOp(GL_COPY)); break;
	case OperE::IS: glSC(glLogicOp(GL_COPY_INVERTED)); break;
	case OperE::D: glSC(glLogicOp(GL_NOOP)); break;
	case OperE::ID: glSC(glLogicOp(GL_INVERT)); break;
	case OperE::SAD: glSC(glLogicOp(GL_AND)); break;
	case OperE::I_SAD: glSC(glLogicOp(GL_NAND)); break;
	case OperE::SOD: glSC(glLogicOp(GL_OR)); break;
	case OperE::I_SOD: glSC(glLogicOp(GL_NOR)); break;
	case OperE::SXD: glSC(glLogicOp(GL_XOR)); break;
	case OperE::I_SXD: glSC(glLogicOp(GL_EQUIV)); break;
	case OperE::SAID: glSC(glLogicOp(GL_AND_REVERSE)); break;
	case OperE::ISAD: glSC(glLogicOp(GL_AND_INVERTED)); break;
	case OperE::SOID: glSC(glLogicOp(GL_OR_REVERSE)); break;
	case OperE::ISOD: glSC(glLogicOp(GL_OR_INVERTED)); break;
	}
}
void RenderingPresetS::ScissorTestS::UpdEnabled(bool enabled) {
	if (enabled) { glSC(glEnable(GL_SCISSOR_TEST)); }
	else { glSC(glDisable(GL_SCISSOR_TEST)); }
}
void RenderingPresetS::ScissorTestS::UpdOffsetAndBase(Vector2U offset, Vector2U size) {
	glSC(glScissor(offset[0], offset[1], size[0], size[1]));
}

