/**
 * @file    texture.h
 * @brief   Texture class
 *
 * @author  Giona Valsecchi (C) SUPSI [giona.valsecchi@supsi.ch] , Pietro Brusadelli (C) SUPSI [pietro.brusadelli@supsi.ch], Filippo De Simoni (C) SUPSI [filippo.desimoni@supsi.ch]
 */

#pragma once

 /** @brief Enumeration to represent the source type of the texture data. */
enum class TextureSource {
    FILE,
    MEMORY,
    NONE
};

/**
 * @brief Manages loading, storing, and applying image data as a texture map.
 *
 * The Texture class handles the process of loading an image file, uploading it to the
 * graphics processing unit (GPU) memory, and providing a handle (\c texId) for use
 * during the rendering pipeline. 
 */
class ENG_API Texture : public Eng::Object
{
public:
    /**
     * @brief Constructor for the Texture class.
     *
     * Initializes the texture object and immediately attempts to load the image from the specified path.
     * @param name The name of the texture object.
     * @param filePath The file system path to the image file (e.g., "assets/image.png").
     */
    Texture(std::string name = "", const std::string& filePath = "");

    /**
     * @brief Constructor that uploads raw float RGB data directly to the GPU.
     * @param name The name of the texture object.
     * @param width Image width in pixels.
     * @param height Image height in pixels.
     * @param data Float RGB data, 3 floats per pixel, values in [0,1].
     */
    Texture(std::string name, int width, int height, const std::vector<float>& data);

    /**
     * @brief Virtual destructor for the Texture class.
     *
     * Responsible for releasing the GPU resource associated with the texture ID.
     */
    virtual ~Texture();

    /**
     * @brief Binds the texture object to the active texture unit in the rendering pipeline.
     *
     * This makes the texture available for sampling by the current shader/material.
     * @param modelview This parameter is usually ignored for texture operations.
     */
    void render(glm::mat4 modelview = glm::mat4(1.0f)) override;

    void updateSubImage(int offsetX, int offsetY, int width, int height, const std::vector<float>& data, int totalImageWidth);

    unsigned int getTexId();

private:
    /** @brief The unique identifier (handle) used by the graphics API (e.g., OpenGL texture ID) for the texture data on the GPU. */
    unsigned int texId;
    /** @brief Flag indicating whether the texture data has been successfully loaded and uploaded to the GPU. */
    bool isLoadedOnGPU{false};
	/** @brief The source type of the texture, indicating whether it was loaded from a file, created from memory data, or is currently uninitialized. */
    TextureSource source{TextureSource::NONE};

	/** @brief Pending file path for the texture to be loaded from, if the texture source is a file. */
    std::string pendingFilePath{""};
    /** @brief Pending texture width to be uploaded to the GPU. */
    int pendingWidth{0};
    /** @brief Pending texture height to be uploaded to the GPU. */
    int pendingHeight{0};
    /** @brief Pending texture data to be uploaded to the GPU. */
    std::vector<float> pendingData{};

    /**
	* @brief Internal method to upload the texture data to the GPU.
    */
	void uploadToGPU();

    /**
     * @brief Internal method to load the image data from a file and upload it to the GPU.
     * @param filePath The file system path to the image file.
     */
    void loadTexture(const std::string& filePath);
};
