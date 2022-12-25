#include "entity.h"

#include <QPainter>

Entity::Entity(QGraphicsItem *parent)
    : QGraphicsRectItem(parent)
{
    // Create and adjust all elements, that are common for all types of entities.
}

Entity::~Entity()
{
    // Destroy all elements, that are common for all types of entities.
}

void Entity::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    // Default render for all types of entities.
    // If there are any changes, those should be described in overrides.

    if (m_spritesheet)
        painter->drawImage(boundingRect(), m_spritesheet->currentFrame());
    else if (m_sprite.isNull())
        painter->drawImage(boundingRect(), m_sprite);
    else
        painter->drawRect(boundingRect().adjusted(10.0f, 10.0f, -10.0f, -10.0f));
}

QRectF Entity::boundingRect() const
{
    // Both of these should be used for collision detection.
    return rect();
}

QPainterPath Entity::shape() const
{
    QPainterPath shape;
    shape.addRect(boundingRect());

    return shape;
}

void Entity::setSpritesheet(Spritesheet *spritesheet)
{
    if (m_spritesheet)
        delete m_spritesheet;

    m_spritesheet = spritesheet;
}

void Entity::setSprite(const QImage &image)
{
    // Default test function. Every entity has a visual representation. We'll use sprite for this purpose.
    m_sprite = image;
}

void Entity::setName(const QString &name)
{
    // Check, if sent {name} is an actual name and set it as current one.

    Q_ASSERT(!name.isEmpty());
    Q_ASSERT(name.size() > 4);

    m_name = name;
}

void Entity::setDescription(const QString &description)
{
    Q_ASSERT(!m_description.isEmpty());
    Q_ASSERT(m_description.size() > 10);

    m_description = description;
}

QString Entity::name() const
{
    return m_name;
}

QString Entity::description() const
{
    return m_description;
}
