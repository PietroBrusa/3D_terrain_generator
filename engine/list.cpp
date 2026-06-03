/**
 * @file    list.cpp
 * @brief   List class implementation
 *
 * @author  Giona Valsecchi (C) SUPSI [giona.valsecchi@supsi.ch] , Pietro Brusadelli (C) SUPSI [pietro.brusadelli@supsi.ch], Filippo De Simoni (C) SUPSI [filippo.desimoni@supsi.ch]
 */

#include "engine.h"

#include <iostream>

namespace Eng
{

    ////////////////
    // List CLASS //
    ////////////////

    List::List(std::string name)
        : Object(name),
        camera(nullptr),
        projectionUniformLoc(-1)
    {}

    List::~List()
    {}

    void List::render(glm::mat4 modelview)
    {
        if (camera == nullptr)
        {
            std::cerr << "No camera found" << std::endl;
            return;
        }

        glm::mat4 viewMatrix = camera->getViewMatrix();

        bool shaderChanged = Shader::shaderChangeReaded();
        Shader* currentShader = Shader::getCurrentInstance();

        if (shaderChanged)
        {
            projectionUniformLoc = currentShader->getParamLocation("projection");
            currentShader->setMatrix(projectionUniformLoc, camera->getProjectionMatrix());
        }

        for (auto inst : list)
        {
            Light* light = dynamic_cast<Light*>(inst.node);
            if (light)
            {
                if (shaderChanged)
                    light->loadShaderParams(currentShader);

                glm::mat4 lightModelView = viewMatrix * inst.nodeWorldMatrix;
                inst.node->render(lightModelView);
            }
        }

        for (auto inst : list) 
        {
            Mesh* mesh = dynamic_cast<Mesh*>(inst.node);
            if (mesh)
            {
                if (shaderChanged)
                    mesh->loadShaderParams(currentShader);

                glm::mat4 modelViewModel = viewMatrix * inst.nodeWorldMatrix;
                inst.node->render(modelViewModel);
            }
        }
    }

    void List::pass(Node* root, glm::mat4 matrix)
    {
        matrix = matrix * root->getMatrix();

        Instance inst;
        inst.node = root;
        inst.nodeWorldMatrix = root->getWorldCoordinateMatrix();

        if (dynamic_cast<Light*>(inst.node) != nullptr)
        {
            this->list.push_front(inst);
        }
        else if (dynamic_cast<Mesh*>(inst.node) != nullptr)
        {
            this->list.push_back(inst);
        }

        for (Node* child : root->getChildren())
        {
            if (child)
            {
                this->pass(child, matrix);
            }
        }
    }

    void List::setCamera(Eng::Camera* camera)
    {
        if (camera == nullptr)
        {
            std::cerr << "Cannot set a null camera" << std::endl;
            return;
        }

        this->camera = camera;

        Shader* currentShader = Shader::getCurrentInstance();
        if (currentShader)
            currentShader->setMatrix(projectionUniformLoc, camera->getProjectionMatrix());
    }

    void List::clear()
    {
        list.clear();
    }

}; // end of namespace Eng::