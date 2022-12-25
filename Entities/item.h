#ifndef ITEM_H
#define ITEM_H

#include "entity.h"

// Item is a subclass of entity. It has all the stuff from base class, some of them overriden and some not +
//      some additional descriptions and behaviour, that is relevant just for items

class Item : public Entity
{
public:
    enum class ItemType {A, B, C};
    Item(QGraphicsItem* parent);
    ~Item();

private:
};

#endif // ITEM_H
