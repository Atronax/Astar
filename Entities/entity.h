#ifndef ENTITY_H
#define ENTITY_H

#include <QGraphicsRectItem>

#include "../spritesheet.h"

// Base class for all the entities.
// Usually, it is used for collision detection when operating with all types of entities in a general way.
// Entity *entity = new Player ("A", "B", "C");
// In this case the {Player}   is the special case of {Creature} with additional descripitions, behaviour and interactiveness.
//              the {Creature} is the subclass     of {Entity}.
// When having deals with collision detection, we usually find some collided {Entity} and need to check its real type
// If some {Entity} is of type {Player}, we just use the {dynamic_cast<Player*>} and check, if the pointer still exists.
// Player *player = dynamic_cast<Player*>(entity);
// if (player != nullptr) do some stuff with player.
// That is standard scheme to operate on all kinds of entities. Subclasses, whatsoever.
class Entity : public QGraphicsRectItem
{
public:
    Entity(QGraphicsItem* parent = nullptr);
    virtual ~Entity();

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) override;
    virtual QRectF boundingRect() const override;
    virtual QPainterPath  shape() const override;

    void setSpritesheet (Spritesheet* spritesheet);
    void setSprite      (const QImage& image);
    void setName        (const QString& name);
    void setDescription (const QString& description);

    QString name()        const;
    QString description() const;

private:
    Spritesheet *m_spritesheet;
    QImage  m_sprite;
    QString m_name;
    QString m_description;

};

#endif // ENTITY_H
