/**
 * @file	spotlight.h
 * @brief	SpotLight class header file
 *
 * @author	Giona Valsecchi (C) SUPSI [giona.valsecchi@supsi.ch] , Pietro Brusadelli (C) SUPSI [pietro.brusadelli@supsi.ch], Filippo De Simoni (C) SUPSI [filippo.desimoni@supsi.ch]
 */

#pragma once

/**
 * @brief Implements a light source that emits light in a focused cone shape.
 *
 * A SpotLight is defined by its position, direction, and a cutoff angle, which determines the
 * size of the cone of light. It inherits attenuation properties from Eng::Light.
 * 
 */
class ENG_API SpotLight : public Eng::Light
{
private:
    /** @brief The angle (in degrees or radians) defining the width of the light cone. Light is emitted only within this angle. */
    float cutoff;

    /** @brief The maximum distance at which the light has an effect. Used to calculate attenuation or as a spot exponent. */
    float influenceRadius;

    /** @brief The direction vector in which the spotlight is pointing. */
    glm::vec3 direction;

    int lightTypeUniformLoc;
    int spotCutoffCosineUniformLoc;
    int spotExponentUniformLoc;
    int lightDirectionUniformLoc;
public:
    /**
     * @brief Constructor for the SpotLight.
     * @param name The name of the light.
     * @param matrix The initial transformation matrix, where the position is the light origin and the Z-axis defines the direction.
     * @param direction The orientation vector of the light cone (default pointing towards -Y).
     * @param cutoff The initial cutoff angle for the light cone (default 45 degrees).
     * @param influenceRadius The radius of effect or concentration factor (default 8.0).
     */
    SpotLight(
        std::string name = "",
        glm::mat4 matrix = glm::mat4(1.0f),
        glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f),
        float cutoff = 45,
        float influenceRadius = 8.0f
    );

    /** @brief Virtual destructor for the SpotLight. */
    virtual ~SpotLight();

    /**
     * @brief Load parameter from shader
     * @param shader Pointer to the current shader instance.
    */
    void loadShaderParams(Eng::Shader* shader) override;

    /**
     * @brief Passes the spotlight parameters (position, direction, cutoff, and radius) to the active shader.
     * @param shader Pointer to the current shader instance.
     * @param modelview The current Model-View matrix to transform light coordinates into Eye Space.
     */
    void renderShader(Eng::Shader* shader, glm::mat4 modelview) override;

    /**
     * @brief Retrieves the current cutoff angle of the light cone.
     * @return The cutoff angle in the defined units (degrees or radians).
     */
    float getCutoff() const;

    /**
     * @brief Retrieves the influence radius or concentration exponent of the spotlight.
     * @return The radius value.
     */
    float getInfluenceRadius() const;

    /**
     * @brief Retrieves the direction vector of the light.
     * @return The 3D direction vector.
     */
    glm::vec3 getDirection() const;

    /**
     * @brief Sets the cutoff angle for the light cone.
     *
     * The input is clamped between 0 and 90 degrees to ensure a valid light cone shape.
     * @param cutoff_ The new cutoff angle to set.
     */
    void setCutoff(float cutoff_);

    /**
     * @brief Sets the radius of influence or the focus exponent for the light.
     * @param radius The new radius or exponent value.
     */
    void setInfluenceRadius(float radius);

    /**
     * @brief Sets the direction vector for the spotlight.
     * @param dir The new direction vector.
     */
    void setDirection(glm::vec3 dir);
};
