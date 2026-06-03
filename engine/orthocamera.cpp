/**
 * @file    orthocamera.cpp
 * @brief   OrthoCamera class implementation
 *
 * @author  Giona Valsecchi (C) SUPSI [giona.valsecchi@supsi.ch] , Pietro Brusadelli (C) SUPSI [pietro.brusadelli@supsi.ch], Filippo De Simoni (C) SUPSI [filippo.desimoni@supsi.ch]
 */

#include "engine.h"

namespace Eng
{

    //////////////////
    // OrthoCamera CLASS //
    //////////////////

    OrthoCamera::OrthoCamera(const std::string &name,
                             const glm::mat4 &matrix)
        : Camera(name, matrix)
    {
    }

    OrthoCamera::~OrthoCamera()
    {
    }

    void OrthoCamera::render(glm::mat4 modelview)
    {
    }

    float OrthoCamera::getLeft()
    {
        return m_left;
    }

    float OrthoCamera::getRight()
    {
        return m_right;
    }

    float OrthoCamera::getBottom()
    {
        return m_bottom;
    }

    float OrthoCamera::getTop()
    {
        return m_top;
    }

    void OrthoCamera::setCameraParams(int width, int heigth, float near_plane, float far_plane)
    {
        float halfWidth = (float)width / 2.0f;
        float halfHeight = (float)heigth / 2.0f;

        setCameraParams(-halfWidth, halfWidth, -halfHeight, halfHeight, near_plane, far_plane);
    }

    void OrthoCamera::setCameraParams(float left, float right, float bottom, float top, float near_plane, float far_plane)
    {
        if (right < left)
        {
            float tmp = right;
            right = left;
            left = tmp;
        }

        if (top < bottom)
        {
            float tmp = top;
            top = bottom;
            bottom = tmp;
        }

        if (far_plane < near_plane)
        {
            float tmp = far_plane;
            far_plane = near_plane;
            near_plane = tmp;
        }

        // Setting up internal vars:
        m_left = left;
        m_right = right;
        m_bottom = bottom;
        m_top = top;

        m_near_plane = near_plane;
        m_far_plane = far_plane;

        setProjectionMatrix(glm::ortho(left, right, bottom, top, near_plane, far_plane));
    }
}; // end of namespace Eng::