#ifndef OBJECT_H
#define OBJECT_H

#include "entity.h"

// Object is the special case of entity, which has additional methods and description.

// This class is base class for all types of objects.
// It will be used to store all the common stuff.
// And to describe static objects, that have no special behaviour.
// Dynamic objects, that react on actions, and have different behavior are special cases of {Objects}
class Object : virtual public Entity
{

public:
    enum class ObjectSType {WALL, ROCK};
    enum class ObjectDType {DOOR, CHEST};
    enum class Orientation {DEFAULT, VERTICAL, HORIZONTAL};
    Object(const ObjectSType& type, const Orientation& orientation, QGraphicsItem* parent = nullptr);
    Object(const ObjectDType& type, const Orientation& orientation, QGraphicsItem* parent = nullptr);
    ~Object();

    const ObjectSType&  objectType () const;
    const Orientation& orientation() const;

    virtual void activate() = 0;

private:
    // Descriptions, that are relevant just for objects.
    virtual void generate (const ObjectSType& type, const Orientation& orientation);

    ObjectSType  m_objectType;
    Orientation m_orientation;

};

#endif // OBJECT_H
