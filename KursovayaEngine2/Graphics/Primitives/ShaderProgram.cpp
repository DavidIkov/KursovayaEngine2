#include"ShaderProgram.h"
#include"glad/glad.h"
#include<vector>
#include<string>
#include"Tools/GLDebug.h"
#include"Graphics/Globals.h"

using namespace KE2;
using namespace Graphics::Primitives;
#if defined KE2_Debug
#define Assert_Binded_Macro if (BindedInstances.gShaderProgramID() != ID) { ErrorsSystemNamespace::SendWarning<<"ShaderProgram is not binded">>ErrorsSystemNamespace::EndOfWarning; Bind(); }
#define Assert_Linked_Macro if (not Linked) ErrorsSystemNamespace::SendError<<"ShaderProgram is not linked">>ErrorsEnumWrapperStruct(ErrorsEnum::NotLinked);
#define Assert_NotLinked_Macro if (Linked) ErrorsSystemNamespace::SendError<<"ShaderProgram is already linked">>ErrorsEnumWrapperStruct(ErrorsEnum::AlreadyLinked);
#else
#define Assert_Binded_Macro
#define Assert_Linked_Macro
#define Assert_NotLinked_Macro
#endif

ShaderProgramClass::ShaderProgramClass() {
	glSC(ID = glCreateProgram());
}
ShaderProgramClass::ShaderProgramClass(const char* vsPath, const char* fsPath) :ShaderProgramClass() {
	ShaderClass VS(vsPath, ShaderClass::TypesEnum::Vertex);
	VS.Compile();
	AttachShader(VS.gID());

	ShaderClass FS(fsPath, ShaderClass::TypesEnum::Fragment);
	FS.Compile();
	AttachShader(FS.gID());

	LinkShaders();
}
ShaderProgramClass::ShaderProgramClass(const char* vsPath, const char* gsPath, const char* fsPath) :ShaderProgramClass() {
	ShaderClass VS(vsPath, ShaderClass::TypesEnum::Vertex);
	VS.Compile();
	AttachShader(VS.gID());

	ShaderClass GS(gsPath, ShaderClass::TypesEnum::Geometry);
	GS.Compile();
	AttachShader(GS.gID());

	ShaderClass FS(fsPath, ShaderClass::TypesEnum::Fragment);
	FS.Compile();
	AttachShader(FS.gID());

	LinkShaders();
}

ShaderProgramClass::ShaderProgramClass(ShaderProgramClass&& toCopy) noexcept :
	ID(toCopy.ID)
#ifdef KE2_Debug
	,Linked(toCopy.Linked)
#endif
{
	toCopy.ID = 0u;
}
ShaderProgramClass& ShaderProgramClass::operator=(ShaderProgramClass&& toCopy) {
	this->~ShaderProgramClass();
	new(this) ShaderProgramClass(std::move(toCopy));
	return *this;
}
void ShaderProgramClass::AttachShader(unsigned int id) const {
	Assert_NotLinked_Macro;
	
	glSC(glAttachShader(ID, id));
}
void ShaderProgramClass::LinkShaders() const {
	Assert_NotLinked_Macro;
	
	glSC(glLinkProgram(ID));
#if defined KE2_Debug
	Linked = true;
	{//check for linking
		int success = 0; glSC(glGetProgramiv(ID, GL_LINK_STATUS, &success));
		if (success == false) {
			int infoLen = 0; glSC(glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &infoLen));
			std::string info(infoLen - 1, '\0');
			glSC(glGetProgramInfoLog(ID, infoLen, 0, (char*)info.c_str()));
			ErrorsSystemNamespace::SendError << "Failed to link shaders, OpenGL returned a message: [" << info << "]" >> ErrorsEnumWrapperStruct(ErrorsEnum::FailedToLinkShaders);
		}
	}
#endif
}
ShaderProgramClass::~ShaderProgramClass() noexcept(false) {
	if (ID != 0u) {
		Unbind();
		glSC(glDeleteProgram(ID));
		ID = 0u;
	}
}

