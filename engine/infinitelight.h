/**
 * @file	infinitelight.h
 * @brief	InfiniteLight class header file
 *
 * @author	Giona Valsecchi (C) SUPSI [giona.valsecchi@supsi.ch] , Pietro Brusadelli (C) SUPSI [pietro.brusadelli@supsi.ch], Filippo De Simoni (C) SUPSI [filippo.desimoni@supsi.ch]
 */

#pragma once

 /**
  * @brief Implements an infinite or directional light source.
  *
  * An InfiniteLight simulates a light source that is infinitely far away (like the sun).
  * All light rays are parallel, and the light intensity does not diminish with distance.
  * This light type is defined only by its direction, which is stored in the inherited
  * Eng::Node transformation matrix.
  */
class ENG_API InfiniteLight : public Eng::Light
{
private:
    /** @brief The direction vector in which the spotlight is pointing. */
    glm::vec3 direction;

    int lightTypeUniformLoc;
    int lightDirectionUniformLoc;
public:
    /**
     * @brief Constructor for the InfiniteLight.
     * @param name The name of the light.
     * @param matrix The initial transformation matrix, where the Z-axis defines the light's direction.
     * @param direction The orientation vector of the light cone (default pointing towards +Z).
     */
    InfiniteLight(std::string name = "", glm::mat4 matrix = glm::mat4(1.0f), glm::vec3 direction = glm::vec3(0.0f, 0.0f, 1.0f));
    
    /** @brief Virtual destructor for the InfiniteLight. */
    virtual ~InfiniteLight();
    
    /**
     * @brief Deletes the attenuation setter from the base Light class.
     * @note Attenuation is not applicable to directional lights because they have infinite range.
     * @delete
     */
    void setAttenuaton(float constant = 1, float linear = 0, float quadratic = 0) = delete;

    /**
     * @brief Load parameter from shader
     * @param shader Pointer to the current shader instance.
    */
    void loadShaderParams(Eng::Shader* shader) override;

    /**
     * @brief Passes the infinitelight parameters (position and direction) to the active shader.
     * @param shader Pointer to the current shader instance.
     * @param modelview The current Model-View matrix to transform light coordinates into Eye Space.
     */
    void renderShader(Eng::Shader* shader, glm::mat4 modelview) override;

    /**
     * @brief Retrieves the direction vector of the light.
     * @return The 3D direction vector.
     */
    glm::vec3 getDirection() const;

    /**
     * @brief Sets the direction vector for the spotlight.
     * @param dir The new direction vector.
     */
    void setDirection(glm::vec3 dir);
};
