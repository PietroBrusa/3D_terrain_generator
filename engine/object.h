/**
 * @file	object.h
 * @brief	Object class header file
 *
 * @author	Giona Valsecchi (C) SUPSI [giona.valsecchi@supsi.ch] , Pietro Brusadelli (C) SUPSI [pietro.brusadelli@supsi.ch], Filippo De Simoni (C) SUPSI [filippo.desimoni@supsi.ch]
 */

#pragma once

///////////////////////
// OBJECT CLASS //
///////////////////////

/**
 * @brief Abstract Base class for all entities in the engine.
 *
 * The Object class serves as the root for all drawable and scene-managed entities
 * (like Meshes, Cameras, Lights, and Nodes). It enforces a unique ID system
 * and requires all derived classes to implement a \c render method.
 */
class ENG_API Object
{
private:
    /** @brief Static counter used to assign unique IDs to new Object instances. */
    static unsigned int nextUniqueId;

    /** @brief The unique identifier assigned to this specific object instance. */
    unsigned int uniqueId;

    /** @brief A user-friendly name assigned to this object for identification in the scene. */
    std::string name;

public:
    /**
     * @brief Constructor for the Object base class.
     *
     * Assigns a unique ID and optionally sets a name.
     * @param name The initial name of the object (default is an empty string).
     */
    Object(const std::string &name = "");

    /**
     * @brief Virtual destructor for the Object base class.
     *
     * Ensures proper cleanup for derived classes.
     */
    virtual ~Object();

    /**
     * @brief The pure virtual method for rendering the object.
     *
     * All derived classes must implement their specific rendering logic here.
     * @param modelview The combined Model-View matrix used to transform the object into view space.
     * Defaults to the identity matrix.
     */
    virtual void render(glm::mat4 modelview = glm::mat4(1.0f)) = 0;

    /////////////
    // Getters //
    /////////////

    /**
     * @brief Gets the unique identifier of the object.
     * @return The unique unsigned integer ID.
     */
    unsigned int getId() const;

    /**
     * @brief Gets the name of the object.
     * @return A constant reference to the object's name string.
     */
    const std::string& getName() const;

    ////////////
    // Setter //
    ////////////

    /**
     * @brief Sets a new name for the object.
     * @param newName The new string name for the object.
     */
    void setName(const std::string& newName);
};
