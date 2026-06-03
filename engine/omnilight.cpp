/**
 * @file    omnilight.cpp
 * @brief   OmniLight class implementation
 *
 * @author  Giona Valsecchi (C) SUPSI [giona.valsecchi@supsi.ch] , Pietro Brusadelli (C) SUPSI [pietro.brusadelli@supsi.ch], Filippo De Simoni (C) SUPSI [filippo.desimoni@supsi.ch]
 */

#include "engine.h"

 // Freeglut:
#include <GL/freeglut.h>

namespace Eng
{

	/////////////////////
	// OmniLight CLASS //
	/////////////////////

	OmniLight::OmniLight(std::string name, glm::mat4 matrix)
		: Light(name, matrix),
		lightTypeUniformLoc(-1)
	{}

	OmniLight::~OmniLight()
	{}

	void OmniLight::loadShaderParams(Eng::Shader* shader)
	{
		if (!shader)
			return;

		Light::loadShaderParams(shader);

		std::string prefix = "[" + std::to_string(getLightID()) + "]";
		lightTypeUniformLoc = shader->getParamLocation(("lightType" + prefix).c_str());
	}

	void OmniLight::renderShader(Eng::Shader* shader, glm::mat4 modelview)
	{
		if (!shader)
			return;

		Light::renderShader(shader, modelview);

		shader->setInt(lightTypeUniformLoc, 0);
	}

}; // end of namespace Eng::