#pragma once

//need to be defined
#undef CFAC_ClassName

#define CFAC_ClassConstructor(accessVariantName, funcPointers) private: struct CFAC_##accessVariantName##_FuncPtrsContainerStruct { funcPointers };\
static constexpr CFAC_##accessVariantName##_FuncPtrsContainerStruct CFAC_##accessVariantName##_FuncPtrsContainer; public:\
class CFAC_##accessVariantName##_Class { CFAC_ClassName& ClassInstRef; public: static constexpr \
CFAC_##accessVariantName##_FuncPtrsContainerStruct& FuncPtrs = *(CFAC_##accessVariantName##_FuncPtrsContainerStruct* const)&CFAC_##accessVariantName##_FuncPtrsContainer;\
CFAC_##accessVariantName##_Class(CFAC_ClassName& classInstRef) :ClassInstRef(classInstRef) {}; template<typename RetType, typename...ArgsTypes>\
RetType operator()(RetType(CFAC_ClassName::*funcPtr)(ArgsTypes...), ArgsTypes&&...funcArguments) const {\
return (ClassInstRef.*funcPtr)(std::forward<ArgsTypes>(funcArguments)...); } };

//constructs functions for ClassFunctionsAccessController_MainConstructorMacro
#define CFAC_FuncPtrConstr(funcName) static constexpr decltype(&CFAC_ClassName::funcName) funcName=&CFAC_ClassName::funcName;


