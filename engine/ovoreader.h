/**
 * @file    ovoreader.h
 * @brief   OvoReader class
 *
 * @author  Giona Valsecchi (C) SUPSI [giona.valsecchi@supsi.ch] , Pietro Brusadelli (C) SUPSI [pietro.brusadelli@supsi.ch], Filippo De Simoni (C) SUPSI [filippo.desimoni@supsi.ch]
 */

#pragma once

/**
 * @brief Utility macro to print a \c glm::mat4 to the standard output in a readable format.
 * @param m The \c glm::mat4 matrix to be printed.
 */
#define MAT2STR(m) std::cout << "Matrix  . . . :  \t" << m[0][0] << "\t" << m[1][0] << "\t" << m[2][0] << "\t" << m[3][0] << std::endl \
                           << "                    \t" << m[0][1] << "\t" << m[1][1] << "\t" << m[2][1] << "\t" << m[3][1] << std::endl \
                           << "                    \t" << m[0][2] << "\t" << m[1][2] << "\t" << m[2][2] << "\t" << m[3][2] << std::endl \
                           << "                    \t" << m[0][3] << "\t" << m[1][3] << "\t" << m[2][3] << "\t" << m[3][3] << std::endl 

/** @brief Standard maximum length for a filename path (e.g., 260 for Windows). */
#define FILENAME_MAX 260

/**
 * @brief Handles the loading and parsing of the engine's custom scene file format (OVO).
 *
 * This class is responsible for opening the file, reading serialized data (chunks),
 * deserializing the data into engine objects (\c Node, \c Mesh, \c Light), and building
 * the scene graph.
 */
class ENG_API OvoReader final
{
public:
    /** @brief Default constructor. */
	OvoReader();

    /** @brief Destructor. Responsible for closing the file handle if open. */
	~OvoReader();

    /**
     * @brief Loads a scene file and constructs the scene graph.
     *
     * This is the main entry point for scene loading.
     * @param filename The full path to the .ovo scene file.
     * @return A pointer to the root \c Eng::Node of the loaded scene graph, or \c nullptr on failure.
     */

	Eng::Node* load(const std::string& filename);

private:
    /** @brief File pointer used for reading the OVO file. */
	FILE* file;

	/** @brief The base directory path of the loaded file, used to resolve relative paths for assets (textures). */
	std::string basePath;

	/** @brief Cache for loaded materials to prevent duplicate loading and manage references. */
	std::map<std::string, Eng::Material*> materials;

	/** @brief Cache for loaded textures to prevent duplicate loading and manage references. */
	std::map<std::string, Eng::Texture*> textures;

	/**
	 * @brief Enumeration of supported object types found within the OVO file format.
	 *
	 * These integer values are used as type identifiers in the serialized data.
	 */
	enum class Type : int
	{
		OBJECT = 0,		///< Base object type.
		NODE = 1,		///< Scene graph node.
		TEXTURE = 7,	///< Texture asset.
		MATERIAL = 9,	///< Material definition.
		LIGHT = 16,		///< Light source.
		MESH = 18,		///< Geometric mesh data.
	};

	/**
	 * @brief Enumeration of specific light subtypes supported by the OVO format.
	 */
	enum class LightSubtype : int
	{
		OMNI = 0,		///< Point light.
		DIRECTIONAL,	///< Infinite/directional light.
		SPOT,			///< Spotlight.
	};

    /**
     * @brief Recursively loads the next node in the scene graph from the file stream.
     * @return A pointer to the loaded \c Eng::Node, or \c nullptr if loading is complete or failed.
     * @private
     */
	Eng::Node* loadNextNode();

    /**
     * @brief Processes a generic object chunk (currently unused, but represents the base object properties).
     * @param data Pointer to the chunk data buffer.
     * @param size Reference to the size of the chunk data.
     * @private
     */
	void processObjectChunk(char* data, unsigned int& size);

    /**
     * @brief Processes a texture chunk, loads the asset, and returns the created \c Eng::Texture object.
     * @param data Pointer to the chunk data buffer.
     * @param size Reference to the size of the chunk data.
     * @return A pointer to the newly created \c Eng::Texture object.
     * @private
     */
	Eng::Texture* processTextureChunk(char* data, unsigned int& size);

    /**
     * @brief Processes a material chunk and creates a \c Eng::Material object, caching it in the \c materials map.
     * @param data Pointer to the chunk data buffer.
     * @param size Reference to the size of the chunk data.
     * @private
     */
	void processMaterialChunk(char* data, unsigned int& size);

    /**
     * @brief Processes a general node chunk and constructs the appropriate \c Eng::Node derived object.
     * @param data Pointer to the chunk data buffer.
     * @param size Reference to the size of the chunk data.
     * @param type The specific \c Type of node to be created (e.g., \c NODE, \c MESH, \c LIGHT).
     * @return A pointer to the created \c Eng::Node or derived object.
     * @private
     */
	Eng::Node* processNodeChunk(char* data, unsigned int& size, Type type);
	
    /**
     * @brief Processes a light chunk, extracts light-specific properties, and creates the appropriate \c Eng::Light derived object.
     * @param data Pointer to the chunk data buffer.
     * @param size Reference to the size of the chunk data.
     * @param name Name of the light object.
     * @param matrix Local transformation matrix of the light.
     * @return A pointer to the created \c Eng::Light derived object.
     * @private
     */
    Eng::Node* processLightChunk(char* data, unsigned int& size, char* name, glm::mat4& matrix);
	
    /**
     * @brief Processes a mesh chunk, extracts geometry and material references, and creates an \c Eng::Mesh object.
     * @param data Pointer to the chunk data buffer.
     * @param size Reference to the size of the chunk data.
     * @param name Name of the mesh object.
     * @param matrix Local transformation matrix of the mesh.
     * @return A pointer to the created \c Eng::Mesh object.
     * @private
     */
    Eng::Node* processMeshChunk(char* data, unsigned int& size, char* name, glm::mat4& matrix);
};
