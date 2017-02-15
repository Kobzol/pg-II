////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	shader.h
//
// summary:	Declaration and implementation of Shader class
// author:	Petr Gajdoš
// 
// Copyright © 2014 Petr Gajdoš. All Rights Reserved.
//////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __SHADER_H_
#define __SHADER_H_

#include <glew.h>
#include <fstream>

using namespace std;

enum ShaderState
{
	SS_UNKNOWN = 0, 
	SS_LOADED,
	SS_CREATED,
	SS_COMPILED
};

class __declspec( dllexport ) Shader
{
friend class ShaderProgram;

private:
	static const unsigned int SHADER_MAX_LINES = 512;
	static const unsigned int SHADER_MAX_LINE_LENGTH = 256;			//= 255 + NULL terminate


protected:
	ShaderState m_shaderState;
	GLenum m_shaderType;
	GLuint m_shaderObject;

public:
	Shader(GLenum  shaderType) : m_shaderType(shaderType), m_shaderState(SS_UNKNOWN), m_shaderObject(0) {}
	virtual ~Shader() { if (m_shaderObject >0) glDeleteShader(m_shaderObject); }
	   
	inline GLhandleARB getShader()  { return m_shaderObject; }
		 
	void create();
	void openFromFile(const char *filename);
	void openFromArray(const char *data);
	char *getInfoLog();

	friend class ShaderProgram;
};


inline void Shader::create()
{
	if (m_shaderState == SS_CREATED) return;
	m_shaderObject = glCreateShader(m_shaderType);   //glCreateShaderObjectARB(shaderType);
	m_shaderState = SS_CREATED;
}

inline char *Shader::getInfoLog()
{
	char *infoLog;
	int infoLogLength;
	glGetShaderiv(m_shaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
	infoLog = new char[infoLogLength + 1];
	glGetShaderInfoLog(m_shaderObject, infoLogLength, &infoLogLength, infoLog);
	return infoLog;
}

inline void Shader::openFromFile(const char *filename)
{
	ifstream input(filename);

	if (!input.is_open())
		throw "Shader::exception(47):  unable to load shader file!";

	if (m_shaderObject == 0)
		create();

	/*  No vertex or fragment program should ever be longer than 512 lines by 255 characters. */
	char *program = (char*)malloc(SHADER_MAX_LINES*SHADER_MAX_LINE_LENGTH);

	const char **programPtr = (const char **)&program;
	size_t len;
	char *ptr = program;

	while (!input.eof())
	{
		input.getline(ptr, SHADER_MAX_LINE_LENGTH);
		len = strlen(ptr);
		ptr[len] = '\n';
		ptr += len + 1;
	}
	ptr[0] = 0;

	input.close();

	glShaderSource(m_shaderObject, 1, programPtr, NULL);
	m_shaderState = SS_LOADED;

	glCompileShader(m_shaderObject);
	int status = 0;
	glGetShaderiv(m_shaderObject, GL_COMPILE_STATUS, (int *)&status);
	if (!status)
	{
		char errorMsg[1024] = { '\0' };
		glGetShaderInfoLog(m_shaderObject, 1024, 0, errorMsg);
		printf("ERROR (%s): %s\n", filename, errorMsg);
		throw "Shader::exception(80):  shader did not compile!";
	}

	m_shaderState = SS_COMPILED;

	free(program);
}

inline void Shader::openFromArray(const char *data)
{
	if (data)
		create();

	const char**programPtr = (const char **)&data;
	glShaderSource(m_shaderObject, 1, programPtr, NULL);
	m_shaderState = SS_LOADED;

	glCompileShaderARB(m_shaderObject);

	int status = 0;
	glGetShaderiv(m_shaderObject, GL_COMPILE_STATUS, (int *)&status);
	if (!status)
		throw "Shader::exception(80):  shader did not compile!";

	m_shaderState = SS_COMPILED;
}

#endif

