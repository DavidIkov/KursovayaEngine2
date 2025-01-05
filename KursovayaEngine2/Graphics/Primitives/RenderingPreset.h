#pragma once
#include"DLL.h"
#include"Tools/ArrayView.h"
#include"Maths/Vector.h"

namespace KE2::Graphics::Primitives {
	//this thing can save and quickly apply settings for rendering, such as cull faces, depth test, blending, etc...
	struct RenderingPresetS {

		struct ColorsS {
			//if all colors disabled, then fragment shader wont be called
			Vector4B EnabledRGBA = Vector4B(true);
			Vector4F ClearRGBA = Vector4F(0.f);
			//every output color attachment value should be +1, so if value is 0 it means that output is not enabled
			std::vector<unsigned> ColorsOutputs;
			
			DLLTREATMENT static void UpdEnabledRGBA(Vector4B colorEnabledRGBA);
			DLLTREATMENT static void UpdClearRGBA(Vector4F clearRGBA);
			DLLTREATMENT static void UpdColorsOutputs(const ArrayView<unsigned>& colorsOutputs);

			struct FlagsS { static constexpr int
					EnabledRGBA = 1 << 0, ClearRGBA = 1 << 1, ColorsOutputs = 1 << 2,
					All = EnabledRGBA | ClearRGBA | ColorsOutputs; } Flags;
		} Colors;

		struct BlendingS{
			bool Enabled = false;
			Vector4F ConstRGBA = Vector4F(0.f);
			//for full explanation visit "https://docs.gl/gl4/glBlendFunc"
			enum class BlendingFuncValE :char {
				Zero, One, Src0Color, OneMinusSrc0Color, DstColor, OneMinusDstColor, Src0Alpha, OneMinusSrc0Alpha,
				DstAlpha, OneMinusDstAlpha, ConstColor, OneMinusConstColor, ConstAlpha, OneMinusConstAlpha,
				SrcAlphaSaturate, Src1Color, OneMinusSrc1Color, Src1Alpha, OneMinusSrc1Alpha
			} SrcVal = BlendingFuncValE ::One, DstVal = BlendingFuncValE ::Zero;

			DLLTREATMENT static void UpdEnabled(bool enabled);
			DLLTREATMENT static void UpdConstRGBA(Vector4F constRGBA);
			DLLTREATMENT static void UpdBlendingFunc(BlendingFuncValE srcVal, BlendingFuncValE dstVal);

			struct FlagsS { static constexpr int
					Enabled = 1 << 3, ConstRGBA = 1 << 4, BlendingFunc = 1 << 5,
					All = Enabled | ConstRGBA | BlendingFunc; } Flags;
		} Blending;
		
		struct FaceCullingS {
			bool Enabled = false;
			//describes which face to cull
			enum class FaceToCullE :char { Back, Front } FaceToCull = FaceToCullE::Back;
			/*describe how to determine fs face front or back,
			to determine that, gpu takes position of 3 vertexes of triangle from camera perspective
			and then IF they positioned clockwise and setting is set to clockwise then its front face,
			and IF they positioned anticlockwise then its back face, if settings is set to anticlockwise then everything is same but reversed*/
			enum class FaceDeterminationE :char { Clockwise, AntiClockwise, } FaceDetermination = FaceDeterminationE::AntiClockwise;

			DLLTREATMENT static void UpdEnabled(bool enabled);
			DLLTREATMENT static void UpdFaceToCull(FaceToCullE faceToCull);
			DLLTREATMENT static void UpdFaceDetermination(FaceDeterminationE faceDetermination);

			struct FlagsS {
				static constexpr int Enabled = 1 << 6, FaceToCull = 1 << 7, FaceDetermination = 1 << 8,
					All = Enabled | FaceToCull | FaceDetermination; } Flags;
		} FaceCulling;

		struct DepthTestS {
			bool Enabled = false, WriteInBufferEnabled = true;
			//gpu will compare two values, current value and stored value, if test passes then pixel will be drawn, otherwise it will be discarded
			enum class TypeOfComparisonE :char {
				Never, Less, LessOrEqual, Greater, GreaterOrEqual, Equal, NotEqual, AlwaysPass
			} TypeOfComparison = TypeOfComparisonE::Less;

			DLLTREATMENT static void UpdEnabled(bool enabled);
			DLLTREATMENT static void UpdWriteInBufferEnabled(bool writeInBufferEnabled);
			DLLTREATMENT static void UpdTypeOfComparison(TypeOfComparisonE typeOfComparison);

