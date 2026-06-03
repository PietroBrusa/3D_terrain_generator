/**
 * @file    infinitelight.cpp
 * @brief   InfiniteLight class implementation
 *
 * @author  Giona Valsecchi (C) SUPSI [giona.valsecchi@supsi.ch] , Pietro Brusadelli (C) SUPSI [pietro.brusadelli@supsi.ch], Filippo De Simoni (C) SUPSI [filippo.desimoni@supsi.ch]
 */

#include "engine.h"


 // Freeglut:
#include <GL/freeglut.h>

namespace Eng
{

	/////////////////////////
	// InfiniteLight CLASS //
	/////////////////////////

	InfiniteLight::InfiniteLight(std::string name, glm::mat4 matrix, glm::vec3 direction)
		: Light(name, matrix),
		direction(direction),
		lightTypeUniformLoc(-1),
		lightDirectionUniformLoc(-1)
	{}

	InfiniteLight::~InfiniteLight()
	{}

	void InfiniteLight::loadShaderParams(Eng::Shader* shader)
	{
		if (!shader)
			return;

		Light::loadShaderParams(shader);

		std::string prefix = "[" + std::to_string(getLightID()) + "]";

		lightTypeUniformLoc = shader->getParamLocation(("lightType" + prefix).c_str());
		lightDirectionUniformLoc = shader->getParamLocation(("lightDirection" + prefix).c_str());
	}

	void InfiniteLight::renderShader(Eng::Shader* shader, glm::mat4 modelview)
	{
		if (!shader)
			return;

		Light::renderShader(shader, modelview);

		glm::vec3 eyeSpaceDirection = glm::vec3(modelview * glm::vec4(direction, 0.0f));
		glm::vec3 normalizedDirection = glm::normalize(eyeSpaceDirection);

		shader->setInt(lightTypeUniformLoc, 1);
		shader->setVec3(lightDirectionUniformLoc, normalizedDirection);
	}

	glm::vec3 InfiniteLight::getDirection() const
	{
		return direction;
	}

	void InfiniteLight::setDirection(glm::vec3 dir)
	{
		direction = dir;
	}

}; // end of namespace Eng::