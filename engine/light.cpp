/**
 * @file    light.cpp
 * @brief   Light class implementation
 *
 * @author  Giona Valsecchi (C) SUPSI [giona.valsecchi@supsi.ch] , Pietro Brusadelli (C) SUPSI [pietro.brusadelli@supsi.ch], Filippo De Simoni (C) SUPSI [filippo.desimoni@supsi.ch]
 */

#include "engine.h"

 // Freeglut:
#include <GL/freeglut.h>

#include <iostream>

int Eng::Light::lightCounter = 0;

namespace Eng {

	////////////////
	// Light CLASS //
	////////////////

	Light::Light(std::string name, glm::mat4 matrix, glm::vec4 position)
		: Node(name, matrix),
		attenuation(1.0f, 0.0f, 0.0f),
		ambient(1.0f),
		diffuse(1.0f),
		specular(1.0f),
		position(position),
		lightPositionUniformLoc(-1),
		lightAmbientUniformLoc(-1),
		lightDiffuseUniformLoc(-1),
		lightSpecularUniformLoc(-1),
		// lightAttenuationUniformLoc(-1),
		activeLightCountUniformLoc(-1),
		shader(nullptr)
	{
		unsigned int maxLights = 8;

		if (lightCounter < maxLights)
		{
			lightId = lightCounter++;
			std::cout << "Enabled light: " << lightId << std::endl;
		}
		else
		{
			lightId = -1;
			std::cerr << "Warning: Max OpenGL lights reached!" << std::endl;
		}
	}

	Light::~Light() {
		if (lightId != -1) {
			lightCounter--;
		}
	}

	void Light::render(glm::mat4 modelview) {
		if (lightId == -1) return;

		if (shader)
			shader->render();


		Eng::Shader* shader = Shader::getCurrentInstance();

		if (shader)
			this->renderShader(shader, modelview);
	}

	void Light::loadShaderParams(Eng::Shader* shader)
	{
		if (!shader)
			return;

		std::string prefix = "[" + std::to_string(lightId) + "]";

		lightPositionUniformLoc = shader->getParamLocation(("lightPosition" + prefix).c_str());
		lightAmbientUniformLoc = shader->getParamLocation(("lightAmbient" + prefix).c_str());
		lightDiffuseUniformLoc = shader->getParamLocation(("lightDiffuse" + prefix).c_str());
		lightSpecularUniformLoc = shader->getParamLocation(("lightSpecular" + prefix).c_str());
		// lightAttenuationUniformLoc = shader->getParamLocation(("lightAttenuation" + prefix).c_str());
		activeLightCountUniformLoc = shader->getParamLocation("activeLightCount");
	}

	void Light::renderShader(Eng::Shader* shader, glm::mat4 modelview)
	{
		if (!shader)
			return;

		shader->setVec3(lightPositionUniformLoc, glm::vec3(modelview[3]));
		shader->setVec3(lightAmbientUniformLoc, getAmbient());
		shader->setVec3(lightDiffuseUniformLoc, getDiffuse());
		shader->setVec3(lightSpecularUniformLoc, getSpecular());
		// shader->setVec3(lightAttenuationUniformLoc, attenuation);
		shader->setInt(activeLightCountUniformLoc, lightCounter);
	}

	int Light::getLightID() {
		return lightId;
	}

	glm::vec4 Light::getAmbient() {
		return ambient;
	}

	glm::vec4 Light::getDiffuse() {
		return diffuse;
	}

	glm::vec4 Light::getSpecular() {
		return specular;
	}

	glm::vec4 Light::getPosition()
	{
		return position;
	}

	Eng::Shader* Light::getShader() const
	{
		return shader;
	}

	void Light::setAmbient(glm::vec4 v) {
		ambient = v;
	}

	void Light::setDiffuse(glm::vec4 v) {
		diffuse = v;
	}

	void Light::setSpecular(glm::vec4 v) {
		specular = v;
	}

	void Light::setAttenuaton(float constant, float linear, float quadratic) {
		attenuation = glm::vec3(constant, linear, quadratic);
	}

	void Light::setPosition(glm::vec4 p)
	{
		position = p;
	}

	void Light::setShader(Eng::Shader* shader_)
	{
		shader = shader_;
	}

};  // namespace Eng
