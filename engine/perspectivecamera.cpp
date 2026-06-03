/**
 * @file    perspective.cpp
 * @brief   PerspectiveCamera class implementation
 *
 * @author  Giona Valsecchi (C) SUPSI [giona.valsecchi@supsi.ch] , Pietro Brusadelli (C) SUPSI [pietro.brusadelli@supsi.ch], Filippo De Simoni (C) SUPSI [filippo.desimoni@supsi.ch]
 */

#include "engine.h"

namespace Eng
{

    /////////////////////////////
    // PerspectiveCamera CLASS //
    /////////////////////////////

    PerspectiveCamera::PerspectiveCamera(const std::string &name,
                                         const glm::mat4 &matrix) : Camera(name, matrix) {}

    PerspectiveCamera::~PerspectiveCamera() {}

    void PerspectiveCamera::render(glm::mat4 modelview)
    {
    }

    float PerspectiveCamera::getFOV()
    {
        return m_fov;
    }

    float PerspectiveCamera::getRatio()
    {
        return m_ratio;
    }

    void PerspectiveCamera::setCameraParams(float fov, float ratio, float near_plane, float far_plane)
    {
        if (fov < 0)
        {
            fov = 100.0f;
        }
        if (ratio < 0)
        {
            ratio = 16.0 / 9.0;
        }

        if (far_plane < near_plane)
        {
            float tmp = far_plane;
            far_plane = near_plane;
            near_plane = tmp;
        }

        // Setting up internal vars:
        m_fov = fov;
        m_ratio = ratio;

        m_near_plane = near_plane;
        m_far_plane = far_plane;

        setProjectionMatrix(glm::perspective(glm::radians(fov), ratio, near_plane, far_plane));
    } 
}; // end of namespace Eng::
