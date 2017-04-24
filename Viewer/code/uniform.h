#pragma once

#include <iostream>

#include <glew.h>
#include <glm.hpp>
#include <glm-0.9.6.3/gtc/type_ptr.hpp>

#include <sceneSetting.h>
#include <vector>

template <typename T>
class Uniform
{
public:
	static void bind(const std::string& name, SceneSetting* setting, const T& value)
	{
		Uniform<T>::bind(name, setting->m_activeShader->m_programObject, value);
	}
	static void bind(const std::string& name, GLuint program, const T& value)
	{
		GLuint uniform;
		if ((uniform = glGetUniformLocation(program, name.c_str())) >= 0)
		{
			Uniform<T>::bindValue(uniform, value);
		}
		else std::cerr << "No uniform named " << name << " found" << std::endl;
	}

	static void bindValue(GLuint uniform, const T& value);
};

template <typename T>
class Uniform<std::vector<T>>
{
public:
	static void bind(const std::string& name, SceneSetting* setting, const std::vector<T> vec)
	{
		Uniform<std::vector<T>>::bind(name, setting->m_activeShader->m_programObject, vec);
	}
	static void bind(const std::string& name, GLuint program, const std::vector<T> vec)
	{
		for (size_t i = 0; i < vec.size(); i++)
		{
			std::string uniformName = name + "[" + std::to_string(i) + "]";
			GLuint uniform;
			if ((uniform = glGetUniformLocation(program, uniformName.c_str())) >= 0)
			{
				Uniform<T>::bindValue(uniform, vec[i]);
			}
			else std::cerr << "No uniform named " << uniformName << " found" << std::endl;
		}
	}
};
