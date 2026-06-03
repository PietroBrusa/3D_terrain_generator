/**
 * @file	orthoOrthoCamera.h
 * @brief	OrthoCamera class header file
 *
 * @author	Giona Valsecchi (C) SUPSI [giona.valsecchi@supsi.ch] , Pietro Brusadelli (C) SUPSI [pietro.brusadelli@supsi.ch], Filippo De Simoni (C) SUPSI [filippo.desimoni@supsi.ch]
 */

#pragma once

 /**
  * @brief Implements a camera with an orthographic (parallel) projection.
  *
  * In an OrthoCamera, objects maintain their size regardless of their distance from the viewer.
  * The viewing volume is a simple rectangular cuboid, defined by its left, right, top, and bottom bounds,
  * as well as the near and far clipping planes.
  */
class ENG_API OrthoCamera : public Eng::Camera
{
private:
    /** @brief The left boundary of the view volume in view space. */
    float m_left;
    /** @brief The right boundary of the view volume in view space. */
    float m_right;
    /** @brief The bottom boundary of the view volume in view space. */
    float m_bottom;
    /** @brief The top boundary of the view volume in view space. */
    float m_top;

public:
    /**
     * @brief Constructor for the OrthoCamera.
     * @param name The name of the camera.
     * @param matrix The initial transformation matrix (position and orientation).
     */
    OrthoCamera(const std::string& name = "", const glm::mat4& matrix = glm::mat4(1.0f));
    
    /** @brief Virtual destructor for the OrthoCamera. */
    virtual ~OrthoCamera();

    /**
     * @brief Renders the camera, which typically means setting the engine's view and orthographic projection matrices.
     * @param modelview This parameter is usually ignored for a Camera's render method.
     */
    void render(glm::mat4 modelview = glm::mat4(1.0f)) override;

    /////////////
    // Getters //
    /////////////

    /**
     * @brief Gets the current left boundary of the view volume.
     * @return The \c m_left value.
     */
    float getLeft();

    /**
     * @brief Gets the current right boundary of the view volume.
     * @return The \c m_right value.
     */
    float getRight();

    /**
     * @brief Gets the current bottom boundary of the view volume.
     * @return The \c m_bottom value.
     */
    float getBottom();

    /**
     * @brief Gets the current top boundary of the view volume.
     * @return The \c m_top value.
     */
    float getTop();

    /////////////
    // Setters //
    /////////////

    /**
     * @brief Sets the camera's orthographic parameters based on screen dimensions and clipping planes.
     *
     * This is a utility function that typically centers the view at (0,0).
     * @param width The width of the viewport/scene area.
     * @param heigth The height of the viewport/scene area.
     * @param near_plane The distance to the near clipping plane.
     * @param far_plane The distance to the far clipping plane.
     */
    void setCameraParams(int width, int heigth, float near_plane, float far_plane);
    
    /**
     * @brief Directly sets the boundary dimensions and clipping planes for the orthographic projection.
     * @param left The left boundary coordinate.
     * @param right The right boundary coordinate.
     * @param bottom The bottom boundary coordinate.
     * @param top The top boundary coordinate.
     * @param near_plane The distance to the near clipping plane.
     * @param far_plane The distance to the far clipping plane.
     */
    void setCameraParams(float left, float right, float bottom, float top, float near_plane, float far_plane);
};
