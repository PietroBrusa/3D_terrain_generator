/**
 * @file	perspectivecamera.h
 * @brief	PerspectiveCamera class header file
 *
 * @author	Giona Valsecchi (C) SUPSI [giona.valsecchi@supsi.ch] , Pietro Brusadelli (C) SUPSI [pietro.brusadelli@supsi.ch], Filippo De Simoni (C) SUPSI [filippo.desimoni@supsi.ch]
 */

#pragma once

 /**
  * @brief Implements a camera with a perspective projection.
  *
  * The PerspectiveCamera simulates how objects appear smaller as their distance from the viewer increases.
  * It is defined by its Field of View (FOV) and Aspect Ratio, which create a viewing frustum
  * that is a truncated pyramid.
  */
class ENG_API PerspectiveCamera : public Eng::Camera
{
private:
    /** @brief The vertical angle (in degrees or radians) defining the extent of the scene visible to the camera (Field of View). */
    float m_fov;
    /** @brief The aspect ratio of the viewport, calculated as width divided by height ($w/h$). */
    float m_ratio;

public:
    /**
     * @brief Constructor for the PerspectiveCamera.
     * @param name The name of the camera.
     * @param matrix The initial transformation matrix (position and orientation).
     */
    PerspectiveCamera(const std::string& name = "", const glm::mat4& matrix = glm::mat4(1.0f));
    
    /** @brief Virtual destructor for the PerspectiveCamera. */
    virtual ~PerspectiveCamera();

    /**
     * @brief Renders the camera, which typically means setting the OpenGL/Engine's projection and view matrices.
     * @param modelview This parameter is usually ignored for a Camera's render method, as it sets the global view state.
     */
    void render(glm::mat4 modelview = glm::mat4(1.0f)) override;

    /////////////
    // Getters //
    /////////////

    /**
     * @brief Gets the current Field of View (FOV) angle.
     * @return The FOV value.
     */
    float getFOV();

    /**
     * @brief Gets the current aspect ratio ($w/h$).
     * @return The aspect ratio value.
     */
    float getRatio();

    /////////////
    // Setters //
    /////////////

    /**
     * @brief Sets all essential parameters for the perspective projection matrix.
     * @param fov The new Field of View angle.
     * @param ratio The new aspect ratio ($w/h$).
     * @param near_plane The new distance to the near clipping plane.
     * @param far_plane The new distance to the far clipping plane.
     */
    void setCameraParams(float fov, float ratio, float near_plane, float far_plane);
};
