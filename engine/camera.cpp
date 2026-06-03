/**
 * @file    camera.cpp
 * @brief   Camera class implementation
 *
 * @author  Giona Valsecchi (C) SUPSI [giona.valsecchi@supsi.ch] , Pietro Brusadelli (C) SUPSI [pietro.brusadelli@supsi.ch], Filippo De Simoni (C) SUPSI [filippo.desimoni@supsi.ch]
 */

#include "engine.h"

// Note: Ensure you include all necessary GLM headers in mesh.h for compilation

namespace Eng
{

    //////////////////
    // CAMERA CLASS //
    //////////////////

    // Constructor Definition
    Camera::Camera(const std::string &name,
                   const glm::mat4 &matrix)
        : Node(name, matrix)
    {
    }

    Camera::~Camera()
    {
    }

    glm::mat4 Camera::getProjectionMatrix() const { return projectionMatrix; };

    void Camera::setViewMatrix(const glm::mat4 &matrix) { setMatrix(matrix); }

    void Camera::setProjectionMatrix(const glm::mat4 &matrix) { projectionMatrix = matrix; }

    glm::mat4 Camera::getViewMatrix() const
    {
        return glm::inverse(getWorldCoordinateMatrix());
    }

    float Camera::getNearPlane()
    {
        return m_near_plane;
    }

    float Camera::getFarPlane()
    {
        return m_far_plane;
    }
}; // end of namespace Eng::