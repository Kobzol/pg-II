////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	entity_Cube.h
//
// summary:	Declaration and implementation of Entity_Cube class
// author:	Petr Gajdoš
// 
// Copyright © 2014 Petr Gajdoš. All Rights Reserved.
//////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __ENTITY_CUBE_H_
#define __ENTITY_CUBE_H_

#include "entity_VAO.h"

class Entity_Cube : public Entity_VAO
{
public:
	Entity_Cube(VAO* vao = nullptr) : Entity_VAO(vao) {}
	~Entity_Cube(void){}

	void draw(const unsigned int eid = 0);
};

inline void Entity_Cube::draw(const unsigned int eid)
{
	if (!m_isInitialized) return;
	
	glBindVertexArray(m_vao->m_object);
	glDrawArrays(GL_QUADS, 0, 24);							//Warming!!! GL_QUADS a deprecated since OpenGL 3.1
	//glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0); 
}

#endif