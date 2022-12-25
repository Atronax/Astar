#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QGraphicsScene>
#include <QGraphicsView>

#include "Graph/node.h"

#include "tile.h"
#include "Entities/object.h"
#include "Entities/creature.h"
#include "Entities/item.h"

class MapView : public QGraphicsView
{
    Q_OBJECT

public:
    enum class TileType  {SQUARE, HEX};
    enum class Selection {IDLE, ACTIVE, HOVERED, EVERYTHING};
    MapView(int width = 0, int height = 0);
    ~MapView();

    // Controls
    void keyPressEvent     (QKeyEvent   *event) override;
    void mousePressEvent   (QMouseEvent *event) override;
    void mouseMoveEvent    (QMouseEvent *event) override;
    void mouseReleaseEvent (QMouseEvent *event) override;

    void resize(int width, int height);

    void buildMap (const QString& symbolicMap, const QString& weightMap);
    void clearMap ();
    void deleteMap ();

private:
    void prepareScene();
    void prepareMap();
    void clearCanvas();

    // Background markup.
    void makeGrid(const TileType& tileType);
    void fillWithRandomTiles();

    // Operate on tiles
    void clearSelection(const Selection& what);
    Tile::TileType symbolToType (const QString& symbolicMap, int xPosition, int yPosition);

    Tile*    tileAt(const Node& node);
    Tile* addTileAt(const QPoint& position, const Tile::TileType& type);
    void  remTileAt(const QPoint& position);

    Node findNode(Tile* tile);

    // Pathfinding.
    void generatePath();
    void addPathPoint(Tile* tile);
    QPair<Tile*, Tile*> m_pathPoints;

    // Canvas.
    QGraphicsScene* m_scene;
    QGraphicsView*  m_view = this;

    // Map characteristics.
    int m_width;
    int m_height;

    // 2D Tile-based map.
    QMap<Node, Tile*>     *m_tiles     = nullptr;
    QMap<Node, Object*>   *m_objects   = nullptr;
    QMap<Node, Creature*> *m_creatures = nullptr;
    QMap<Node, Item*>     *m_items     = nullptr;

    // Various relevant&&irrelevant data
    const int WIDTH = 400;
    const int HEIGHT = 400;
    const int CELLSIZE = 80;

signals:
    void findPath(const Node& start, const Node& end);

public slots:
    void onFoundPath(const QVector<Node>& path);
};

#endif // MAPVIEW_H
