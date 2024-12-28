#include"Shader.h"
#include"glad/glad.h"
#include<string>
#include"Tools/GLDebug.h"
#include<fstream>
#include"KE2_Manager.h"

using namespace KE2;
using namespace Graphics::Primitives;

#if defined KE2_Debug
#define Assert_NotCompiled_Macro if (Compiled) ErrorsSystemNamespace::SendError<<"Shader is already compiled">>ErrorsEnumWrapperStruct(ErrorsEnum::AlreadyCompiled);
#else
#define Assert_NotCompiled_Macro
#endif

ShaderClass::ShaderClass(const char* filePath, TypesEnum typ) 
#ifdef KE2_Debug
	:ShaderType(typ)
#endif
{
	glSC(ID = glCreateShader((typ == TypesEnum::Fragment) ? GL_FRAGMENT_SHADER : ((typ == TypesEnum::Vertex) ? GL_VERTEX_SHADER : GL_GEOMETRY_SHADER)));
	std::string srcCode; 
	{
		if(KE2::Manager::OutputStream != nullptr) *KE2::Manager::OutputStream << "Reading shader \"" << filePath << '\"' << std::endl;
		std::fstream srcFile(filePath, std::ios_base::in); std::string	curLine;
		size_t charactersAmount = 0; while (std::getline(srcFile, curLine)) charactersAmount += curLine.length() + 1;
		srcFile.clear(); srcFile.seekg(0); srcCode.reserve(charactersAmount);
		while (std::getline(srcFile, curLine)) srcCode += curLine + '\n';
	}
	const char* code = srcCode.c_str();
	glSC(glShaderSource(ID, 1, &code, 0));
}
ShaderClass::ShaderClass(TypesEnum typ, const char* code) 
#ifdef KE2_Debug
	:ShaderType(typ)
#endif
{
	glSC(ID = glCreateShader((typ == TypesEnum::Fragment) ? GL_FRAGMENT_SHADER : ((typ == TypesEnum::Vertex) ? GL_VERTEX_SHADER : GL_GEOMETRY_SHADER)));
	glSC(glShaderSource(ID, 1, &code, 0));
}
ShaderClass::ShaderClass(ShaderClass&& toCopy) noexcept :
	ID(toCopy.ID)
#ifdef KE2_Debug
	,Compiled(toCopy.Compiled), ShaderType(toCopy.ShaderType)
#endif
{
	toCopy.ID = 0u;
}
ShaderClass& ShaderClass::operator=(ShaderClass&& toCopy) {
	this->~ShaderClass();
	new(this) ShaderClass(std::move(toCopy));
	return *this;
}
ShaderClass::~ShaderClass() noexcept(false) {
	if (ID != 0u) {
		glSC(glDeleteShader(ID));
		ID = 0u;
	}
}
void ShaderClass::Compile() const {

	Assert_NotCompiled_Macro;

	glSC(glCompileShader(ID));
#if defined KE2_Debug
	Compiled = true;
	{//check for compilation
		int success = 0; glSC(glGetShaderiv(ID, GL_COMPILE_STATUS, &success));
		if (success == false) {
			int infoLen = 0; glSC(glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &infoLen));
			std::string info(infoLen - 1, '\0');
			glSC(glGetShaderInfoLog(ID, infoLen, 0, (char*)info.c_str()));
			ErrorsSystemNamespace::SendError << "Failed to compile " <<
				((ShaderType == TypesEnum::Fragment) ? "fragment" : ((ShaderType == TypesEnum::Vertex) ? "vertex" : "geometry")) <<
				" shader, OpenGL returned a message: [" << info << "]" >> ErrorsEnumWrapperStruct(ErrorsEnum::FailedToCompile);
		}
	}
#endif
}