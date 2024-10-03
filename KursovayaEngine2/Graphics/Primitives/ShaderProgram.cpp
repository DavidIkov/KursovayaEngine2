#include"ShaderProgram.h"
#include"glad/glad.h"
#include"Tools/DebuggingTools.h"
#include<vector>
#include<string>
#include"Tools/GLDebug.h"
#include"Tools/DebugRuntimeAssert.h"
#include"Graphics/Globals.h"

using namespace Graphics::Primitives;
#define Assert_NotDeleted_Macro DebugRuntimeAssert(DebuggingTools::ErrorTypes::Critical, not Deleted, "ShaderProgram is deleted", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION);
#if defined KE2_Debug
#define Assert_Linked_Macro DebugRuntimeAssert(DebuggingTools::ErrorTypes::Critical, Linked, "ShaderProgram is already not linked", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION);
#define Assert_NotLinked_Macro DebugRuntimeAssert(DebuggingTools::ErrorTypes::Critical, not Linked, "ShaderProgram is already linked", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION);
#else
#define Assert_Linked_Macro
#define Assert_NotLinked_Macro
#endif

unsigned int ShaderProgramClass::gID() {
	Assert_NotDeleted_Macro;
	return ID;
}
ShaderProgramClass::ShaderProgramClass() {
	glSC(ID = glCreateProgram());
}
ShaderProgramClass::ShaderProgramClass(RespConstrFlag, const ShaderProgramClass& toCopy) {
	memcpy(this, &toCopy, sizeof(ShaderProgramClass));
	toCopy.Deleted = true;
}
ShaderProgramClass::ShaderProgramClass(const ShaderProgramClass&& toCopy) {
	memcpy(this, &toCopy, sizeof(ShaderProgramClass));
	toCopy.Deleted = true;
}
void ShaderProgramClass::operator=(const ShaderProgramClass&& toCopy) {
	memcpy(this, &toCopy, sizeof(ShaderProgramClass));
	toCopy.Deleted = true;
}
void ShaderProgramClass::AttachShader(ShaderClass::CFAC_FullAccess_Class shaderCFAC) {
	Assert_NotDeleted_Macro;
	Assert_NotLinked_Macro;
	
	glSC(glAttachShader(ID, shaderCFAC(shaderCFAC.FuncPtrs.gID)));
}
void ShaderProgramClass::LinkShaders() {
	Assert_NotDeleted_Macro;
	Assert_NotLinked_Macro;
	
	glSC(glLinkProgram(ID));
#if defined KE2_Debug
	Linked = true;
	{//check for linking
		int success;
		char info[512];
		glSC(glGetProgramiv(ID, GL_LINK_STATUS, &success));
		if (!success) {
			glSC(glGetProgramInfoLog(ID, 512, 0, info));
			std::string msg = "SHADERS LINKING ERROR: ";
			msg += info;
			DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical, msg.c_str(), KURSAVAYAENGINE2_CORE_ERRORS::FAILED_THIRD_PARTY_FUNCTION });
		}
	}
#endif
}
ShaderProgramClass::~ShaderProgramClass() {
	if (not Deleted) {// no need for warning becouse destructor will be called at end of scope anyway
		glSC(glDeleteProgram(ID));
		Deleted = true;
	}
}
void ShaderProgramClass::Delete() {
	Assert_NotDeleted_Macro;
	this->~ShaderProgramClass();
}

void ShaderProgramClass::Bind() {
	Assert_NotDeleted_Macro;
	Assert_Linked_Macro;
#if defined KE2_Debug
	BindedInstances.sShaderProgram_ID(ID);
#endif
	glSC(glUseProgram(ID));
}
void ShaderProgramClass::Unbind() {
	glSC(glUseProgram(0));
}





#if defined KE2_Debug
#define uniformCOPYPASTE(funcName, ...)\
Assert_NotDeleted_Macro; Assert_Linked_Macro; {\
	glSC(glUseProgram(ID));\
	int location = glGetUniformLocation(ID, name);\
	if (location == -1) {\
		std::string msg; msg+="UNIFORM CALLED \""; msg+=name; msg+="\" NOT FOUND IN SHADER PROGRAM";\
		DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning,msg.c_str(),KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_FUNCTION_WITH_INVALID_ARGUMENTS });\
	}\
	else glSC(funcName(glGetUniformLocation(ID, name), __VA_ARGS__));\
}
#else
#define uniformCOPYPASTE(funcName, ...) glSC(funcName(glGetUniformLocation(ID, name), __VA_ARGS__));
#endif

