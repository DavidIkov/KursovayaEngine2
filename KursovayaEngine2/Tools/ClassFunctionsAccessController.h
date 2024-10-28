#pragma once
#include<utility>

//need to be defined
#undef CFAC_ClassName

#define CFAC_ClassConstructor(accessType_Name, funcs) class CFAC_##accessType_Name##_Class {\
CFAC_ClassName& ClassInstRef; public: CFAC_##accessType_Name##_Class(CFAC_ClassName& inst):\
ClassInstRef(inst){} CFAC_##accessType_Name##_Class(CFAC_##accessType_Name##_Class& cfac):\
ClassInstRef(cfac.ClassInstRef){} funcs };
#define CFAC_FuncConstr(funcName) template<typename...Types> auto funcName##(\
Types&&...vals)->decltype(ClassInstRef.##funcName##(std::forward<Types>(vals)...)){\
return ClassInstRef.##funcName##(std::forward<Types>(vals)...);}

/*
#define CFAC_ClassConstructor(accessVariantName, funcPointers) private: struct CFAC_##accessVariantName##_FuncPtrsContainerStruct { funcPointers };\
static constexpr CFAC_##accessVariantName##_FuncPtrsContainerStruct CFAC_##accessVariantName##_FuncPtrsContainer; public:\
class CFAC_##accessVariantName##_Class { CFAC_ClassName& ClassInstRef; public: static constexpr \
CFAC_##accessVariantName##_FuncPtrsContainerStruct& FuncPtrs = *(CFAC_##accessVariantName##_FuncPtrsContainerStruct* const)&CFAC_##accessVariantName##_FuncPtrsContainer;\
CFAC_##accessVariantName##_Class(CFAC_ClassName& classInstRef) :ClassInstRef(classInstRef) {}; template<typename FuncType, typename...ArgsTypes>\
auto operator()(FuncType funcPtr, ArgsTypes&&...args)->decltype((ClassInstRef.*funcPtr)(std::forward<ArgsTypes>(args)...)) const {\
return (ClassInstRef.*funcPtr)(std::forward<ArgsTypes>(args)...); } };
//constructs functions for ClassFunctionsAccessController_MainConstructorMacro
#define CFAC_FuncPtrConstr(funcName) static constexpr decltype(&CFAC_ClassName::funcName) funcName=&CFAC_ClassName::funcName;
#define CFAC_SpecializedFuncPtrConstr(funcName, funcNameAddition, retType, params) static constexpr \
retType(CFAC_ClassName::*funcName##funcNameAddition)##params=&CFAC_ClassName::funcName;
*/


