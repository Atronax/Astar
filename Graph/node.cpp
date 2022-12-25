#include "node.h"

#include <QDataStream>

Node::Node(int x, int y)
{
    setX(x);
    setY(y);
}

Node::~Node()
{

}

int Node::x() const
{
    return m_x;
}

int Node::y() const
{
    return m_y;
}

void Node::setX(int x)
{
    if (x >= MIN_VALUE && x <= MAX_VALUE)
        m_x = x;
}

void Node::setY(int y)
{
    if (y >= MIN_VALUE && y <= MAX_VALUE)
        m_y = y;
}

bool Node::isDefault()
{
    return (m_x == -1 && m_y == -1);
}

QDataStream& operator>>(QDataStream &in, Node &node)
{
    int x;
    int y;

    in >> x >> y;

    node.setX(x);
    node.setY(y);

    return in;
}

QDataStream& operator<<(QDataStream &out, const Node &node)
{
    out << node.x() << node.y();

    return out;
}
