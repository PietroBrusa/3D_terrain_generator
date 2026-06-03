/**
 * @file	mesh.h
 * @brief	Mesh class header file
 *
 * @author	Giona Valsecchi (C) SUPSI [giona.valsecchi@supsi.ch] , Pietro Brusadelli (C) SUPSI [pietro.brusadelli@supsi.ch], Filippo De Simoni (C) SUPSI [filippo.desimoni@supsi.ch]
 */

#pragma once

 /**
  * @brief Represents a renderable 3D geometric object.
  *
  * The Mesh class holds the vertex data (position, normals, texture coordinates)
  * and face indices that define the geometry. It inherits from Eng::Node to be
  * part of the scene graph and is associated with an Eng::Material for rendering properties.
  */
class ENG_API Mesh : public Eng::Node
{
private:
    /** @brief Vector containing the 3D position coordinates for all vertices of the mesh. */
    std::vector<glm::vec3> vertexes;
    /** @brief Vector containing the lods vector containing the indices that define the faces (triangles) of the mesh (e.g., triplets of vertex indices). */
    std::vector<std::vector<glm::uvec3>> lodFaces;
    /** @brief Vector containing the 4D normal vectors (often w=0 or w=1 for homogeneous coordinates) for each vertex, used for lighting calculations. */
    std::vector<glm::vec3> normals;
    /** @brief Vector containing the 2D texture coordinates (UVs) for each vertex, used to map textures onto the mesh. */
    std::vector<glm::vec2> textureCoordinates;

    /** @brief Vector containing the LOD indexes */
    std::vector<unsigned int> vboFacesList;

    /** @brief The current active level of detail */
    int activeLod;

    /* Indexes for VBO/VAO */
    /** @brief OpenGL Vertex Array Object ID for this mesh. */
    unsigned int vao;
    /** @brief OpenGL Vertex Buffer Object ID for vertex positions. */
    unsigned int vboVertexes;
    /** @brief OpenGL Vertex Buffer Object ID for face indices. */
    unsigned int vboFaces;
    /** @brief OpenGL Vertex Buffer Object ID for vertex normals. */
    unsigned int vboNormals;
    /** @brief OpenGL Vertex Buffer Object ID for texture coordinates. */
    unsigned int vboTextureCoordinates;

    /** @brief Flag indicating whether OpenGL buffers have been initialized for this mesh. */
    bool buffersInitialized;

    /** @brief Pointer to the material object that defines the mesh's appearance (color, textures, shaders). */
    Eng::Material* material;

    /** @brief Initializes OpenGL buffers (VAO/VBO) for the mesh geometry. */
    void initBuffers();

    /** @brief Deletes OpenGL buffers to free GPU resources when the mesh is destroyed. */
    void deleteBuffers();

    int modelviewUniformLoc;
    int normalMatrixUniformLoc;

public:
    /**
     * @brief Constructor for the Mesh class.
     *
     * Initializes the mesh with optional geometry data.
     * @param name The name of the mesh (passed to Node).
     * @param matrix The initial local transformation matrix (passed to Node, default is identity).
     * @param vertexes The initial array of vertex positions.
     * @param faces The initial array of face indices.
     * @param normals The initial array of vertex normals.
     * @param textureCoordinates The initial array of texture coordinates.
     */
    Mesh(const std::string& name = "", const glm::mat4& matrix = glm::mat4(1.0f),
        std::vector<glm::vec3> vertexes = std::vector<glm::vec3>(),
        std::vector<std::vector<glm::uvec3>> lodFaces = std::vector<std::vector<glm::uvec3>>(),
        std::vector<glm::vec3> normals = std::vector<glm::vec3>(),
        std::vector<glm::vec2> textureCoordinates = std::vector<glm::vec2>()
    );

    /** @brief Virtual destructor for the Mesh class. */
    virtual ~Mesh();

    /**
     * @brief Renders the mesh geometry using the associated material and the accumulated modelview matrix.
     *
     * This method typically binds the material, sets up OpenGL buffers, and issues a draw call.
     * @param modelview The combined Model-View matrix accumulated from the scene graph.
     */
    void render(glm::mat4 modelview = glm::mat4(1.0f)) override;

    /**
     * @brief Load parameter from shader
     * @param shader Pointer to the current shader instance.
    */
    virtual void loadShaderParams(Eng::Shader* shader);

    /**
     * @brief Passes the mesh parameters to the active shader.
     * @param shader Pointer to the current shader instance.
     * @param modelview The current Model-View matrix to transform mesh coordinates into Eye Space
     */
    virtual void renderShader(Eng::Shader* shader, glm::mat4 modelview);

    /**
     * @brief Sets the material that controls the appearance of the mesh.
     * @param materialPtr A pointer to the Eng::Material object.
     */
    void setMaterial(Eng::Material* materialPtr);

    /**
     * @brief Gets the material currently associated with the mesh.
     * @return A pointer to the Eng::Material object.
     */
    Eng::Material* getMaterial();

    void setActiveLOD(unsigned int index);

    std::vector<glm::vec3> getVertexes() const;
    std::vector<glm::uvec3> getFaces() const;
    std::vector<glm::uvec3> getFacesForLOD(int index) const;
    std::vector<glm::vec3> getNormals() const;
    std::vector<glm::vec2> getTextureCoordinates() const;

    int getLodsCount() const;
};
