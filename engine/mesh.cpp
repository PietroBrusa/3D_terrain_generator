/**
 * @file    mesh.cpp
 * @brief   Mesh class implementation
 *
 * @author  Giona Valsecchi (C) SUPSI [giona.valsecchi@supsi.ch] , Pietro Brusadelli (C) SUPSI [pietro.brusadelli@supsi.ch], Filippo De Simoni (C) SUPSI [filippo.desimoni@supsi.ch]
 */

#include "engine.h"

#include <algorithm>

// GLEW:
#include <GL/glew.h>

// GLM:
#include <glm/gtc/matrix_inverse.hpp>

namespace Eng
{

    /////////////////
    // MESH CLASS //
    /////////////////

    Mesh::Mesh(const std::string& name, const glm::mat4& matrix,
        std::vector<glm::vec3> vertexes,
        std::vector<std::vector<glm::uvec3>> faces,
        std::vector<glm::vec3> normals,
        std::vector<glm::vec2> textureCoordinates)
        : Node(name, matrix),
        vertexes{ vertexes },
        lodFaces{ faces },
        normals{ normals },
        textureCoordinates{ textureCoordinates },
        activeLod(0),
        buffersInitialized{ false },
        material{ nullptr },
        vao(-1),
        vboVertexes(-1),
        vboFaces(-1),
        vboNormals(-1),
        vboTextureCoordinates(-1),
        modelviewUniformLoc(-1),
        normalMatrixUniformLoc(-1)
    {
    }

    Mesh::~Mesh()
    {
        deleteBuffers();
    }

    void Mesh::render(glm::mat4 modelview)
    {
        if (vertexes.empty())
            return;

		if (!buffersInitialized) // Lazy initialization of OpenGL buffers when the mesh is first rendered
			initBuffers();

        if (material)
            material->render();

        Eng::Shader* shader = Shader::getCurrentInstance();

        if (shader)
            this->renderShader(shader, modelview);

        // Rendering the mesh
        int currentLOD = std::clamp(activeLod, 0, (int)lodFaces.size() - 1);

        glBindVertexArray(vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboFacesList[currentLOD]);
        glDrawElements(GL_TRIANGLES, lodFaces[currentLOD].size() * 3, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

    void Mesh::loadShaderParams(Eng::Shader* shader)
    {
        if (!shader) 
            return;

        if (material)
            material->loadShaderParams(shader);

        modelviewUniformLoc = shader->getParamLocation("modelview");
        normalMatrixUniformLoc = shader->getParamLocation("normalMatrix");
    }

    void Mesh::renderShader(Eng::Shader* shader, glm::mat4 modelview)
    {
        if (!shader)
            return;

        if (material)
            material->renderShader(shader);

        glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(modelview));

        shader->setMatrix(modelviewUniformLoc, modelview);
        shader->setMatrix3(normalMatrixUniformLoc, normalMatrix);
    }

    void Mesh::setMaterial(Material* materialPtr)
    {
        this->material = materialPtr;
    }

    Material* Mesh::getMaterial()
    {
        return this->material;
    }

    void Mesh::setActiveLOD(unsigned int index)
    {
        if (index < getLodsCount()) this->activeLod = index;
    }

    std::vector<glm::vec3> Mesh::getVertexes() const
    {
        return vertexes;
    }

    std::vector<glm::uvec3> Mesh::getFaces() const
    {
        if (lodFaces.empty()) return {};
        return lodFaces[std::clamp(activeLod, 0, (int)lodFaces.size() - 1)];
    }

    std::vector<glm::uvec3> Mesh::getFacesForLOD(int lodIndex) const
    {
        if (lodFaces.empty()) return {};

        int safeIndex = std::clamp(lodIndex, 0, (int)lodFaces.size() - 1);
        return lodFaces[safeIndex];
    }

    std::vector<glm::vec3> Mesh::getNormals() const
    {
        return normals;
    }

    std::vector<glm::vec2> Mesh::getTextureCoordinates() const
    {
        return textureCoordinates;
    }

    int Mesh::getLodsCount() const
    {
        return vboFacesList.size();
    }

    void Mesh::initBuffers()
    {
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

        if (!vertexes.empty())
        {
            glGenBuffers(1, &vboVertexes);
			glBindBuffer(GL_ARRAY_BUFFER, vboVertexes);
			glBufferData(GL_ARRAY_BUFFER, vertexes.size() * sizeof(glm::vec3), vertexes.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
			glEnableVertexAttribArray(0);
        }

        if (!normals.empty())
        {
			glGenBuffers(1, &vboNormals);
			glBindBuffer(GL_ARRAY_BUFFER, vboNormals);
			glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
			glEnableVertexAttribArray(1);
        }

        if (!textureCoordinates.empty())
        {
			glGenBuffers(1, &vboTextureCoordinates);
			glBindBuffer(GL_ARRAY_BUFFER, vboTextureCoordinates);
			glBufferData(GL_ARRAY_BUFFER, textureCoordinates.size() * sizeof(glm::vec2), textureCoordinates.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
			glEnableVertexAttribArray(2);
        }

        if (!lodFaces.empty())
        {
            vboFacesList.resize(lodFaces.size());
            glGenBuffers(lodFaces.size(), vboFacesList.data());

            for (size_t i = 0; i < lodFaces.size(); ++i)
            {
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboFacesList[i]);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, lodFaces[i].size() * sizeof(glm::uvec3), lodFaces[i].data(), GL_STATIC_DRAW);
            }

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }

		glBindVertexArray(0);
		buffersInitialized = true;
		loadShaderParams(Shader::getCurrentInstance());
    }

    void Mesh::deleteBuffers()
    {
		if (!buffersInitialized) return;

        if (vboVertexes) glDeleteBuffers(1, &vboVertexes);
        if (vboFaces) glDeleteBuffers(1, &vboFaces);
        if (vboNormals) glDeleteBuffers(1, &vboNormals);
        if (vboTextureCoordinates) glDeleteBuffers(1, &vboTextureCoordinates);
        if (vao) glDeleteVertexArrays(1, &vao);

        if (!vboFacesList.empty())
        {
            glDeleteBuffers(vboFacesList.size(), vboFacesList.data());
            vboFacesList.clear();
        }
    }
}; // end of namespace Eng::