void ShaderProgramClass::Bind() const {
	Assert_Linked_Macro;
#if defined KE2_Debug
	BindedInstances.sShaderProgramID(ID);
#endif
	glSC(glUseProgram(ID));
}
void ShaderProgramClass::Unbind() {
	glSC(glUseProgram(0));
#if defined KE2_Debug
	BindedInstances.sShaderProgramID(0);
#endif
}

unsigned int ShaderProgramClass::GetUniformIDByName(const char* name) const {
	Assert_Linked_Macro;
	Assert_Binded_Macro;
	glSC(int uniformID = glGetUniformLocation(ID, name));
	return uniformID;
}
ShaderProgramClass::UniformData ShaderProgramClass::GetUniformData(unsigned int index) const {
	Assert_Linked_Macro;
	Assert_Binded_Macro;
	UniformData returnData;
	char name[100] = {};
	int actualNameLength = 0;
	int size = 0;
	glSC(glGetActiveUniform(ID, index, sizeof(name) / sizeof(char), &actualNameLength, &size, &returnData.Type, name));
	if (actualNameLength == (sizeof(name) / sizeof(char) - 1))
		ErrorsSystemNamespace::SendError << "name of uniform is too long, current limit is: [" << std::to_string(sizeof(name) / sizeof(char)) << "]" >> ErrorsEnumWrapperStruct(ErrorsEnum::UniformNameIsTooLarge);
	returnData.Size = size;
	returnData.Name = std::string(name);
	returnData.ID = GetUniformIDByName(name);
	return returnData;
}
void ShaderProgramClass::GetUniformsData(DynArr<UniformData>& dynArr) const {
	Assert_Linked_Macro;
	Assert_Binded_Macro;

	int amountOfUniforms = 0;
	glSC(glGetProgramiv(ID, GL_ACTIVE_UNIFORMS, &amountOfUniforms));

	dynArr.ChangeCapacity(amountOfUniforms);

	for (int i = 0; i < amountOfUniforms; i++)
		dynArr.ReplaceAtIndex(i, GetUniformData(i));
}


#if defined KE2_Debug
#define uniformCOPYPASTE(funcName, ...)\
Assert_Linked_Macro; Assert_Binded_Macro;\
glSC(funcName(uniformID, __VA_ARGS__));
#else
#define uniformCOPYPASTE(funcName, ...) glSC(funcName(uniformID, __VA_ARGS__));
#endif

