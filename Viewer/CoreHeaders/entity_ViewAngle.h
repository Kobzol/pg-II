#pragma once

#include "entity_VAO.h"

#include "../code/uniform.h"

class ViewAngleEntity : public Entity_VAO
{
public:
	ViewAngleEntity(VAO* vao = nullptr) : Entity_VAO(vao)
	{
		
	}
	~ViewAngleEntity(void) {}

	void draw(const unsigned int eid = 0);
};

inline void ViewAngleEntity::draw(const unsigned int eid)
{
	if (!m_isInitialized) return;

	SceneSetting* ss = SceneSetting::GetInstance();
	Uniform<glm::mat4>::bind("MMatrix", ss->m_activeShader->m_programObject, this->m_modelMatrix);

	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindVertexArray(m_vao->m_object);
	glDrawArrays(GL_QUADS, 0, 4);
	glBindVertexArray(0);

	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
}
