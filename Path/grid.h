#ifndef GRID_H
#define GRID_H

#include "Graph/graph.h"
#include <QtXml/QtXml>

class QSize;
class QPoint;

// Grid class represents 2-dimensional grid with logic cells of 1x1 size.
// This grid will be used then to find the shortest path between two cells.
// Every cell has 2-dimensional coordinates, tracebility status and weight.
class Grid
{
public:
    Grid(const QSize& size = QSize(0,0));
    ~Grid();

    Graph makeGraph() const;
    Graph graph() const;

    void resize (int width, int height);


    QVector<Node> nodes() const;
    const Node&   nodeAt (const QPoint& position);
    QVector<Node> shortestPath (const Node& from, const Node& to) const;

    QVector<Node> row (const int& index) const;
    QVector<Node> col (const int& index) const;

    // Making nodes tracable or untracable (by node, by position, by row, by column)
    void fill     (const Node& node);
    void unfill   (const Node& node);
    bool isFilled (const Node& node) const;

    void fill     (const QPoint& position);
    void unfill   (const QPoint& position);
    bool isFilled (const QPoint& position) const;

    void fillRow    (const int& rowIndex);
    void fillColumn (const int& colIndex);
    void fillVector (const QVector<QVector<int> >& vec);

    // Operating on weights
    int weightFor (const Node& node);
    int weightFor (const QPoint& position);    
    void setWeightFor (const Node& node, int value);
    void setWeightFor (const QPoint& position, int value);       

private:
    void initialize();
    void generateNodes();

    QVector<Node> unfilledNeighbourNodesFor (const Node& node) const;

    static constexpr int MAX_WIDTH = 50;
    static constexpr int MAX_HEIGHT = 50;

    Graph                   m_graph;
    QSize                   m_size;
    QVector<QVector<Node>>  m_nodes;
    QMap<Node, bool>        m_isFilled;
    QMap<Node, int >        m_weights;
};

#endif // GRID_H
