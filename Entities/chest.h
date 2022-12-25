#ifndef CHEST_H
#define CHEST_H

#include "object.h"
#include "Interfaces/iopenable.h"

class Chest : virtual public Object, public iOpenable
{
public:
    Chest(const Orientation& orientation, QGraphicsItem* parent);
    ~Chest();

    virtual void activate() override;
    virtual void open()  override;
    virtual void close() override;

};

#endif // CHEST_H
