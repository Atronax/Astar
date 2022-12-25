#include "tree.h"
#include "graph.h"

#include <QDebug>

Graph::Graph(const QSet<Node>& nodes, const QSet<Edge>& edges)
{
    m_nodes = nodes;
    m_edges = edges;
}

Graph::~Graph()
{

}

const QSet<Node> &Graph::nodes() const
{
    return m_nodes;
}

const QSet<Edge> &Graph::edges() const
{
    return m_edges;
}

bool Graph::contains(const Node &node)
{
    return m_nodes.contains(node);
}

bool Graph::contains(const Edge &edge)
{
    return m_edges.contains(edge);
}

void Graph::setNodes(const QSet<Node> &value)
{
    m_nodes = value;
}

void Graph::setEdges(const QSet<Edge> &value)
{
    m_edges = value;
}

void Graph::addNode(const Node &node)
{
    m_nodes.insert(node);
}

void Graph::addEdge(const Node &from, const Node &to, int weight)
{
    m_edges.insert(Edge(from,to,weight));
}

QVector<Edge> Graph::outgoingEdgesFor(const Node &node) const
{
    QVector<Edge> result;

    foreach (Edge edge, m_edges)
        if (node == edge.from())
            result.push_back(edge);

    return result;
}

QVector<Edge> Graph::incomingEdgesFor(const Node &node) const
{
    QVector<Edge> result;

    foreach (Edge edge, m_edges)
        if (node == edge.to())
            result.push_back(edge);

    return result;
}

// Returns vector of all the nodes, that have incoming edges connecting them with {node}.
QVector<Node> Graph::outgoingNodesFor(const Node &node) const
{
    QVector<Node> result;

    QVector<Edge> outgoing_edges = outgoingEdgesFor(node);
    foreach (Edge edge, outgoing_edges)
        result.push_back(edge.to());

    return result;
}

// Returns vector of all the nodes, that have outgoing edges connecting them with {node}.
QVector<Node> Graph::incomingNodesFor(const Node &node) const
{
    QVector<Node> result;

    QVector<Edge> incoming_edges = incomingEdgesFor(node);
    foreach (Edge edge, incoming_edges)
        result.push_back(edge.from());

    return result;
}

// Shortest Path Tree.
// Returns SPT with the root node. Operates on graph copy. Use further methods.
QVector<Node> Graph::shortestPath(const Node &from, const Node &to) const
{
    qDebug() << "Shortest path. In Graph::sp";

    return shortestPathTree(from).DFS_RootTo(to);
}

Tree Graph::shortestPathTree(const Node &root) const
{
    Graph graph_copy = *this;

    // 1. Mark all the node as unpicked.
    // 2. Initialize nodes weights.
    // 3. While the graph has any unpicked node:
    //    - Pick the node with the lowest weight
    //    - Update weights of all its neighbours
    //    - If first picked node is a root for new tree, we mustn't pick its edge. For all other case, do that step.
    //    - Make a graph with all the chosen elements

    // Call only if we are using simple map with cell weights of 1.
    // Otherwise,
    // - load the weight data in some sort of 2d array;
    // - fill the relevant graph member variables using {graph.setWeights};
    // - and only then calculate the shortest path tree and return the shortest path as a sequnce of nodes.

    graph_copy.unpickAll();
    graph_copy.initWeights(root);

    Node rootSPT = Node(0,0);
    bool isFirstNode = true;
    while (graph_copy.hasUnpickedNode())
    {
        Node lightest_node = graph_copy.lightestUnpickedNode();

        graph_copy.pick(lightest_node);
        graph_copy.updateWeightsForNeighbourNode(lightest_node);
        if (isFirstNode)
        {
            rootSPT = lightest_node;
            isFirstNode = false;
            continue;
        }
        graph_copy.pickConnectingEdgeFor(lightest_node);
    }

    Graph graph (graph_copy.m_pickedNodes, graph_copy.m_pickedEdges);

    qDebug() << "Shortest path. SPT has been built.";

    return Tree(rootSPT, graph);
}

