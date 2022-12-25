#include "object.h"

Object::Object(const ObjectSType& type, const Orientation& orientation, QGraphicsItem *parent)
    : Entity(parent)
{
    generate(type, orientation);
}

Object::Object(const Object::ObjectDType &type, const Object::Orientation &orientation, QGraphicsItem *parent)
    : Entity(parent)
{
    Q_UNUSED (type);
    Q_UNUSED (orientation);

    // generate(type, orientation);
}

Object::~Object()
{

}

const Object::ObjectSType &Object::objectType() const
{
    return m_objectType;
}

const Object::Orientation &Object::orientation() const
{
    return m_orientation;
}

void Object::generate(const ObjectSType& type, const Orientation& orientation)
{
    // Fill basic things for objects:
    // - their visuals (static or dynamic, if needed)

    m_objectType = type;
    m_orientation = orientation;

    switch(type)
    {
        case ObjectSType::WALL:
        // Since walls can be horizontal and vertical,
        // And those has different visual look,
        // We'll add {Orientation} attribute to cover these cases.
        switch(orientation)
        {
            case Orientation::HORIZONTAL:
            // Since {Object} is a subclass of {Entity},
            // We can use its methods here freely.
            setName("Wall");
            setDescription("It is a wall. Hmm.. Looks like horizontal one.");

            // Hmm.. Wall, animated at 60 frames per second. Cool.
            setSpritesheet(new Spritesheet("D:/wallH_animated.png", 100, 100, 60));
            break;

            case Orientation::VERTICAL:
            setName("Wall");
            setDescription("It is a wall. No matter, if it is of horizontal or vertical orientation.");
            setSprite(QImage("D:/wallV.png"));
            break;

            case Orientation::DEFAULT:
            break;
        }
        break;

        case ObjectSType::ROCK:
        break;
    }
}
