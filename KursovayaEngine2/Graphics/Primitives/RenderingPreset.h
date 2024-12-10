#pragma once
#include"DLL.h"

namespace KE2::Graphics::Primitives {

	namespace RenderingPresetEnumArgumentsNamespace {

		//used to not render vertexes which cant be seen since they facing away from the camera
		namespace FaceCulling {
			//describes which face to cull
			enum class FaceToCull :unsigned short int {
				Back, Front
			};
			/*describe how to determine fs face front or back,
			to determine that, gpu takes position of 3 vertexes of triangle from camera perspective
			and then IF they positioned clockwise and setting is set to clockwise then its front face,
			and IF they positioned anticlockwise then its back face, if settings is set to anticlockwise then everything is same but reversed*/
			enum class FaceDetermination :unsigned short int {
				Clockwise, AntiClockwise,
			};
		}

		//used to determine if object is behind some other object or nah
		namespace DepthTest {
			//gpu will compare two values, current value and stored value, if test passes then pixel will be drawn, otherwise it will be discarded
			enum class TypeOfComparison :unsigned short int {
				Never, Less, LessOrEqual, Greater, GreaterOrEqual, Equal, NotEqual, AlwaysPass
			};
		}

		//used to discard some elements from screen, can be used for outlining
		namespace StencilTest {
			//gpu will comparison two values, if comparison fails then pixel is discarder, othervise it wont
			enum class TypeOfComparison :unsigned short int {
				Never, Less, LessOrEqual, Greater, GreaterOrEqual, Equal, NotEqual, AlwaysPass
			};
			//actions to take with current stencil buffer value on different events that may occur, for move info visit "https://docs.gl/gl3/glStencilOp"
			enum class Actions :unsigned short int {
				Keep, Zero, Replace, Increase, IncreaseWrap, Decrease, DecreaseWrap, Invert
			};
		}
		//correspond to blending color, depending on alpha value of RGBA
		namespace Blending {
			/*Src-Source, Dst-Destination, Const-Constant(this is basically a blending color which you can set)
			this enum correspond which function gpu should use when it need to blend somnething,
			for more technical info visit "https://docs.gl/gl4/glBlendFunc" since there is a good explanation
			on this topic and i dont see why i should copy it in here since it will be same but with different names
			TODO: make thing named "dual source blending" since glBlendFunc have arguments where second source mentioned*/
			enum class FunctionForColor :unsigned short int {
				Zero, One, SrcColor, OneMinusSrcColor, DstColor, OneMinusDstColor, SrcAlpha, OneMinusSrcAlpha,
				DstAlpha, OneMinusDstAlpha, ConstColor, OneMinusConstColor, ConstAlpha, OneMinusConstAlpha,
				SrcAlphaSaturate,
			};
		}


	};

	//this thing can save and quickly apply settings for rendering, such as cull faces, depth test, blending, etc...
	class RenderingPresetClass {
		bool FaceCulling_Enabled;
		RenderingPresetEnumArgumentsNamespace::FaceCulling::FaceToCull FaceCulling_FaceToCull;
		RenderingPresetEnumArgumentsNamespace::FaceCulling::FaceDetermination FaceCulling_FaceDetermination;

		bool DepthTest_Enabled;
		bool DepthTest_WriteInBufferEnabled;
		RenderingPresetEnumArgumentsNamespace::DepthTest::TypeOfComparison DepthTest_TypeOfComparison;

		bool StencilTest_Enabled;
		unsigned short int StencilTest_BaseMask;
		RenderingPresetEnumArgumentsNamespace::StencilTest::TypeOfComparison StencilTest_ComparisonType;
		unsigned short int StencilTest_ReferenceValue;
		unsigned short int StencilTest_Mask;
		RenderingPresetEnumArgumentsNamespace::StencilTest::Actions StencilTest_ActionOnSF;
		RenderingPresetEnumArgumentsNamespace::StencilTest::Actions StencilTest_ActionOnSPDF;
		RenderingPresetEnumArgumentsNamespace::StencilTest::Actions StencilTest_ActionOnSPDP;

		bool Blending_Enabled;
		//TODO:make RGBA class and make it work in here
		float Blending_ConstR;
		float Blending_ConstG;
		float Blending_ConstB;
		float Blending_ConstA;
		RenderingPresetEnumArgumentsNamespace::Blending::FunctionForColor Blending_FuncForColor1;
		RenderingPresetEnumArgumentsNamespace::Blending::FunctionForColor Blending_FuncForColor2;

		float ClearR;
		float ClearG;
		float ClearB;
		float ClearA;

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

		void UpdClearRGBA();

	public:

		DLLTREATMENT void sFaceCulling_Enabled(bool newVal);
		DLLTREATMENT void sFaceCulling_FaceToCull(RenderingPresetEnumArgumentsNamespace::FaceCulling::FaceToCull newVal);
		DLLTREATMENT void sFaceCulling_FaceDetermination(RenderingPresetEnumArgumentsNamespace::FaceCulling::FaceDetermination newVal);

		DLLTREATMENT void sDepthTest_Enabled(bool newVal);
		DLLTREATMENT void sDepthTest_WriteInBufferEnabled(bool newVal);
		DLLTREATMENT void sDepthTest_TypeOfComparison(RenderingPresetEnumArgumentsNamespace::DepthTest::TypeOfComparison newVal);

		DLLTREATMENT void sStencilTest_Enabled(bool newVal);
		DLLTREATMENT void sStencilTest_BaseMask(unsigned int newVal);
		DLLTREATMENT void sStencilTest_ComparisonType(RenderingPresetEnumArgumentsNamespace::StencilTest::TypeOfComparison newVal);
		DLLTREATMENT void sStencilTest_ReferenceValue(unsigned int newVal);
		DLLTREATMENT void sStencilTest_Mask(unsigned int newVal);
		DLLTREATMENT void sStencilTest_ActionOnSF(RenderingPresetEnumArgumentsNamespace::StencilTest::Actions newVal);
		DLLTREATMENT void sStencilTest_ActionOnSPDF(RenderingPresetEnumArgumentsNamespace::StencilTest::Actions newVal);
		DLLTREATMENT void sStencilTest_ActionOnSPDP(RenderingPresetEnumArgumentsNamespace::StencilTest::Actions newVal);

		DLLTREATMENT void sBlending_Enabled(bool newVal);
		DLLTREATMENT void sBlending_ConstR(float newVal);
		DLLTREATMENT void sBlending_ConstG(float newVal);
		DLLTREATMENT void sBlending_ConstB(float newVal);
		DLLTREATMENT void sBlending_ConstA(float newVal);
		DLLTREATMENT void sBlending_FuncForColor1(RenderingPresetEnumArgumentsNamespace::Blending::FunctionForColor newVal);
		DLLTREATMENT void sBlending_FuncForColor2(RenderingPresetEnumArgumentsNamespace::Blending::FunctionForColor newVal);

		DLLTREATMENT void sClearR(float newVal);
		DLLTREATMENT void sClearG(float newVal);
		DLLTREATMENT void sClearB(float newVal);
		DLLTREATMENT void sClearA(float newVal);




		DLLTREATMENT RenderingPresetClass(
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

			float f_ClearR,
			float f_ClearG,
			float f_ClearB,
			float f_ClearA
		);
		DLLTREATMENT RenderingPresetClass(const RenderingPresetClass& toCopy);
		DLLTREATMENT void operator=(const RenderingPresetClass& toCopy);
		DLLTREATMENT void Bind();
		DLLTREATMENT static void Unbind();
	};
}