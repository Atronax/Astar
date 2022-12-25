#include "door.h"

Door::Door(const Orientation& orientation, QGraphicsItem* parent)
    : Object(ObjectDType::DOOR, orientation, parent)
{
    setOpened(false);
}

void Door::activate()
{
    if (isOpened())
        close();
    else
        open();
}

void Door::open()
{
    // These are visual representation of opened doors.
    switch(orientation())
    {
        case Orientation::HORIZONTAL:
        setSprite(QImage("doorH_opened.png"));
        break;

        case Orientation::VERTICAL:
        setSprite(QImage("doorV_opened.png"));
        break;

        case Orientation::DEFAULT:
        break;
    }

    // This boolean value will be used to indicate whether the door is opened or closed.
    // It will be used later to allow or deny movement. This should be an additional constraint for pathfinding subsystem.
    setOpened(true);
}

void Door::close()
{
    switch(orientation())
    {
        case Orientation::HORIZONTAL:
        setSprite(QImage("doorH_closed.png"));
        break;

        case Orientation::VERTICAL:
        setSprite(QImage("doorV_closed.png"));
        break;

        case Orientation::DEFAULT:
        break;
    }

    setOpened(false);
}

void Door::destroy()
{
    setOpened(true);
    delete this;
}
