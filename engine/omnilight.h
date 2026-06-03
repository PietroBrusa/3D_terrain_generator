/**
 * @file	omnilight.h
 * @brief	OmniLight class header file
 *
 * @author	Giona Valsecchi (C) SUPSI [giona.valsecchi@supsi.ch] , Pietro Brusadelli (C) SUPSI [pietro.brusadelli@supsi.ch], Filippo De Simoni (C) SUPSI [filippo.desimoni@supsi.ch]
 */

#pragma once
#include "shader.h"

 /**
  * @brief Implements an omnidirectional (point) light source.
  *
  * An OmniLight emits light equally in all directions from a specific point in space.
  * Its intensity typically diminishes over distance based on the attenuation factors
  * inherited from the Eng::Light base class. This is used to simulate light bulbs and small local sources.
  */
class ENG_API OmniLight : public Eng::Light
{
private:
    int lightTypeUniformLoc;
public:
    /**
     * @brief Constructor for the OmniLight.
     * @param name The name of the light.
     * @param matrix The initial transformation matrix, where the translation component defines the light's position.
     */
    OmniLight(std::string name = "", glm::mat4 matrix = glm::mat4(1.0f));
    
    /** @brief Virtual destructor for the OmniLight. */
    virtual ~OmniLight();

    /**
     * @brief Load parameter from shader
     * @param shader Pointer to the current shader instance.
    */
    void loadShaderParams(Eng::Shader* shader) override;

    /**
     * @brief Passes the omnilight parameters (position) to the active shader.
     * @param shader Pointer to the current shader instance.
     * @param modelview The current Model-View matrix to transform light coordinates into Eye Space.
     */
    void renderShader(Eng::Shader* shader, glm::mat4 modelview) override;
};
