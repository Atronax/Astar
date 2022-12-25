#include "mapview.h"

#include <QKeyEvent>
#include <QMouseEvent>
#include <QDebug>

MapView::MapView(int width, int height)
    : QGraphicsView()
{
    m_width = width;
    m_height = height;

    prepareScene();
    prepareMap();
}

MapView::~MapView()
{
     clearCanvas();
}

void MapView::keyPressEvent(QKeyEvent *event)
{
    // for example move (Direction::UP);
    if (event->key() == Qt::Key_W)
    {
    }

    if (event->key() == Qt::Key_A)
    {
    }

    if (event->key() == Qt::Key_S)
    {
    }

    if (event->key() == Qt::Key_D)
    {
    }
}

void MapView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        // take {current  position} of active unit
        // find {selected position} using view and mouse position
        // take the nodes of the tiles beneath them
        // now, that we have two points and {nodes} (if they fall for constraints)
        //      build the spt between them, and use that to find shortest path

        QList<QGraphicsItem*> selectedItems = items(event->pos());
        if (selectedItems.isEmpty())
            return;

        QGraphicsItem* itemOnTop = selectedItems.first();
        Tile* tile = dynamic_cast<Tile*>(itemOnTop);
        if (tile)
        {
            addPathPoint(tile);

            tile->setState(Tile::State::ACTIVE);
        }
    }
}

void MapView::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    clearSelection(Selection::HOVERED);

    QList<QGraphicsItem*> hoveredItems = items(event->pos());
    if (hoveredItems.isEmpty())
        return;

    QGraphicsItem* itemOnTop = hoveredItems.first();
    Tile* tile = dynamic_cast<Tile*>(itemOnTop);
    if (tile)
    {
        if (!tile->isActive())
            tile->setState(Tile::State::HOVERED);
    }

    qDebug() << "hovered items: " << hoveredItems.size();
}

void MapView::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void MapView::prepareScene()
{
    // Prepare scene
    m_scene = new QGraphicsScene (this);

    // Prepare view
    setScene(m_scene);
    setSceneRect(0, 0, m_width*CELLSIZE, m_height*CELLSIZE);
    setMouseTracking(true);

    // Prepare background objects
    makeGrid(TileType::SQUARE);
}

void MapView::prepareMap()
{
    m_tiles = new QMap<Node, Tile*>();
}

void MapView::clearCanvas()
{
    m_scene->deleteLater();

    deleteLater();
}

void MapView::makeGrid(const TileType& tileType)
{
    QPen stroke (QBrush(Qt::yellow), 2, Qt::DashLine);

    switch (tileType)
    {
        case TileType::SQUARE:
        {
            // make horizontal lines
            for (int y = 0; y < HEIGHT; y+= CELLSIZE)
                m_scene->addItem(new QGraphicsLineItem(QLineF(0,y,WIDTH,y)));

            // make vertical lines
            for (int x = 0; x < WIDTH; x+= CELLSIZE)
                m_scene->addItem(new QGraphicsLineItem(QLineF(x, 0, x, HEIGHT)));

        }
        break;

        case TileType::HEX:
        break;
    }
}

Tile::TileType MapView::symbolToType(const QString &symbolicMap, int xPosition, int yPosition)
{
    Tile::TileType result = Tile::TileType::NOTHING;

    int index = yPosition*m_width + xPosition;
    QChar symbol = symbolicMap.at(index).toLower();

    // qDebug() << xPosition << "," << yPosition << " - " << index << ":" << symbol;

    if (symbol == 'w')
        result = Tile::TileType::WATER;

    if (symbol == 'r')
        result = Tile::TileType::ROAD;

    if (symbol == 'h')
        result = Tile::TileType::HILL;

    if (symbol == 'f')
        result = Tile::TileType::FOREST;

    if (symbol == 'm')
        result = Tile::TileType::MOUNTAIN;

    return result;
}

Tile *MapView::tileAt(const Node &node)
{
    for (int i = 0; i < m_tiles->size(); ++i)
    {
        Node  currentNode = m_tiles->keys().at(i);
        Tile* currentTile = m_tiles->values().at(i);

        if (currentNode == node) // .x() == node.x() && currentNode.y() == node.y())
        {
            qDebug() << QString("Shortest path. Found tile %1 at node %2.").arg(currentTile->toString()).arg(currentNode.toString());
            return currentTile;
        }
    }

    return nullptr;

    return nullptr;
}

void MapView::clearSelection(const Selection& what)
{
    for (int i = 0; i < m_tiles->values().size(); ++i)
    {
        Tile* tile = m_tiles->values().at(i);


        switch(what)
        {
            case Selection::IDLE:
            break;

            case Selection::ACTIVE:
            break;

            case Selection::HOVERED:
            if (tile->isHovered())
                tile->setState(Tile::State::IDLE);
            break;

            case Selection::EVERYTHING:
            tile->setState(Tile::State::IDLE);
            break;
        }
    }
}

