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
#define Assert_Binded_Macro if (DebugRuntimeAssert(DebuggingTools::ErrorTypes::Warning, BindedInstances.gShaderProgramID() == ID, "ShaderProgram is not binded", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION)) Bind();
#define Assert_Linked_Macro DebugRuntimeAssert(DebuggingTools::ErrorTypes::Critical, Linked, "ShaderProgram is already not linked", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION);
#define Assert_NotLinked_Macro DebugRuntimeAssert(DebuggingTools::ErrorTypes::Critical, not Linked, "ShaderProgram is already linked", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION);
#else
#define Assert_Binded_Macro
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
ShaderProgramClass::ShaderProgramClass(const ShaderProgramClass&& toCopy) {
	memcpy(this, &toCopy, sizeof(ShaderProgramClass));
	toCopy.Deleted = true;
}
void ShaderProgramClass::operator=(const ShaderProgramClass&& toCopy) {
	memcpy(this, &toCopy, sizeof(ShaderProgramClass));
	toCopy.Deleted = true;
}
void ShaderProgramClass::AttachShader(unsigned int id) {
	Assert_NotDeleted_Macro;
	Assert_NotLinked_Macro;
	
	glSC(glAttachShader(ID, id));
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
	if (not Deleted) {
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
#if defined KE2_Debug
	BindedInstances.sShaderProgram_ID(0);
#endif
}

unsigned int ShaderProgramClass::GetUniformIDByName(const char* name) {
	Assert_NotDeleted_Macro;
	Assert_Linked_Macro;
	Assert_Binded_Macro;
	glSC(int uniformID = glGetUniformLocation(ID, name));
	return uniformID;
}
ShaderProgramClass::UniformData ShaderProgramClass::GetUniformData(unsigned int index) {
	Assert_NotDeleted_Macro;
	Assert_Linked_Macro;
	Assert_Binded_Macro;
	UniformData returnData;
	char name[100] = {};
	int actualNameLength = 0;
	int size = 0;
	glSC(glGetActiveUniform(ID, index, sizeof(name) / sizeof(char), &actualNameLength, &size, &returnData.Type, name));
	if (actualNameLength == (sizeof(name) / sizeof(char) - 1))
		DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical,"name of uniform MAY(most likely) is too long for current buffer length",KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_FUNCTION_WITH_INVALID_ARGUMENTS });
	returnData.Size = size;
	returnData.Name = std::string(name);
	returnData.ID = GetUniformIDByName(name);
	return returnData;
}
void ShaderProgramClass::GetUniformsData(DynArr<UniformData>* dynArr) {
	Assert_NotDeleted_Macro;
	Assert_Linked_Macro;
	Assert_Binded_Macro;

	int amountOfUniforms = 0;
	glSC(glGetProgramiv(ID, GL_ACTIVE_UNIFORMS, &amountOfUniforms));

	dynArr->ChangeCapacity(amountOfUniforms);

	for (int i = 0; i < amountOfUniforms; i++)
		dynArr->InsertByConstructor(i, GetUniformData(i));
}


#if defined KE2_Debug
#define uniformCOPYPASTE(funcName, ...)\
Assert_NotDeleted_Macro; Assert_Linked_Macro; Assert_Binded_Macro;\
glSC(funcName(uniformID, __VA_ARGS__));
#else
#define uniformCOPYPASTE(funcName, ...) glSC(funcName(uniformID, __VA_ARGS__));
#endif

