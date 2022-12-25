#include <QTextStream>
#include <QFile>
#include <QSize>
#include <QPoint>

#include "grid.h"

Grid::Grid(const QSize& size)
{
    qDebug() << "in Grid constructor";

    m_size = size;    

    initialize();
    generateNodes();
}

Grid::~Grid()
{

}

Graph Grid::makeGraph() const
{
    // 1. Add all the existing nodes into the resulting graph.
    // 2. Add all the existing edges between unfilled nodes into graph. For this we need look through all the nodes:
    //    - Do nothing, if it is already filled.
    //    - Check all the neighbour node for each of the nodes in the graph and add connecting edges for them.

    Graph result;

    // add nodes
    foreach (Node node, nodes())
        result.addNode(node);

    // add edges
    foreach (Node node, result.nodes())
    {
        if (isFilled(node))
            continue;

        QVector<Node> neighbour_nodes = unfilledNeighbourNodesFor(node);
        foreach (Node neighbour, neighbour_nodes)
        {
            result.addEdge(node, neighbour, m_weights[neighbour]);
            result.addEdge(neighbour, node, m_weights[node]);
        }
    }

    qDebug() << "Shortest path. Graph was generated: ";
    qDebug() << "Shortest path. Nodes: " << result.nodes().size();
    qDebug() << "Shortest path. Edges: " << result.edges().size();

    return result;
}

void Grid::initialize()
{
    m_nodes    = QVector<QVector<Node>>(m_size.width()*m_size.height());
    m_isFilled = QMap<Node,bool>();
    m_weights  = QMap<Node, int>();    
}

void Grid::generateNodes()
{
    qDebug() << "Grid::generateNodes";
    qDebug() << "Node is empty: " << m_nodes.isEmpty();

    m_nodes.clear();
    m_nodes.resize(m_size.width()*m_size.height());

    // Do nothing for empty grids
    if (m_nodes.size() == 0)
        return;

    // Fill grid with 2d nodes
    for (int i = 0; i < m_size.width(); ++i)
    {
        // fill the column
        QVector<Node> node_column;
        for (int j = 0; j < m_size.height(); ++j)
            node_column.push_back(Node(i,j));

        // add it to list
        m_nodes.push_back(node_column);
    }

    // mark all the nodes as unfilled
    foreach (Node node, nodes())
        unfill(node);

    qDebug() << "Nodes size: " << m_nodes.size();
}

Graph Grid::graph() const
{
    return makeGraph();
}

void Grid::resize(int width, int height)
{
    qDebug() << "resize is called with " << width << ":" << height;

    bool widthIsAllowed = (width >= 0 && width <= MAX_WIDTH);
    bool heightIsAllowed = (height >= 0 && height <= MAX_HEIGHT);

    if (widthIsAllowed && heightIsAllowed)
    {
        m_size.setWidth(width);
        m_size.setHeight(height);
        qDebug() << "In Grid::resize. " << m_size;

        generateNodes();
    }
}

const Node& Grid::nodeAt(const QPoint &position)
{
    int row    = position.y();
    int column = position.x();

    // Since {m_nodes} is a vector of vectors,
    // y value of position represents row,
    // x value of position represents column.
    return m_nodes.at(row).at(column);
}

QVector<Node> Grid::nodes() const
{
    QVector<Node> result;

    foreach (QVector<Node> column, m_nodes)
    {
        foreach (Node node, column)
            result.push_back(node);
    }

    return result;
}

QVector<Node> Grid::shortestPath(const Node &from, const Node &to) const
{
    qDebug() << QString ("Shortest path. Weights (%1 nodes): ").arg(m_weights.size());
    for (int i = 0; i < m_weights.size(); ++i)
        qDebug() << "Shortest path. " << m_weights.keys().at(i).toString() << " : " << m_weights.values().at(i) << "\n";

    qDebug() << "Shortest path. Before";

    Graph graph_copy = graph();

    qDebug() << "Shortest path. There";
          graph_copy.setWeights(m_weights);

    qDebug() << "Shortest path. Here";
    qDebug() << "Shortest path. Node {3,3} has weight of " << graph_copy.weightOf(Node(3, 3));

    QVector<Node> shortestPath = graph_copy.shortestPath(from, to);

    qDebug() << QString("Shortest path found. There are %1 nodes there.").arg(shortestPath.size());
    qDebug() << QString("Shortest path is:");
    for (int i = 0; i < shortestPath.size(); ++i)
    {
        int index = i;
        Node node = shortestPath.at(i);
        qDebug() << QString("Shortest path. Step %1: %2.").arg(index).arg(node.toString());
    }


    return shortestPath;
}

