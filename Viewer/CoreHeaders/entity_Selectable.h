#pragma once

#include "entity.h"
#include "entity_Quad.h"
#include "../code/uniform.h"

class EntitySelectable : public Entity
{
public:
	EntitySelectable(Entity* entity, ShaderProgram* constShader, QuadEntity* quadEntity, ShaderProgram* quadShader)
		: entity(entity), constShader(constShader), quadEntity(quadEntity), quadShader(quadShader)
	{

	}

	void draw(const unsigned int eid)
	{
		this->counter += 0.05f;

		SceneSetting *ss = SceneSetting::GetInstance();
		ShaderProgram* origShader = ss->m_activeShader;

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilMask(0xFF);

		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
		ss->m_activeShader = origShader;
		ss->m_activeShader->enable();
		this->entity->draw();

		if (this->selected)
		{
			ss->m_activeShader = this->constShader;
			ss->m_activeShader->enable();

			Uniform<glm::vec4>::bind("color", ss->m_activeShader->m_programObject, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			//glStencilMask(0x00);
			glDisable(GL_DEPTH_TEST);

			this->entity->setScale(scale.x * this->scale.x, scale.y * this->scale.y, scale.z * this->scale.z);
			this->entity->draw();
			this->entity->setScale(scale.x, scale.y, scale.z);

			glStencilMask(0xFF);
			glEnable(GL_DEPTH_TEST);
		}

		if (this->selected)
		{
			ss->m_activeShader = this->quadShader;
			ss->m_activeShader->enable();

			glm::quat rotation = glm::rotate(glm::quat(), this->counter, glm::vec3(0.0f, 0.0f, 1.0f));

			glm::mat4 model = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, -1.0f));
			model *= glm::mat4_cast(rotation);
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 1.0f));

			this->quadEntity->m_modelMatrix = model;
			this->quadEntity->draw();
		}

		glDisable(GL_STENCIL_TEST);
	}

	Entity* entity;
	ShaderProgram* constShader;

	QuadEntity* quadEntity;
	ShaderProgram* quadShader;

	glm::vec3 scale{ 1.05f, 1.05f, 1.05f };
	bool selected = false;

	float counter = 0.0f;
};
