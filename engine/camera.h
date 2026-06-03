/**
 * @file	camera.h
 * @brief	Camera class header file
 *
 * @author	Giona Valsecchi (C) SUPSI [giona.valsecchi@supsi.ch] , Pietro Brusadelli (C) SUPSI [pietro.brusadelli@supsi.ch], Filippo De Simoni (C) SUPSI [filippo.desimoni@supsi.ch]
 */

#pragma once

//////////////////
// Camera CLASS //
//////////////////

/**
 * @brief Abstract base class for all cameras in the engine.
 *
 * The Camera class inherits from Eng::Node, allowing it to be positioned and oriented
 * within the scene graph. It manages the projection matrix and defines the near and
 * far clipping planes. Concrete camera types (e.g., Perspective, Orthographic)
 * will derive from this class and implement their specific projection logic.
 */
class ENG_API Camera : public Eng::Node
{
private:
    /** @brief The projection matrix (e.g., perspective or orthographic) defining the viewing frustum. */
    glm::mat4 projectionMatrix;

protected:
    /** @brief The distance from the camera to the nearest visible object (near clipping plane). */
    float m_near_plane;

    /** @brief The distance from the camera to the furthest visible object (far clipping plane). */
    float m_far_plane;

public:
    /**
     * @brief Constructor for the Camera base class.
     *
     * Initializes the camera's position/orientation via the inherited Node matrix.
     * @param name The name of the camera (passed to the base Object class).
     * @param matrix The initial local transformation matrix (default is identity).
     */
    Camera(const std::string& name = "", const glm::mat4& matrix = glm::mat4(1.0f));
    
    /** @brief Virtual destructor for the Camera base class. */
    virtual ~Camera();

    ////////////
    // Setter //
    ////////////

    /**
     * @brief Retrieves the projection matrix.
     * @return The \c glm::mat4 projection matrix.
     */
    virtual glm::mat4 getProjectionMatrix() const;

    /**
     * @brief Sets the transformation matrix that defines the camera's world position and orientation.
     *
     * In computer graphics, the view matrix is typically the inverse of the camera's world matrix.
     * @param matrix The new view matrix (\c glm::mat4).
     */
    virtual void setViewMatrix(const glm::mat4& matrix);

    /**
     * @brief Sets the projection matrix directly.
     * @param matrix The new projection matrix (\c glm::mat4).
     */
    virtual void setProjectionMatrix(const glm::mat4& matrix);

    /////////////
    // Getters //
    /////////////

    /**
     * @brief Calculates and retrieves the current view matrix (World-to-View transformation).
     *
     * This is usually the inverse of the camera's world coordinate matrix (inherited from Node).
     * @return The \c glm::mat4 view matrix.
     */
    virtual glm::mat4 getViewMatrix() const;

    /**
     * @brief Gets the value of the near clipping plane.
     * @return The near plane distance.
     */
    float getNearPlane();

    /**
     * @brief Gets the value of the far clipping plane.
     * @return The far plane distance.
     */
    float getFarPlane();
};