void ShaderProgramClass::SetUniform1f(unsigned int uniformID, float v0) { uniformCOPYPASTE(glUniform1f, v0); }
void ShaderProgramClass::SetUniform2f(unsigned int uniformID, float v0, float v1) { uniformCOPYPASTE(glUniform2f, v0, v1); }
void ShaderProgramClass::SetUniform3f(unsigned int uniformID, float v0, float v1, float v2) { uniformCOPYPASTE(glUniform3f, v0, v1, v2); }
void ShaderProgramClass::SetUniform4f(unsigned int uniformID, float v0, float v1, float v2, float v3) { uniformCOPYPASTE(glUniform4f, v0, v1, v2, v3); }
void ShaderProgramClass::SetUniform1i(unsigned int uniformID, int v0) { uniformCOPYPASTE(glUniform1i, v0); }
void ShaderProgramClass::SetUniform2i(unsigned int uniformID, int v0, int v1) { uniformCOPYPASTE(glUniform2i, v0, v1); }
void ShaderProgramClass::SetUniform3i(unsigned int uniformID, int v0, int v1, int v2) { uniformCOPYPASTE(glUniform3i, v0, v1, v2); }
void ShaderProgramClass::SetUniform4i(unsigned int uniformID, int v0, int v1, int v2, int v3) { uniformCOPYPASTE(glUniform4i, v0, v1, v2, v3); }
void ShaderProgramClass::SetUniform1ui(unsigned int uniformID, unsigned int v0) { uniformCOPYPASTE(glUniform1ui, v0); }
void ShaderProgramClass::SetUniform2ui(unsigned int uniformID, unsigned int v0, unsigned int v1) { uniformCOPYPASTE(glUniform2ui, v0, v1); }
void ShaderProgramClass::SetUniform3ui(unsigned int uniformID, unsigned int v0, unsigned int v1, unsigned int v2) { uniformCOPYPASTE(glUniform3ui, v0, v1, v2); }
void ShaderProgramClass::SetUniform4ui(unsigned int uniformID, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3) { uniformCOPYPASTE(glUniform4ui, v0, v1, v2, v3); }
void ShaderProgramClass::SetUniform1fv(unsigned int uniformID, unsigned int count, const float* value) { uniformCOPYPASTE(glUniform1fv, count, value); }
void ShaderProgramClass::SetUniform2fv(unsigned int uniformID, unsigned int count, const float* value) { uniformCOPYPASTE(glUniform2fv, count, value); }
void ShaderProgramClass::SetUniform3fv(unsigned int uniformID, unsigned int count, const float* value) { uniformCOPYPASTE(glUniform3fv, count, value); }
void ShaderProgramClass::SetUniform4fv(unsigned int uniformID, unsigned int count, const float* value) { uniformCOPYPASTE(glUniform4fv, count, value); }
void ShaderProgramClass::SetUniform1iv(unsigned int uniformID, unsigned int count, const int* value) { uniformCOPYPASTE(glUniform1iv, count, value); }
void ShaderProgramClass::SetUniform2iv(unsigned int uniformID, unsigned int count, const int* value) { uniformCOPYPASTE(glUniform2iv, count, value); }
void ShaderProgramClass::SetUniform3iv(unsigned int uniformID, unsigned int count, const int* value) { uniformCOPYPASTE(glUniform3iv, count, value); }
void ShaderProgramClass::SetUniform4iv(unsigned int uniformID, unsigned int count, const int* value) { uniformCOPYPASTE(glUniform4iv, count, value); }
void ShaderProgramClass::SetUniform1uiv(unsigned int uniformID, unsigned int count, const unsigned int* value) { uniformCOPYPASTE(glUniform1uiv, count, value); }
void ShaderProgramClass::SetUniform2uiv(unsigned int uniformID, unsigned int count, const unsigned int* value) { uniformCOPYPASTE(glUniform2uiv, count, value); }
void ShaderProgramClass::SetUniform3uiv(unsigned int uniformID, unsigned int count, const unsigned int* value) { uniformCOPYPASTE(glUniform3uiv, count, value); }
void ShaderProgramClass::SetUniform4uiv(unsigned int uniformID, unsigned int count, const unsigned int* value) { uniformCOPYPASTE(glUniform4uiv, count, value); }
void ShaderProgramClass::SetUniformMatrix2fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value) { uniformCOPYPASTE(glUniformMatrix2fv, count, transpose, value); }
void ShaderProgramClass::SetUniformMatrix3fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value) { uniformCOPYPASTE(glUniformMatrix3fv, count, transpose, value); }
void ShaderProgramClass::SetUniformMatrix4fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value) { uniformCOPYPASTE(glUniformMatrix4fv, count, transpose, value); }
void ShaderProgramClass::SetUniformMatrix2x3fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value) { uniformCOPYPASTE(glUniformMatrix2x3fv, count, transpose, value); }
void ShaderProgramClass::SetUniformMatrix3x2fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value) { uniformCOPYPASTE(glUniformMatrix3x2fv, count, transpose, value); }
void ShaderProgramClass::SetUniformMatrix2x4fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value) { uniformCOPYPASTE(glUniformMatrix2x4fv, count, transpose, value); }
void ShaderProgramClass::SetUniformMatrix4x2fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value) { uniformCOPYPASTE(glUniformMatrix4x2fv, count, transpose, value); }
void ShaderProgramClass::SetUniformMatrix3x4fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value) { uniformCOPYPASTE(glUniformMatrix3x4fv, count, transpose, value); }
void ShaderProgramClass::SetUniformMatrix4x3fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value) { uniformCOPYPASTE(glUniformMatrix4x3fv, count, transpose, value); }

