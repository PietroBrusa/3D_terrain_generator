/**
 * @file    node.h
 * @brief   Scene graph node base class
 *
 * @author  Giona Valsecchi (C) SUPSI [giona.valsecchi@supsi.ch] , Pietro Brusadelli (C) SUPSI [pietro.brusadelli@supsi.ch], Filippo De Simoni (C) SUPSI [filippo.desimoni@supsi.ch]
 */

#pragma once

/**
 * @brief Represents a single element in the hierarchical Scene Graph.
 *
 * The Node class is the basic building block for organizing a 3D scene.
 * Each node holds a local transformation matrix and manages a list of child nodes.
 * The final world transformation is computed by concatenating the node's local
 * matrix with all its parent nodes' matrices.
 */
class ENG_API Node : public Eng::Object {
    /** @brief Pointer to the parent node in the scene graph. Null if this is the root node. */
    Eng::Node* m_parent;

    /** @brief A collection of child nodes, establishing the hierarchy. */
    std::vector<Eng::Node*> m_children;

    /** @brief Local transformation matrix (position, rotation, scale) relative to the parent node. */
    glm::mat4 m_matrix;

   public:
    /**
     * @brief Constructor for a scene Node.
     * @param name The name of the node (passed to the base Object class).
     * @param matrix The initial local transformation matrix (default is identity).
     */
    Node(const std::string& name = "", const glm::mat4& matrix = glm::mat4(1.0f));

    /**
     * @brief Destructor for the Node class.
     *
     * Responsibility for cleaning up child nodes should be defined here (e.g., recursive deletion).
     */
    virtual ~Node();

    /**
     * @brief Renders the node and recursively calls render on all its children.
     *
     * The modelview matrix is updated before rendering children to include this node's
     * world transformation.
     * @param modelview The Model-View matrix accumulated from parent nodes.
     */
    virtual void render(glm::mat4 modelview = glm::mat4(1.0f)) override;

    // -------------------------------------------------------------------------
    // SCENE GRAPH METHODS
    // -------------------------------------------------------------------------

    /**
     * @brief Retrieves the parent of this node.
     * @return A pointer to the parent Node, or \c nullptr if this is the root.
     */
    Eng::Node* getParent() const { return m_parent; }

    /**
     * @brief Sets a new parent for this node, adjusting the scene graph structure.
     * @param parent A pointer to the new parent node.
     */
    void setParent(Node* parent);

    /**
     * @brief Adds a child node to this node's collection.
     * @param child A pointer to the Node to be added.
     * @return \c true if the child was successfully added, \c false otherwise.
     */
    bool addChild(Node* child);

    /**
     * @brief Searches for a direct child node by its name.
     * @param name The name of the child node to find.
     * @return A pointer to the matching child Node, or \c nullptr if not found.
     */
    Eng::Node* getChild(std::string name) const;

    /**
     * @brief Retrieves a direct child node by its index in the children vector.
     * @param n The index of the child node.
     * @return A pointer to the child Node at the specified index, or \c nullptr if the index is out of bounds.
     */
    Eng::Node* getChild(unsigned int n) const;

    /**
     * @brief Gets a list of all direct children of this node.
     * @return A vector of pointers to all child nodes.
     */
    std::vector<Eng::Node*> getChildren() const;

    /**
     * @brief Gets the total number of direct child nodes.
     * @return The count of child nodes.
     */
    unsigned int getNumberOfChildren() const;

    /**
     * @brief Removes a child node at a specific index.
     * @param n The index of the child to remove.
     * @return \c true if the child was successfully removed, \c false otherwise (e.g., index out of bounds).
     */
    bool removeChild(unsigned int n);

    /**
     * @brief Removes a specific child node by pointer reference.
     * @param child A pointer to the child Node to remove.
     * @return \c true if the child was found and removed, \c false otherwise.
     */
    bool removeChild(Eng::Node* child);

    // -------------------------------------------------------------------------
    // TRANSFORMATION METHODS
    // -------------------------------------------------------------------------

    /**
     * @brief Gets the node's local transformation matrix.
     * @return The \c glm::mat4 representing the local transformation (relative to parent).
     */
    const glm::mat4 getMatrix() const;

    /**
     * @brief Sets the node's local transformation matrix.
     * @param matrix The new \c glm::mat4 to be used as the local transformation (default is identity).
     */
    void setMatrix(const glm::mat4& matrix = glm::mat4(1.0f));

    /**
     * @brief Calculates and retrieves the node's final world coordinate transformation matrix.
     *
     * This involves recursively concatenating the local matrix with all parent matrices up to the root.
     * @return The world transformation matrix (\c glm::mat4).
     */
    glm::mat4 getWorldCoordinateMatrix() const;
};