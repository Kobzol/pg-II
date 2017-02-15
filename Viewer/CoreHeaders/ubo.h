////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	ubo.h
//
// summary:	Declaration and implementation of Uniform Buffer Object structure
// author:	Petr Gajdoš
// 
// Copyright © 2014 Petr Gajdoš. All Rights Reserved.
//////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __UBO_H_
#define __UBO_H_

#include <string.h>

#include "bufferObject.h"

/// <summary>	An UBO. 
/// 			In case of UBO class, the whole GL buffer objects is accessed via m_object variable of the BufferObject class. UBO does not contain any other sub-buffers, compared to e.g. FBO or VAO.</summary>
/// <remarks>	Gajdi, 7. 4. 2015. </remarks>
class UBO : public BufferObject
{
public:
	GLuint m_bindingPoint;

public:
	UBO(GLuint bindingPoint) : BufferObject(), m_bindingPoint(bindingPoint) {}
	virtual ~UBO(){ if (m_object) glDeleteBuffers(1, &m_object); }

	virtual void bind();
	virtual void unbind();
	virtual void clear();
	virtual void init();

	virtual void getUniformInfo(){}
};

inline void UBO::init()
{
	if (m_isInitialized)
	{
		clear();
	}
	m_isInitialized = false;
}

inline void UBO::clear()
{
	if (m_isInitialized)
	{
		m_isInitialized = false;
		glDeleteBuffers(1, &m_object);
	}
}

inline void UBO::bind()
{
	if (m_isInitialized)
		glBindBuffer(GL_UNIFORM_BUFFER, m_object);
}

inline void UBO::unbind()
{
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

#endif