void ShaderProgramClass::SetUniform1f(const char* name, float v0) { uniformCOPYPASTE(glUniform1f, v0); }
void ShaderProgramClass::SetUniform2f(const char* name, float v0, float v1) { uniformCOPYPASTE(glUniform2f, v0, v1); }
void ShaderProgramClass::SetUniform3f(const char* name, float v0, float v1, float v2) { uniformCOPYPASTE(glUniform3f, v0, v1, v2); }
void ShaderProgramClass::SetUniform4f(const char* name, float v0, float v1, float v2, float v3) { uniformCOPYPASTE(glUniform4f, v0, v1, v2, v3); }
void ShaderProgramClass::SetUniform1i(const char* name, int v0) { uniformCOPYPASTE(glUniform1i, v0); }
void ShaderProgramClass::SetUniform2i(const char* name, int v0, int v1) { uniformCOPYPASTE(glUniform2i, v0, v1); }
void ShaderProgramClass::SetUniform3i(const char* name, int v0, int v1, int v2) { uniformCOPYPASTE(glUniform3i, v0, v1, v2); }
void ShaderProgramClass::SetUniform4i(const char* name, int v0, int v1, int v2, int v3) { uniformCOPYPASTE(glUniform4i, v0, v1, v2, v3); }
void ShaderProgramClass::SetUniform1ui(const char* name, unsigned int v0) { uniformCOPYPASTE(glUniform1ui, v0); }
void ShaderProgramClass::SetUniform2ui(const char* name, unsigned int v0, unsigned int v1) { uniformCOPYPASTE(glUniform2ui, v0, v1); }
void ShaderProgramClass::SetUniform3ui(const char* name, unsigned int v0, unsigned int v1, unsigned int v2) { uniformCOPYPASTE(glUniform3ui, v0, v1, v2); }
void ShaderProgramClass::SetUniform4ui(const char* name, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3) { uniformCOPYPASTE(glUniform4ui, v0, v1, v2, v3); }
void ShaderProgramClass::SetUniform1fv(const char* name, unsigned int count, const float* value) { uniformCOPYPASTE(glUniform1fv, count, value); }
void ShaderProgramClass::SetUniform2fv(const char* name, unsigned int count, const float* value) { uniformCOPYPASTE(glUniform2fv, count, value); }
void ShaderProgramClass::SetUniform3fv(const char* name, unsigned int count, const float* value) { uniformCOPYPASTE(glUniform3fv, count, value); }
void ShaderProgramClass::SetUniform4fv(const char* name, unsigned int count, const float* value) { uniformCOPYPASTE(glUniform4fv, count, value); }
void ShaderProgramClass::SetUniform1iv(const char* name, unsigned int count, const int* value) { uniformCOPYPASTE(glUniform1iv, count, value); }
void ShaderProgramClass::SetUniform2iv(const char* name, unsigned int count, const int* value) { uniformCOPYPASTE(glUniform2iv, count, value); }
void ShaderProgramClass::SetUniform3iv(const char* name, unsigned int count, const int* value) { uniformCOPYPASTE(glUniform3iv, count, value); }
void ShaderProgramClass::SetUniform4iv(const char* name, unsigned int count, const int* value) { uniformCOPYPASTE(glUniform4iv, count, value); }
void ShaderProgramClass::SetUniform1uiv(const char* name, unsigned int count, const unsigned int* value) { uniformCOPYPASTE(glUniform1uiv, count, value); }
void ShaderProgramClass::SetUniform2uiv(const char* name, unsigned int count, const unsigned int* value) { uniformCOPYPASTE(glUniform2uiv, count, value); }
void ShaderProgramClass::SetUniform3uiv(const char* name, unsigned int count, const unsigned int* value) { uniformCOPYPASTE(glUniform3uiv, count, value); }
void ShaderProgramClass::SetUniform4uiv(const char* name, unsigned int count, const unsigned int* value) { uniformCOPYPASTE(glUniform4uiv, count, value); }
void ShaderProgramClass::SetUniformMatrix2fv(const char* name, unsigned int count, bool transpose, const float* value) { uniformCOPYPASTE(glUniformMatrix2fv, count, transpose, value); }
void ShaderProgramClass::SetUniformMatrix3fv(const char* name, unsigned int count, bool transpose, const float* value) { uniformCOPYPASTE(glUniformMatrix3fv, count, transpose, value); }
void ShaderProgramClass::SetUniformMatrix4fv(const char* name, unsigned int count, bool transpose, const float* value) { uniformCOPYPASTE(glUniformMatrix4fv, count, transpose, value); }
void ShaderProgramClass::SetUniformMatrix2x3fv(const char* name, unsigned int count, bool transpose, const float* value) { uniformCOPYPASTE(glUniformMatrix2x3fv, count, transpose, value); }
void ShaderProgramClass::SetUniformMatrix3x2fv(const char* name, unsigned int count, bool transpose, const float* value) { uniformCOPYPASTE(glUniformMatrix3x2fv, count, transpose, value); }
void ShaderProgramClass::SetUniformMatrix2x4fv(const char* name, unsigned int count, bool transpose, const float* value) { uniformCOPYPASTE(glUniformMatrix2x4fv, count, transpose, value); }
void ShaderProgramClass::SetUniformMatrix4x2fv(const char* name, unsigned int count, bool transpose, const float* value) { uniformCOPYPASTE(glUniformMatrix4x2fv, count, transpose, value); }
void ShaderProgramClass::SetUniformMatrix3x4fv(const char* name, unsigned int count, bool transpose, const float* value) { uniformCOPYPASTE(glUniformMatrix3x4fv, count, transpose, value); }
void ShaderProgramClass::SetUniformMatrix4x3fv(const char* name, unsigned int count, bool transpose, const float* value) { uniformCOPYPASTE(glUniformMatrix4x3fv, count, transpose, value); }

