#ifndef GRAPH_H
#define GRAPH_H

#include <QSet>
#include <QMap>
#include <QVector>

#include "node.h"
#include "edge.h"

class Tree;

// Since graph is a set of nodes, that are connected using edges, the class description should have:
// 1. Set of nodes and edges between them
// 2. Map of nodes weights
// 3. Map of connecting edges
// 4. Helping sets of picked\unpicked nodes and edges (to look for SPT and SP)
class Graph
{
public:
    Graph(const QSet<Node>& nodes = QSet<Node>(), const QSet<Edge>& edges = QSet<Edge>());
    ~Graph();

    const QSet<Node>& nodes() const;
    const QSet<Edge>& edges() const;

    void setNodes(const QSet<Node>& value);
    void setEdges(const QSet<Edge>& value);

    void addNode (const Node& node);
    void addEdge (const Node& from, const Node& to, int weight);

    bool contains(const Node& node);
    bool contains(const Edge& edge);

    // return a vector of outgoing / incoming edges or nodes, that are to() / from() of these edges
    QVector<Edge> outgoingEdgesFor (const Node& node) const;
    QVector<Node> outgoingNodesFor (const Node& node) const;
    QVector<Edge> incomingEdgesFor (const Node& node) const;
    QVector<Node> incomingNodesFor (const Node& node) const;

    // methods to build the shortest path tree and find the shortest path between nodes {from} and {to} as a vector of sequantial nodes
    QVector<Node> shortestPath (const Node& from, const Node& to) const;
    Tree shortestPathTree (const Node& root) const;

    // Methods to operate on weights of the nodes, which are used to find the shortest path.
    // F.e., if used for tiles, this can be used to find the path with the minimal weight between two tiles.

    // Map of weights, that is generated based on some loaded tilemap.
    // +++++++++++++
    // ++++++++B111+
    // ++++wwwww441+
    // ++++22111141+
    // ++A22++++111+


    // A - start point, B - end point.
    // h - hills    (cost: 2ap)
    // r - road     (cost: 1ap)
    // m - mountain (cost: 4ap)
    // w - water    (untracable)

    // This methods will be useful since we operate these kind of grids.
    int  weightOf (const Node& node) const;
    void setWeights  (const QMap<Node, int>& weightMap);
    void setWeightOf (const Node& node, int weight);

private:
    QSet<Node> m_nodes;
    QSet<Edge> m_edges;

    // Helper methods, that are used by SPT algorithm.
    Edge connectingEdgeFor (const Node& node) const;
    Edge connectingEdgeOf  (const Node& from, const Node& to) const;
    void setConnectingEdgeFor  (const Node& node, const Edge& edge);
    void pickConnectingEdgeFor (const Node& node);
    QMap<Node,Edge> m_connectingEdge;


    void initWeights (const Node& starting_node);
    void updateWeightsForNeighbourNode (const Node& node);
    QMap<Node,int> m_nodeWeight;
    bool defaultWeights = true;

    void unpickAll ();
    bool hasUnpickedNode() const;
    Node lightestUnpickedNode() const;
    bool isPicked (const Node& node) const;
    bool isPicked (const Edge& edge) const;
    void pick (const Node& node);
    void pick (const Edge& edge);

    QVector<Node> unpickedNeighbourNodesFor (const Node& node) const;
    QSet<Node> m_pickedNodes, m_unpickedNodes;
    QSet<Edge> m_pickedEdges, m_unpickedEdges;
};

#endif // GRAPH_H
