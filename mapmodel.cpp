#include <QSize>
#include <QPoint>

// #include <QDebug>

#include "mapmodel.h"
MapModel::MapModel(int width, int height, const uint& cellsize)
{
    m_grid     = Grid(QSize(width, height));

    m_mapSize  = QSize(width, height);
    m_cellSize = cellsize;
}

MapModel::~MapModel()
{

}

QVector<Node> MapModel::nodes() const
{
    QVector<Node> result;

    // Since logic nodes now have view representation with some size,
    // we need to update those values before returning the result.
    foreach (Node node, m_grid.nodes())
    {
        node.setX(node.x() * cellsize());
        node.setY(node.y() * cellsize());

        result.push_back(node);
    }

    return result;
}

QVector<Node> MapModel::shortestPath (const Node& from, const Node& to) const
{
    qDebug() << QString("Shortest path. There are %1 nodes in the grid.").arg(m_grid.nodes().size());

    return m_grid.shortestPath(from, to);
}

int MapModel::width() const
{
    return cellsInRow() * cellsize();
}

int MapModel::height() const
{
    return cellsInColumn() * cellsize();
}

int MapModel::cellsize() const
{
    return m_cellSize;
}

int MapModel::cellsInRow() const
{
    return m_mapSize.width();
}

int MapModel::cellsInColumn() const
{
    return m_mapSize.height();
}

void MapModel::resizeMap(int width, int height)
{
    qDebug() << "here: " << width << "," << height;

    /*
    bool  widthAllowed = (width  >= 0 && width  <= MAX_WIDTH);
    bool heightAllowed = (height >= 0 && height <= MAX_HEIGHT);

    if (!widthAllowed)
    {
        qDebug() << "Width is not allowed. Make sure it is in allowed intervals.";
        return;
    }

    if (!heightAllowed)
    {
        qDebug() << "Height is not allowed. Make sure it is in allowed intervals.";
        return;
    }
    */

    m_mapSize.setWidth(width);
    m_mapSize.setHeight(height);

    // qDebug() << m_mapSize.width() << ":" << m_mapSize.height();
}

void MapModel::resizeCell(int size)
{
    bool sizeAllowed = (size >= 0 && size <= MAX_CELLSIZE);

    if (!sizeAllowed)
    {
        qDebug() << "Size is not allowed";
        return;
    }

    m_cellSize = size;
}

void MapModel::fillCell(const QPoint &position)
{
    m_grid.fill(position);
}

void MapModel::fillAtMP(const QPoint &position)
{
    QPoint gridPosition = QPoint(position.x() / m_cellSize, position.y() / m_cellSize);

    m_grid.fill(gridPosition);
}

void MapModel::unfillCell(const QPoint &position)
{
    m_grid.unfill(position);
}

void MapModel::unfillAtMP(const QPoint &position)
{
    QPoint gridPosition = QPoint(position.x() / m_cellSize, position.y() / m_cellSize);

    m_grid.unfill(gridPosition);
}

bool MapModel::isFilledCell(const QPoint &position)
{
    return m_grid.isFilled(position);
}

void MapModel::setWeights(const QString &weightMap)
{
    qDebug() << weightMap.size() << ":" << m_mapSize.width() * m_mapSize.height();

    // If the size of the weight map is not the same as the size of the map itself, it is not allowed to use.
    if (weightMap.size() != m_mapSize.width() * m_mapSize.height())
    {
        qDebug() << "Weight map can't be used for this map";
        return;
    }

    // Resize the logic grid, if needed.
    m_grid.resize(m_mapSize.width(), m_mapSize.height());

    qDebug() << "After resize";

    // Place parsed weight value to each node of the logic grid.
    int index = 0;
    for (int h = 0; h < m_mapSize.height(); ++h)
    {
        for (int w = 0; w < m_mapSize.width(); ++w)
        {
            int weight = 0;

            QChar symbol = weightMap.at(index);
            if (symbol.isDigit())
                weight = symbol.digitValue();

            qDebug() << QString("Setting weight for node {%1, %2}: %3").arg(w).arg(h).arg(weight);
            setWeightForCell(QPoint(w,h), weight);

            ++index;
        }
    }
}

void MapModel::setWeightForCell(const QPoint &position, int value)
{
    // Graph graph_copy = m_grid.graph();
    Node node (position.x(), position.y());

    if (value <= 0)
        m_grid.fill(node);
    m_grid.setWeightFor(node, value);
}

bool MapModel::isFilledAtMP(const QPoint &position)
{
    QPoint gridPosition = QPoint(position.x() / m_cellSize, position.y() / m_cellSize);

    return m_grid.isFilled(gridPosition);
}

void MapModel::fillRow(int rowIndex)
{
    m_grid.fillRow(rowIndex);
}

void MapModel::fillColumn(int columnIndex)
{
    m_grid.fillColumn(columnIndex);
}

void MapModel::fillVector(const QVector<QVector<int> > &vector)
{
    m_grid.fillVector(vector);
}