void Graph::setWeights(const QMap<Node, int> &weightMap)
{
    for (int i = 0; i < weightMap.size(); ++i)
    {
        Node node   = weightMap.keys().at(i);
        int  weight = weightMap.values().at(i);

        qDebug() << QString("Shortest path. Node %1 has weight of %2").arg(node.toString()).arg(weight);

        setWeightOf(node, weight);
    }

    defaultWeights = false;
}

void Graph::pick(const Node &node)
{
    m_unpickedNodes.remove(node);
    m_pickedNodes.insert(node);
}

void Graph::pick(const Edge &edge)
{
    m_unpickedEdges.remove(edge);
    m_pickedEdges.insert(edge);
}

void Graph::unpickAll()
{
    foreach (Node node, m_nodes)
        m_unpickedNodes.insert(node);

    foreach (Edge edge, m_edges)
        m_unpickedEdges.insert(edge);
}

bool Graph::isPicked(const Node &node) const
{
    return m_pickedNodes.contains(node);
}

bool Graph::isPicked(const Edge &edge) const
{
    return m_pickedEdges.contains(edge);
}

Edge Graph::connectingEdgeFor(const Node& node) const
{
    return m_connectingEdge[node];
}

void Graph::setConnectingEdgeFor(const Node &node, const Edge &edge)
{
    m_connectingEdge[node] = edge;
}

Edge Graph::connectingEdgeOf(const Node &from, const Node &to) const
{
    foreach (Edge edge, m_edges)
        if (edge.from() == from && edge.to() == to)
            return edge;

    return Edge();
}

void Graph::pickConnectingEdgeFor(const Node &node)
{
    pick(connectingEdgeFor(node));
}

int Graph::weightOf(const Node &node) const
{
    return m_nodeWeight[node];
}

void Graph::setWeightOf(const Node &node, int weight)
{
    m_nodeWeight[node] = weight;
}

bool Graph::hasUnpickedNode() const
{
    return !m_unpickedNodes.empty();
}

Node Graph::lightestUnpickedNode() const
{
    Node result = m_unpickedNodes.values().first();

    foreach (Node node, m_unpickedNodes)
        if (weightOf(node) < weightOf(result))
            result = node;

    return result;
}

QVector<Node> Graph::unpickedNeighbourNodesFor(const Node &node) const
{
    QVector<Node> result;

    foreach (Node neighbour_node, outgoingNodesFor(node))
        if (!isPicked(neighbour_node))
            result.push_back(neighbour_node);

    return result;
}

// Weights methods
void Graph::initWeights(const Node &startnode)
{
    setWeightOf(startnode, 0);
    foreach (Node node, m_nodes)
        if (node != startnode)
            setWeightOf(node, 1000);
}

// Updates the weight of neighbour nodes, if their new weights are lower than before.
// Places relevant edge between given and neighbouring node (shortest one).
void Graph::updateWeightsForNeighbourNode(const Node &node)
{
    foreach (Node neighbour, unpickedNeighbourNodesFor(node))
    {
        Edge edge_to_neighbour = connectingEdgeOf(node,neighbour);

        qDebug() << "Shortest path. Edge: " << edge_to_neighbour.weight();

        int old_weight = weightOf(neighbour);
        int new_weight = weightOf(node) + edge_to_neighbour.weight();

        // qDebug() << QString("Shortest path. Weight of neighbour node %1 is %2").arg(neighbour.toString()).arg(weightOf(neighbour));
        // qDebug() << QString("Shortest path. Weight of node %1 is %2").arg(node.toString()).arg(weightOf(node));

        if (new_weight < old_weight)
        {
            setWeightOf(neighbour, new_weight);
            setConnectingEdgeFor(neighbour, edge_to_neighbour);

            qDebug() << QString("Shortest path. New weight %1 placed for node %2.").arg(new_weight).arg(neighbour.toString());
        }
    }
}
