#include "tree.h"

#include <QDebug>

Tree::Tree(const Node& root)
{
    m_root = root;
    m_graph.addNode(root);
}

Tree::Tree(const Node &root, const Graph &graph)
{
    m_root = root;
    m_graph = graph;
}

Tree::~Tree()
{

}

void Tree::addChildNodeTo(const Node &to, const Node &child, int weight)
{
    m_graph.addNode(child);
    m_graph.addEdge(to,child,weight);
}

const QSet<Node>& Tree::nodes() const
{
    return m_graph.nodes();
}

const QSet<Edge>& Tree::edges() const
{
    return m_graph.edges();
}

// recursive dfs
QVector<Node> Tree::DFS_RootTo(const Node &to) const
{
    Tree clone = *this;

    qDebug() << "Shortest path. Copying the tree to call the DFS algorithm.";
    qDebug() << "Shortest path. Tree details: " << clone.toString();
    qDebug() << "Shortest path. Calculating DFS";
    qDebug() << "Shortest path. ------------------------------";

    QVector<Node> path;
    return clone.DFS_FromTo(m_root, to, path);
}

// Returns path between nodes {node; to} as as vector of nodes
QVector<Node> Tree::DFS_FromTo(const Node &node, const Node &to, QVector<Node> path)
{
    /*
     * 1. Mark {node} as visited one.
     * 2. If {node} is a target (node == to), add it to path and return the {path} variable.
     * 3. If there is no visited childs,
     *    remember previous node (path.back()),
     *    remove current node for path vector (path.pop_back())
     *    and make a recursive call of DFS tree method for previous node.
     * 4. If there are unvisited childs, take one and call DFS tree method for it.
    */

    // qDebug() << "Shortest path. Looking for shortest path between two nodes.";

    visit (node);

    if (node == to)
    {
        path.push_back(node);
        qDebug() << QString("Shortest path. Path found. Nodes count: %1").arg(path.size());
        return path;
    }

    if (!hasUnvisitedChild(node))
    {
        qDebug() << QString("Shortest path. No unvisited nodes. Moving to previous node %1").arg(path.back().toString());

        Node last_node = path.back();
        path.pop_back();

        return DFS_FromTo(last_node, to, path);
    }
    else
    {
        qDebug() << QString("Shortest path. There are some unvisited nodes. Pushing node %1 to path. Looking the DFS for unvisited child.").arg(node.toString());

        path.push_back(node);
        return DFS_FromTo(unvisitedChildFor(node), to, path);
    }
}

QString Tree::toString()
{
    return QString("Root: %1. Count of nodes: %2. Count of edges: %3.").arg(m_root.toString()).arg(m_graph.nodes().size()).arg(m_graph.edges().size());
}

// Returns first unvisited node for node placed as parameters (the first in vector of outgoing nodes).
Node Tree::unvisitedChildFor(const Node &node)
{
    QVector<Node> children = m_graph.outgoingNodesFor(node);
    foreach (Node child, children)
    {
        if (!isVisited(child))
        {
            qDebug() << QString("Shortest path. Found unvisited node %1").arg(child.toString());
            return child;
        }
    }

    return Node();
}

// Returns true, if parameter node has at least one unvisited child.
bool Tree::hasUnvisitedChild(const Node &node)
{
    QVector<Node> children = m_graph.outgoingNodesFor(node);

    int children_unvisited = 0;

    foreach (Node child, children)
        if (!isVisited(child))
            ++children_unvisited;

    return children_unvisited > 0;
}

// Returns true, if parameter node is visited.
bool Tree::isVisited(const Node &node)
{
    return m_visited.contains(node);
}

// Marks the parameter node as visited.
void Tree::visit(const Node &node)
{    
    if (!m_visited.contains(node))
    {
        qDebug() << QString("Shortest path. Marking node %1 as visited").arg(node.toString());
        m_visited.insert(node);
    }
}
