#ifndef TILE_H
#define TILE_H

#include <QGraphicsRectItem>
#include <QGradient>
#include <QPen>

#include "spritesheet.h"

class Tile : public QObject, public QGraphicsRectItem
{    
    Q_OBJECT

public:
    enum class State    {ACTIVE, HOVERED, IDLE};
    enum class TileType {WATER, ROAD, FOREST, HILL, MOUNTAIN, NOTHING};
    Tile(const TileType& type, QGraphicsItem* parent = nullptr);
    Tile(const Tile& rhs);
    ~Tile();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) override;
    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    void setState        (const State& state);
    void setMovementCost (int movementCost);
    void setTracability  (bool tracability);

    bool isIdle();
    bool isActive();
    bool isHovered();

    TileType tileType() const;
    bool  isTracable()  const;
    int movementCost()  const;
    QString toString();


    inline void operator= (const Tile& rhs)
    {
        setTracability (rhs.isTracable());
        setMovementCost(rhs.movementCost());
    }

private:
    void prepareAnimation();
    void generate(const TileType& type);

    // Visuals
    Spritesheet *m_spritesheet;
    State    m_state;
    TileType m_type;
    QPen     m_borderPen;    
    QPen     m_textPen;

    // Movement
    bool m_isTracable;
    int  m_movementCost;

    // Animation
    QTimer m_animationTimer;
    bool   m_reverse;
    int    m_penWidth;

    QColor interpolate (const QColor& first, const QColor& second, float percentage);
    QGradient m_gradient;
    float     m_step;

public slots:
    void onAnimationTick();
};

#endif // TILE_H
