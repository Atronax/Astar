#include "edge.h"

Edge::Edge(const Node& from, const Node& to, uint weight)
{
    m_from = from;
    m_to = to;
    m_weight = weight;
}

Edge::~Edge()
{

}

const Node &Edge::from() const
{
    return m_from;
}

const Node &Edge::to() const
{
    return m_to;
}

uint Edge::weight() const
{
    return m_weight;
}

