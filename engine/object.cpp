/**
 * @file    object.cpp
 * @brief   Object class implementation
 *
 * @author  Giona Valsecchi (C) SUPSI [giona.valsecchi@supsi.ch] , Pietro Brusadelli (C) SUPSI [pietro.brusadelli@supsi.ch], Filippo De Simoni (C) SUPSI [filippo.desimoni@supsi.ch]
 */

#include "engine.h"

unsigned int Eng::Object::nextUniqueId = 1;

namespace Eng
{

    ///////////////////////
    // OBJECT CLASS //
    ///////////////////////

    /**
     * @brief Constructs a new Object.
     * * The UID is automatically assigned from the static counter.
     * @param name The descriptive name for the object.
     */
    Object::Object(const std::string &name) : uniqueId{nextUniqueId++}, name{name}
    {
    }

    Object::~Object()
    {
    }

    unsigned int Object::getId() const
    {
        return uniqueId;
    }

    const std::string& Object::getName() const
    {
        return name;
    }

    void Object::setName(const std::string& newName)
    {
        name = newName;
    }

}; // end of namespace Eng::
