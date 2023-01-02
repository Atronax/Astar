#include "tile.h"

#include <QDebug>
#include <QPainter>

Tile::Tile(const Tile::TileType &type, QGraphicsItem* parent)
    : QGraphicsRectItem(parent)
{
    generate(type);
    prepareAnimation();
}

Tile::Tile(const Tile &rhs)
    : QGraphicsRectItem(rhs.parentItem())
{
    generate(rhs.tileType());
    prepareAnimation();
}

Tile::~Tile()
{

}

void Tile::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    // If spritesheet is used to draw tile, select current frame and draw it here.
    // Otherwise, simply fill the background with dark gray color.

    // Something in spritesheet causes out of bound list
    if (m_spritesheet && !m_spritesheet->empty())
       painter->drawImage(boundingRect(), m_spritesheet->currentFrame());
    else
       painter->fillRect (boundingRect(), QColor("#111"));

    // Draw bounding rectange.
    painter->setPen(m_borderPen);
         painter->drawRect(boundingRect());

    // Draw text.
    painter->setPen(m_textPen);
        painter->drawText(boundingRect(), toString(), QTextOption(Qt::AlignHCenter));
        painter->drawText(boundingRect(), QString::number(m_movementCost), QTextOption(Qt::AlignRight | Qt::AlignBottom));
}

QRectF Tile::boundingRect() const
{
    return rect();
}

QPainterPath Tile::shape() const
{
    QPainterPath shape;
    shape.addRect(rect());

    return shape;
}

void Tile::setState(const Tile::State &state)
{
    // Do nothing, if the state hasn't changed since last action.
    if (m_state == state)
        return;

    // Otherwise, change state of the tile and do some relevant stuff (change visuals, sprite, animation etc.)
    // This is done for tiles, but easily can be extrapolated on any object on top of it.
    m_state = state;
    switch (m_state)
    {
        case State::ACTIVE:
        m_borderPen = QPen(QColor("#fab739"), 4);
        break;

        case State::HOVERED:        
        m_borderPen = QPen(Qt::white, 2);
        m_textPen   = QPen(QColor("#fab739"), 2);
        break;

        case State::IDLE:
        m_borderPen = QPen(Qt::black, 1);
        m_textPen   = QPen(Qt::white, 1);
        break;
    }

    update();
}

void Tile::setMovementCost(int movementCost)
{
    if (movementCost == -1 || (movementCost > 0 && movementCost < 10))
        m_movementCost = movementCost;
}

void Tile::setTracability(bool isTracable)
{
    m_isTracable = isTracable;
}

bool Tile::isIdle()
{
    return (m_state == State::IDLE);
}

bool Tile::isActive()
{
    return (m_state == State::ACTIVE);
}

bool Tile::isHovered()
{
    return (m_state == State::HOVERED);
}

Tile::TileType Tile::tileType() const
{
    return m_type;
}

int Tile::movementCost() const
{
    return m_movementCost;
}

void Tile::prepareAnimation()
{
    m_animationTimer.stop();

    int fps = 30;
    m_penWidth = 1;
    m_reverse  = false;

    m_gradient = QGradient();
    m_gradient.setColorAt(0.0f, QColor("#eb6734"));
    m_gradient.setColorAt(1.0f, QColor("#ebb434"));

    connect (&m_animationTimer, SIGNAL(timeout()), this, SLOT(onAnimationTick()));
    m_animationTimer.start(1000/fps);
}

bool Tile::isTracable() const
{
    return m_isTracable;
}

void Tile::generate(const Tile::TileType &type)
{
    // Using spritesheet class, we can use both static or dynamic images and draw them in paint method.
    // For this case we'll use test spritesheet, that has 4-framed per second animation.
    // Here we can also place other tile relevant stuff like:
    // - movement cost, that can be read from {weightTable};
    // - constraints, that have the influence on top objects.
    // These constraints gathered together form the rules.

    // Default values for movement cost are placed here.
    // If they needed to be loaded from XML map, they can be replaced using special helper method.

    m_spritesheet = nullptr;
    m_type = type;

    switch (type)
    {
        case TileType::NOTHING:
        qDebug() << "Nothing was generated.";
        break;

        case TileType::WATER:
        // m_spritesheet = new Spritesheet("test.png", 100, 100, 4);
        m_movementCost = -1;
        m_isTracable = false;
        break;

        case TileType::HILL:
        m_spritesheet = new Spritesheet("c:/grass.png", 100, 100, 4, this);
        m_movementCost = 3;
        m_isTracable = true;
        break;

        case TileType::ROAD:
        // m_spritesheet = new Spritesheet("test.png", 100, 100, 4);
        m_movementCost = 1;
        m_isTracable = true;
        break;

        case TileType::FOREST:
        // m_spritesheet = new Spritesheet("test.png", 100, 100, 4);
        m_movementCost = 2;
        m_isTracable = true;
        break;

        case TileType::MOUNTAIN:
        // m_spritesheet = new Spritesheet("test.png", 100, 100, 4);
        m_movementCost = 4;
        m_isTracable = true;
        break;
    }

    setState(State::IDLE);
}

QColor Tile::interpolate(const QColor &first, const QColor &second, float percentage)
{
    int f_hue = first.hue();
    int s_hue = second.hue();
    int  dist = s_hue - f_hue;

    int r_hue = f_hue + dist*percentage;

    QColor result = first;
    result.setHsv(r_hue, first.saturation(), first.value());
    return result;
}

QString Tile::toString()
{
    QString result;

    switch(m_type)
    {
        case TileType::NOTHING:
            // qDebug() << "nothing";
            result = "n";
        break;

        case TileType::WATER:
            // qDebug() << "water";
            result = "w";
        break;

        case TileType::ROAD:
            // qDebug() << "road";
            result = "r";
        break;

        case TileType::HILL:
            // qDebug() << "hill";
            result = "h";
        break;

        case TileType::FOREST:
            // qDebug() << "forest";
            result = "f";
        break;

        case TileType::MOUNTAIN:
            // qDebug() << "mountain";
            result = "m";
        break;
    }

    return result;
}

void Tile::onAnimationTick()
{
    switch(m_state)
    {
        case State::IDLE:
        // Animation for idle state.
        // These are separated from spritesheet animations and can be used to animate tile components,
        // Which we draw on top of the spritesheet itself.
        break;

        case State::ACTIVE:
        // Animation for active state.
        if (!m_reverse)
        {
            m_step += 0.01f;           
            if (m_step == 1.0f)
                m_reverse = true;
        }
        else
        {
            m_step -= 0.01f;
            m_penWidth -= 0.03f;
            if (m_step == 0.0f)
                m_reverse = false;
        }

        m_borderPen.setColor(interpolate("#5df54d", "#6cd6fd", m_step));       
        update();
        break;

        case State::HOVERED:
        // Animation for hovered state.
        break;
    }
}
