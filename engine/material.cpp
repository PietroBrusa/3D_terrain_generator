/**
 * @file    material.cpp
 * @brief   Material class implementation
 *
 * @author  Giona Valsecchi (C) SUPSI [giona.valsecchi@supsi.ch] , Pietro Brusadelli (C) SUPSI [pietro.brusadelli@supsi.ch], Filippo De Simoni (C) SUPSI [filippo.desimoni@supsi.ch]
 */

#include "engine.h"

 // FreeGlut:
#include <GL/freeglut.h>

namespace Eng
{

	////////////////////
	// Material CLASS //
	////////////////////

	Material::Material(std::string name, glm::vec4 emission, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, float shiness)
		: Object(name),
		emission(emission),
		ambient(ambient),
		diffuse(diffuse),
		specular(specular),
		shininess(shiness),
		texture(nullptr),
		shader(nullptr),
		matEmissionUniformLoc(-1),
		matAmbientUniformLoc(-1),
		matDiffuseUniformLoc(-1),
		matSpecularUniformLoc(-1),
		matShininessUniformLoc(-1),
		hasTextureUniformLoc(-1)
	{}

	Material::~Material()
	{
		delete texture;
	}

	void Material::render(glm::mat4 modelview)
	{
		if (shader)
			shader->render();

		Shader* shader = Shader::getCurrentInstance();

		if (texture)
			texture->render(modelview);
	}

	void Material::loadShaderParams(Eng::Shader* shader)
	{
		if (!shader)
			return;

		matEmissionUniformLoc = shader->getParamLocation("matEmission");
		matAmbientUniformLoc = shader->getParamLocation("matAmbient");
		matDiffuseUniformLoc = shader->getParamLocation("matDiffuse");
		matSpecularUniformLoc = shader->getParamLocation("matSpecular");
		matShininessUniformLoc = shader->getParamLocation("matShininess");
		hasTextureUniformLoc = shader->getParamLocation("hasTexture");
	}

	void Material::renderShader(Eng::Shader* shader)
	{
		if (!shader)
			return;

		shader->setVec3(matEmissionUniformLoc, emission);
		shader->setVec3(matAmbientUniformLoc, ambient);
		shader->setVec3(matDiffuseUniformLoc, diffuse);
		shader->setVec3(matSpecularUniformLoc, specular);
		shader->setFloat(matShininessUniformLoc, shininess);
		shader->setBool(hasTextureUniformLoc, texture);
	}

	glm::vec4 Material::getEmission() const
	{
		return emission;
	}

	glm::vec4 Material::getAmbient() const
	{
		return ambient;
	}

	glm::vec4 Material::getDiffuse() const
	{
		return diffuse;
	}

	glm::vec4 Material::getSpecular() const
	{
		return specular;
	}

	float Material::getShininess() const
	{
		return shininess;
	}

	Eng::Texture* Material::getTexture() const
	{
		return texture;
	}

	Eng::Shader* Material::getShader() const
	{
		return shader;
	}

	void Material::setEmission(const glm::vec4& emission_)
	{
		emission = emission_;
	}

	void Material::setAmbient(const glm::vec4& ambient_)
	{
		ambient = ambient_;
	}

	void Material::setDiffuse(const glm::vec4& diffuse_)
	{
		diffuse = diffuse_;
	}

	void Material::setSpecular(const glm::vec4& specular_)
	{
		specular = specular_;
	}

	void Material::setShininess(float shininess_)
	{
		shininess = shininess_;
	}

	void Material::setTexture(Eng::Texture* texture_)
	{
		texture = texture_;
	}

	void Material::setShader(Eng::Shader* shader_)
	{
		shader = shader_;
	}

}; // end of namespace Eng::