void ShaderProgramClass::SetUniform1f(unsigned int uniformID, float v0) const { uniformCOPYPASTE(glUniform1f, v0); }
void ShaderProgramClass::SetUniform2f(unsigned int uniformID, float v0, float v1) const { uniformCOPYPASTE(glUniform2f, v0, v1); }
void ShaderProgramClass::SetUniform3f(unsigned int uniformID, float v0, float v1, float v2) const { uniformCOPYPASTE(glUniform3f, v0, v1, v2); }
void ShaderProgramClass::SetUniform4f(unsigned int uniformID, float v0, float v1, float v2, float v3) const { uniformCOPYPASTE(glUniform4f, v0, v1, v2, v3); }
void ShaderProgramClass::SetUniform1i(unsigned int uniformID, int v0) const { uniformCOPYPASTE(glUniform1i, v0); }
void ShaderProgramClass::SetUniform2i(unsigned int uniformID, int v0, int v1) const { uniformCOPYPASTE(glUniform2i, v0, v1); }
void ShaderProgramClass::SetUniform3i(unsigned int uniformID, int v0, int v1, int v2) const { uniformCOPYPASTE(glUniform3i, v0, v1, v2); }
void ShaderProgramClass::SetUniform4i(unsigned int uniformID, int v0, int v1, int v2, int v3) const { uniformCOPYPASTE(glUniform4i, v0, v1, v2, v3); }
void ShaderProgramClass::SetUniform1ui(unsigned int uniformID, unsigned int v0) const { uniformCOPYPASTE(glUniform1ui, v0); }
void ShaderProgramClass::SetUniform2ui(unsigned int uniformID, unsigned int v0, unsigned int v1) const { uniformCOPYPASTE(glUniform2ui, v0, v1); }
void ShaderProgramClass::SetUniform3ui(unsigned int uniformID, unsigned int v0, unsigned int v1, unsigned int v2) const { uniformCOPYPASTE(glUniform3ui, v0, v1, v2); }
void ShaderProgramClass::SetUniform4ui(unsigned int uniformID, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3) const { uniformCOPYPASTE(glUniform4ui, v0, v1, v2, v3); }
void ShaderProgramClass::SetUniform1fv(unsigned int uniformID, unsigned int count, const float* value) const { uniformCOPYPASTE(glUniform1fv, count, value); }
void ShaderProgramClass::SetUniform2fv(unsigned int uniformID, unsigned int count, const float* value) const { uniformCOPYPASTE(glUniform2fv, count, value); }
void ShaderProgramClass::SetUniform3fv(unsigned int uniformID, unsigned int count, const float* value) const { uniformCOPYPASTE(glUniform3fv, count, value); }
void ShaderProgramClass::SetUniform4fv(unsigned int uniformID, unsigned int count, const float* value) const { uniformCOPYPASTE(glUniform4fv, count, value); }
void ShaderProgramClass::SetUniform1iv(unsigned int uniformID, unsigned int count, const int* value) const { uniformCOPYPASTE(glUniform1iv, count, value); }
void ShaderProgramClass::SetUniform2iv(unsigned int uniformID, unsigned int count, const int* value) const { uniformCOPYPASTE(glUniform2iv, count, value); }
void ShaderProgramClass::SetUniform3iv(unsigned int uniformID, unsigned int count, const int* value) const { uniformCOPYPASTE(glUniform3iv, count, value); }
void ShaderProgramClass::SetUniform4iv(unsigned int uniformID, unsigned int count, const int* value) const { uniformCOPYPASTE(glUniform4iv, count, value); }
void ShaderProgramClass::SetUniform1uiv(unsigned int uniformID, unsigned int count, const unsigned int* value) const { uniformCOPYPASTE(glUniform1uiv, count, value); }
void ShaderProgramClass::SetUniform2uiv(unsigned int uniformID, unsigned int count, const unsigned int* value) const { uniformCOPYPASTE(glUniform2uiv, count, value); }
void ShaderProgramClass::SetUniform3uiv(unsigned int uniformID, unsigned int count, const unsigned int* value) const { uniformCOPYPASTE(glUniform3uiv, count, value); }
void ShaderProgramClass::SetUniform4uiv(unsigned int uniformID, unsigned int count, const unsigned int* value) const { uniformCOPYPASTE(glUniform4uiv, count, value); }
void ShaderProgramClass::SetUniformMatrix2fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value) const { uniformCOPYPASTE(glUniformMatrix2fv, count, transpose, value); }
void ShaderProgramClass::SetUniformMatrix3fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value) const { uniformCOPYPASTE(glUniformMatrix3fv, count, transpose, value); }
void ShaderProgramClass::SetUniformMatrix4fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value) const { uniformCOPYPASTE(glUniformMatrix4fv, count, transpose, value); }
void ShaderProgramClass::SetUniformMatrix2x3fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value) const { uniformCOPYPASTE(glUniformMatrix2x3fv, count, transpose, value); }
void ShaderProgramClass::SetUniformMatrix3x2fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value) const { uniformCOPYPASTE(glUniformMatrix3x2fv, count, transpose, value); }
void ShaderProgramClass::SetUniformMatrix2x4fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value) const { uniformCOPYPASTE(glUniformMatrix2x4fv, count, transpose, value); }
void ShaderProgramClass::SetUniformMatrix4x2fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value) const { uniformCOPYPASTE(glUniformMatrix4x2fv, count, transpose, value); }
void ShaderProgramClass::SetUniformMatrix3x4fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value) const { uniformCOPYPASTE(glUniformMatrix3x4fv, count, transpose, value); }
void ShaderProgramClass::SetUniformMatrix4x3fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value) const { uniformCOPYPASTE(glUniformMatrix4x3fv, count, transpose, value); }

