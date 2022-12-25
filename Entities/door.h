#ifndef DOOR_H
#define DOOR_H

#include "object.h"
#include "interfaces/iopenable.h"
#include "interfaces/idestroyable.h"

class Door : virtual public Object, public iOpenable, public iDestroyable
{
public:
    Door(const Orientation& orientation, QGraphicsItem* parent);
    ~Door();

    virtual void activate() override;

    virtual void open()    override;
    virtual void close()   override;
    virtual void destroy() override;
};

#endif // DOOR_H