void MapView::resize(int width, int height)
{
    // if width is allowed and height is allowed, resize
    if (width > 0 && height > 0)
    {
        m_width = width;
        m_height = height;
    }
}

void MapView::buildMap(const QString &symbolicMap, const QString& weightMap)
{
    clearMap();

    qDebug() << "Building the map";
    qDebug() << "Symbolic map: \n " << symbolicMap;

    for (int y = 0; y < m_height; ++y)
    {
        for (int x = 0; x < m_width; ++x)
        {
            Tile* tile = addTileAt(QPoint(x,y), symbolToType(symbolicMap, x, y));

            int index = y*m_width + x;
            int movementCost = weightMap.at(index).digitValue();
            tile->setMovementCost(movementCost);
        }
    }

    qDebug() << QString("%1 tiles has been created.").arg(m_tiles->size());
    qDebug() << "Map has been composed";
}

void MapView::clearMap()
{
    // Do nothing, if there isn't any tiles yet.
    if (m_tiles == nullptr || m_tiles->isEmpty())
        return;

    // Find and remove all the tiles from the list and scene
    // Then delete them. Also, clear the list.
    for (int i = 0; i < m_tiles->size(); ++i)
    {
        Node  node = m_tiles->keys().at(i);
        Tile* tile = m_tiles->values().at(i);

        m_tiles->remove(node);
        m_scene->removeItem(tile);

        delete tile;
        tile = nullptr;
    }

    m_tiles->clear();
}

void MapView::deleteMap()
{
    delete m_tiles;
    m_tiles = nullptr;
}

Tile* MapView::addTileAt(const QPoint &position, const Tile::TileType &type)
{
    Node node (position.x(), position.y());
    Tile *tile =  new Tile(type);
    tile->setRect(position.x() * CELLSIZE, position.y() * CELLSIZE, CELLSIZE, CELLSIZE);    

    m_scene->addItem(tile);
    m_scene->update();

    qDebug() << QString("Tile of type %1 was set at position {%2,%3}.").arg(tile->toString()).arg(position.x()).arg(position.y());
    m_tiles->insert(node, tile);
    return tile;
}

Node MapView::findNode(Tile *tile)
{
    Node node;

    for (int i = 0; i < m_tiles->size(); ++i)
    {
        if (tile == m_tiles->values().at(i))
            node =  m_tiles->keys().at(i);
    }

    return node;
}

void MapView::generatePath()
{
    // There is no start point or end point. Do nothing in this case.
    if (m_pathPoints.first == nullptr || m_pathPoints.second == nullptr)
        return;

    Node start = findNode(m_pathPoints.first);
    Node end   = findNode(m_pathPoints.second);

    // Send these to board instance, so that it would be able to call relevant blackbox methods of model, that operates on graph
    if (!start.isDefault() && !end.isDefault())
        emit findPath(start, end);
}

void MapView::addPathPoint(Tile *tile)
{
    // If there is  no {from} tile, add it to path points. Clear previous selection, if needed.
    // If there is  no {to  } tile, add it to path points.
    // If there are both {from} and {to} tiles, generate path between them and nullify the pointers.
    if (!m_pathPoints.first)
    {
        clearSelection(Selection::EVERYTHING);
        m_pathPoints.first = tile;
    }
    else if (!m_pathPoints.second)
        m_pathPoints.second = tile;

    if (m_pathPoints.first && m_pathPoints.second)
    {
        qDebug() << "Both points are selected. Trying to find the shortest path tree.";
        if (m_pathPoints.first->isTracable() && m_pathPoints.second->isTracable())
            generatePath();

        // Clear selection
        m_pathPoints.first = nullptr;
        m_pathPoints.second = nullptr;
    }
}

void MapView::fillWithRandomTiles()
{
    for (int x = 0; x < m_width; ++x)
    {
        for (int y = 0; y < m_height; ++y)
        {
            // generate tile, add it to the list and to scene
            // - randomly
            // - or using XML map

           int type = rand()%5;
           addTileAt(QPoint(x,y), static_cast<Tile::TileType>(type));
        }
    }
}

void MapView::onFoundPath(const QVector<Node> &path)
{
    clearSelection(Selection::EVERYTHING);

    qDebug() << "Shortest path. Path size: " << path.size();

    for (int i = 0; i < path.size(); ++i)
    {
        Node  node = path.at(i);
        Tile* tile = tileAt(node);

        tile->setState(Tile::State::ACTIVE);
    }
}
