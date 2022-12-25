#ifndef CREATURE_H
#define CREATURE_H

#include "entity.h"

// Creature is the special case of entity. Just like {Object} it could be final class or
//          base class for various types of creatures with unique behaviour and\or description.

class Creature : public Entity
{
public:
    enum class CreatureType {A, B, C, D};
    Creature(QGraphicsItem* parent = nullptr);
    ~Creature();

private:
    // Descriptions, that are relevant just for creatures.
};

#endif // CREATURE_H