QVector<Node> Grid::row(const int &i) const
{
    QVector<Node> result;

    foreach (QVector<Node> col_nodes, m_nodes)
        result.push_back(col_nodes[i]);

    return result;
}

QVector<Node> Grid::col(const int &i) const
{
    return m_nodes[i];
}

// ==================== SPT

// Mark the node as filled of unfilled (for one node, one point, whole row or column).
// Check if the node is filled or unfilled.
void Grid::fill(const Node &node)
{
    m_isFilled[node] = true;
}

void Grid::fill(const QPoint &pos)
{
    fill(Node(pos.x(),pos.y()));
}

void Grid::unfill(const Node &node)
{
    m_isFilled[node] = false;
}

void Grid::unfill(const QPoint &pos)
{
    unfill(Node(pos.x(),pos.y()));
}

bool Grid::isFilled(const Node &node) const
{
    return m_isFilled[node];
}

bool Grid::isFilled(const QPoint &pos) const
{
    return isFilled(Node(pos.x(),pos.y()));
}

void Grid::fillRow(const int &i)
{
    foreach (Node node, row(i))
        fill(node);
}

void Grid::fillColumn(const int &i)
{
    foreach (Node node, col(i))
        fill(node);
}

void Grid::fillVector(const QVector<QVector<int> > &vec)
{
    for (int y = 0; y < m_nodes[0].size(); ++y) // H
        for (int x = 0; x < m_nodes.size(); ++x) // W
            if (vec[y][x] == 1)
                fill(QPoint(x,y));
}

int Grid::weightFor(const Node &node)
{
    return m_weights[node];
}

int Grid::weightFor(const QPoint &position)
{
    return m_weights[Node(position.x(), position.y())];
}

void Grid::setWeightFor(const Node &node, int value)
{
    m_weights[node] = value;
}

void Grid::setWeightFor(const QPoint &position, int value)
{
    m_weights[Node(position.x(), position.y())] = value;
}

// Returns vector of all neighbouring unfilled nodes for current {node}.
// This method describes all directions (!), that are tracable from the current node.
QVector<Node> Grid::unfilledNeighbourNodesFor(const Node &node) const
{
    QVector<Node> result;

    int rows_count = m_nodes[0].size();
    int cols_count = m_nodes.size();

    // up
    if (node.y() > 0)
    {
        Node top_node (node.x(),node.y() - 1);
        if (!isFilled(top_node))
            result.push_back(top_node);
    }

    // down
    if (node.y() < rows_count - 1)
    {
        Node bottom_node (node.x(), node.y() + 1);
        if (!isFilled(bottom_node))
            result.push_back(bottom_node);
    }

    // left
    if (node.x() > 0)
    {
        Node left_node (node.x() - 1, node.y());
        if (!isFilled(left_node))
            result.push_back(left_node);
    }

    // right
    if (node.x() < cols_count - 1)
    {
        Node right_node (node.x() + 1, node.y());
        if (!isFilled(right_node))
            result.push_back(right_node);
    }

    /*
    // Diagonal movement:
    // вверх-влево
    if (node.y() > 0 && node.x() > 0)
    {
        Node topleft_node (node.x() - 1, node.y() - 1);
        if (!isFilled(topleft_node))
            result.push_back(topleft_node);
    }

    // вверх-вправо
    if (node.y() > 0 && node.x() < cols_count - 1)
    {
        Node topright_node (node.x() + 1, node.y() - 1);
        if (!isFilled(topright_node))
            result.push_back(topright_node);
    }

    // вниз-влево
    if (node.y() < rows_count - 1 && node.x() > 0)
    {
        Node bottomleft_node (node.x() - 1, node.y() + 1);
        if (!isFilled(bottomleft_node))
            result.push_back(bottomleft_node);
    }

    // вниз-вправо
    if (node.y() < rows_count - 1 && node.x() < cols_count - 1)
    {
        Node bottomright_node (node.x() + 1, node.y() + 1);
        if (!isFilled(bottomright_node))
            result.push_back(bottomright_node);
    }
    */

    return result;
}
