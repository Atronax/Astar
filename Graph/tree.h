#ifndef TREE_H
#define TREE_H

#include <QVector>

#include "node.h"
#include "graph.h"

// Tree is a graph without cycles.
class Tree
{
public:
    Tree(const Node& root);
    Tree(const Node& root, const Graph& graph);
    ~Tree();

    void addChildNodeTo (const Node& to, const Node& child, int weight);
    const QSet<Node>& nodes() const;
    const QSet<Edge>& edges() const;

    QVector<Node> DFS_RootTo (const Node& to) const;
    QVector<Node> DFS_FromTo (const Node& node, const Node& to, QVector<Node> path);

    QString toString();

private:
    Node m_root;
    Graph m_graph;

    // Depth First Search
    Node unvisitedChildFor (const Node& node);
    bool hasUnvisitedChild (const Node& node);
    bool isVisited         (const Node& node);
    void visit             (const Node& node);
    QSet<Node> m_visited;
};

#endif // TREE_H
