#ifndef MAPMODEL_H
#define MAPMODEL_H

#include "Path\grid.h"

// Map class represents the region, filled with cells.
// Each cell can be filled (tracable) or unfilled (untracable).
// This class allows easy detection of shortest path between any two cells
// depending on the weights these cells have.

class MapModel
{
public:
    MapModel(int width = 0 , int height = 0, const uint& cellsize = 50);
    ~MapModel();

    QVector<Node> nodes() const;
    QVector<Node> shortestPath(const Node& from, const Node& to) const;

    // Sizes of the map
    int width() const;
    int height() const;
    int cellsize() const;
    int cellsInRow() const;
    int cellsInColumn() const;

    void resizeMap  (int width, int height);
    void resizeCell (int size);

    // These operate on logic cell (1x1)
    void fillCell     (const QPoint& position);
    void unfillCell   (const QPoint& position);
    bool isFilledCell (const QPoint& position);

    // Set and check weights methods
    void setWeights       (const QString& weightMap);
    void setWeightForCell (const QPoint& position, int value);
    int  weightOfCell     (const QPoint& position);

    // These operate on view cell (that has some concrete size)
    void fillAtMP     (const QPoint& position);
    void unfillAtMP   (const QPoint& position);
    bool isFilledAtMP (const QPoint& position);

    void fillRow    (int row);
    void fillColumn (int column);
    void fillVector (const QVector<QVector<int> >& vector);

private:
    // Logic representation
    Grid m_grid;

    // Default constants
    static constexpr int MAX_WIDTH = 50;
    static constexpr int MAX_HEIGHT = 50;
    static constexpr int MAX_CELLSIZE = 64;

    // Sizes
    QSize m_mapSize;
    uint  m_cellSize;
};

#endif // MAP_H
