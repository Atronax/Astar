#include "chest.h"

Chest::Chest(const Orientation& orientation, QGraphicsItem* parent)
    : Object(ObjectDType::CHEST, orientation, parent)
{

}

void Chest::activate()
{
    if (!isOpened())
        open();
    else
        close();
}

void Chest::open()
{
    // change graphics
    // load chest subsystem

    setOpened(true);
}

void Chest::close()
{
    // check if the button in chest subsystem clicked
    // if so, do lower code
    // change graphics

    setOpened(false);
}
