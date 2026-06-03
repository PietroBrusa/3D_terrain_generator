/**
 * @file    material.h
 * @brief   Material class
 *
 * @author  Giona Valsecchi (C) SUPSI [giona.valsecchi@supsi.ch] , Pietro Brusadelli (C) SUPSI [pietro.brusadelli@supsi.ch], Filippo De Simoni (C) SUPSI [filippo.desimoni@supsi.ch]
 */

#pragma once

/**
 * @brief Defines the surface properties and appearance of a geometric object (Mesh).
 *
 * The Material class manages the color components (ambient, diffuse, specular), shininess,
 * and texture used when a Mesh is rendered. It controls how light is reflected off the surface.
 * 
 */
class ENG_API Material : public Eng::Object
{
    /** @brief Pointer to the texture object applied to the surface. */
    Eng::Texture* texture;

    /** @brief Pointer to the shader object applied on material */
    Eng::Shader* shader;

    /** @brief The color component the material emits, regardless of scene lighting. */
    glm::vec4 emission;

    /** @brief The color component of the material that reflects ambient light. */
    glm::vec4 ambient;

    /** @brief The color component that reflects diffuse light (directionally independent shading). */
    glm::vec4 diffuse;

    /** @brief The color component that reflects specular light (highlights). */
    glm::vec4 specular;

    /** @brief The exponent controlling the sharpness of specular highlights (controls reflection falloff). */
    float shininess;

    int matEmissionUniformLoc;
    int matAmbientUniformLoc;
    int matDiffuseUniformLoc;
    int matSpecularUniformLoc;
    int matShininessUniformLoc;
    int hasTextureUniformLoc;

public:
    /**
     * @brief Constructor for the Material class.
     * @param name The name of the material.
     * @param emission The initial emission color (default is black).
     * @param ambient The initial ambient reflection color (default is dark gray).
     * @param diffuse The initial diffuse reflection color (default is light gray).
     * @param specular The initial specular reflection color (default is medium gray).
     * @param shiness The initial shininess exponent (default 128.0f for a sharp highlight).
     */
    Material(std::string name = "", glm::vec4 emission = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4 ambient = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f), glm::vec4 diffuse = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f), glm::vec4 specular = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), float shiness = 128.0f);

    /** @brief Virtual destructor for the Material class. */
    virtual ~Material();

    // -------------------------------------------------------------------------
    // CORE VIRTUAL METHOD IMPLEMENTATION
    // -------------------------------------------------------------------------

    /**
     * @brief Sets the material's properties (colors, shininess, texture) in the active rendering pipeline state.
     * @param modelview This parameter is typically ignored, as material properties are independent of the modelview matrix.
     */
    virtual void render(glm::mat4 modelview = glm::mat4(1.0f)) override;

    /**
     * @brief Load parameter from shader
     * @param shader Pointer to the current shader instance.
    */
    virtual void loadShaderParams(Eng::Shader* shader);

    /**
     * @brief Passes the mesh parameters to the active shader.
     * @param shader Pointer to the current shader instance.
     */
    virtual void renderShader(Eng::Shader* shader);

    /////////////
    // Getters //
    /////////////

    /**
     * @brief Gets the material's emission color.
     * @return The \c glm::vec4 emission color.
     */
    glm::vec4 getEmission() const;

    /**
     * @brief Gets the material's ambient color.
     * @return The \c glm::vec4 ambient color.
     */
    glm::vec4 getAmbient() const;

    /**
     * @brief Gets the material's diffuse color.
     * @return The \c glm::vec4 diffuse color.
     */
    glm::vec4 getDiffuse() const;

    /**
     * @brief Gets the material's specular color.
     * @return The \c glm::vec4 specular color.
     */
    glm::vec4 getSpecular() const;

    /**
     * @brief Gets the material's shininess exponent.
     * @return The shininess float value.
     */
    float getShininess() const;

    /**
     * @brief Gets the texture associated with the material.
     * @return A pointer to the \c Eng::Texture object.
     */
    Eng::Texture* getTexture() const;

    /**
     * @brief Gets the shader associated with the material.
     * @return A pointer to the \c Eng::Shader object.
     */
    Eng::Shader* getShader() const;

    /////////////
    // Setters //
    /////////////

    /**
     * @brief Sets the material's emission color.
     * @param emission_ The new \c glm::vec4 emission color.
     */
    void setEmission(const glm::vec4& emission_);

    /**
     * @brief Sets the material's ambient color.
     * @param ambient_ The new \c glm::vec4 ambient color.
     */
    void setAmbient(const glm::vec4& ambient_);

    /**
     * @brief Sets the material's diffuse color.
     * @param diffuse_ The new \c glm::vec4 diffuse color.
     */
    void setDiffuse(const glm::vec4& diffuse_);

    /**
     * @brief Sets the material's specular color.
     * @param specular_ The new \c glm::vec4 specular color.
     */
    void setSpecular(const glm::vec4& specular_);

    /**
     * @brief Sets the material's shininess exponent.
     * @param shininess_ The new shininess float value.
     */
    void setShininess(float shininess_);

    /**
     * @brief Sets the texture associated with the material.
     * @param texture_ A pointer to the \c Eng::Texture object.
     */
    void setTexture(Eng::Texture* texture_);

    /**
     * @brief Sets the shader associated with the material.
     * @param shader_ A pointer to the \c Eng::Shader object.
     */
    void setShader(Eng::Shader* shader_);
};