			struct FlagsS {
				static constexpr int Enabled = 1 << 9, WriteInBufferEnabled = 1 << 10, TypeOfComparison = 1 << 11,
					All = Enabled | WriteInBufferEnabled | TypeOfComparison; } Flags;
		} DepthTest;

		struct StencilTestS {
			bool Enabled = false;
			unsigned int BaseMask = ~0u;
			//gpu will compare two values, if comparison fails then pixel is discarded, otherwise it wont
			enum class ComparisonTypeE :char {
				Never, Less, LessOrEqual, Greater, GreaterOrEqual, Equal, NotEqual, AlwaysPass
			} ComparisonType = ComparisonTypeE::AlwaysPass;
			unsigned int ReferenceValue = 0u, Mask = ~0u;
			//actions to take with current stencil buffer value on different events that may occur, for move info visit "https://docs.gl/gl3/glStencilOp"
			enum class ActionsE :char {
				Keep, Zero, Replace, Increase, IncreaseWrap, Decrease, DecreaseWrap, Invert
			} ActionOnSF = ActionsE::Keep, ActionOnSPDF = ActionsE::Keep, ActionOnSPDP = ActionsE::Keep;

			DLLTREATMENT static void UpdEnabled(bool enabled);
			DLLTREATMENT static void UpdBaseMask(unsigned int baseMask);
			DLLTREATMENT static void UpdComparisonFunc(ComparisonTypeE comparisonType, unsigned int referenceValue, unsigned int mask);
			DLLTREATMENT static void UpdActionFunc(ActionsE actionOnSF, ActionsE actionOnSPDF, ActionsE actionOnSPDP);

			struct FlagsS { static constexpr int
					Enabled = 1 << 12, BaseMask = 1 << 13, ComparisonFunc = 1 << 14, ActionFunc = 1 << 15,
					All = Enabled | BaseMask | ComparisonFunc | ActionFunc; } Flags;
		} StencilTest;
		//TODO add ability to set stencil test for Front and Back faces, for example glStencilMaskSeparate

		void Update(int flags = ~0) {
			if (flags & ColorsS::FlagsS::EnabledRGBA) ColorsS::UpdEnabledRGBA(Colors.EnabledRGBA);
			if (flags & ColorsS::FlagsS::ClearRGBA) ColorsS::UpdClearRGBA(Colors.ClearRGBA);
			if (flags & ColorsS::FlagsS::ColorsOutputs) ColorsS::UpdColorsOutputs(Colors.ColorsOutputs);

			if (flags & BlendingS::FlagsS::Enabled) BlendingS::UpdEnabled(Blending.Enabled);
			if (flags & BlendingS::FlagsS::ConstRGBA) BlendingS::UpdConstRGBA(Blending.ConstRGBA);
			if (flags & BlendingS::FlagsS::BlendingFunc) BlendingS::UpdBlendingFunc(Blending.SrcVal, Blending.DstVal);

			if (flags & FaceCullingS::FlagsS::Enabled) FaceCullingS::UpdEnabled(FaceCulling.Enabled);
			if (flags & FaceCullingS::FlagsS::FaceToCull) FaceCullingS::UpdFaceToCull(FaceCulling.FaceToCull);
			if (flags & FaceCullingS::FlagsS::FaceDetermination) FaceCullingS::UpdFaceDetermination(FaceCulling.FaceDetermination);

			if (flags & DepthTestS::FlagsS::Enabled) DepthTestS::UpdEnabled(DepthTest.Enabled);
			if (flags & DepthTestS::FlagsS::WriteInBufferEnabled) DepthTestS::UpdWriteInBufferEnabled(DepthTest.WriteInBufferEnabled);
			if (flags & DepthTestS::FlagsS::TypeOfComparison) DepthTestS::UpdTypeOfComparison(DepthTest.TypeOfComparison);

			if (flags & StencilTestS::FlagsS::Enabled) StencilTestS::UpdEnabled(StencilTest.Enabled);
			if (flags & StencilTestS::FlagsS::BaseMask) StencilTestS::UpdBaseMask(StencilTest.BaseMask);
			if (flags & StencilTestS::FlagsS::ComparisonFunc) StencilTestS::UpdComparisonFunc(StencilTest.ComparisonType, StencilTest.ReferenceValue, StencilTest.Mask);
			if (flags & StencilTestS::FlagsS::ActionFunc) StencilTestS::UpdActionFunc(StencilTest.ActionOnSF, StencilTest.ActionOnSPDF, StencilTest.ActionOnSPDP);
		}
	};
}