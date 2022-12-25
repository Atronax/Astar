#ifndef EDGE_H
#define EDGE_H

#include "node.h"

// Edge class describes the edge between two nodes.
// Since we want our units to move across various kinds of tiles on our tilemap,
// The nodes should have different weights and edges would do just fine here.
// Tracing algorithm will use that data to find the shortest path between two nodes.
class Edge
{
public:    
    Edge(const Node& from = Node(), const Node& to = Node(), uint weight = 0);
    ~Edge();

    uint weight() const;
    const Node& from() const;
    const Node& to() const;

    inline void operator= (const Edge& rhs)
    {
        m_from   = rhs.m_from;
        m_to     = rhs.m_to;
        m_weight = rhs.m_weight;
    }

private:
    uint m_weight;
    Node m_from;
    Node m_to;
};

inline bool operator== (const Edge &lhs, const Edge &rhs)
{
    if (lhs.from() == rhs.from() && lhs.to() == rhs.to() && lhs.weight() == rhs.weight())
        return true;
    else
        return false;
}

inline bool operator!= (const Edge &lhs, const Edge &rhs)
{
    if (lhs.from() != rhs.from() || lhs.to() != rhs.to() || lhs.weight() != rhs.weight())
        return true;
    else
        return false;
}

inline uint qHash(const Edge& edge, uint seed)
{
    return qHash(edge.from(), seed) + qHash(edge.to(), seed) + qHash(edge.weight(), seed);
}

#endif // EDGE_H
