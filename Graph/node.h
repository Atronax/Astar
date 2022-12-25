#ifndef NODE_H
#define NODE_H

#include <QVector>
#include <QHash>

#include <QDebug>

// Node class represents 2-dimensional graph vertex.
// This will be used later to find shortest path on 2d tilemap later.
class Node
{
public:
    Node(int x = -1, int y = -1);
    ~Node();    

    int x() const;
    int y() const;

    void setX(int x);
    void setY(int y);

    bool isDefault();

    inline void operator= (const Node &rhs)
    {
        setX(rhs.x());
        setY(rhs.y());
    }

    inline QString toString() const
    {
        return QString("{%1,%2}").arg(x()).arg(y());
    }

protected:
    friend QDataStream& operator<< (QDataStream&, const Node&);
    friend QDataStream& operator>> (QDataStream&,       Node&);

private:
    const int MIN_VALUE = 0;
    const int MAX_VALUE = 100;

    int m_x;
    int m_y;
};

inline bool operator== (const Node &lhs, const Node &rhs)
{
    if (lhs.x() == rhs.x() && lhs.y() == rhs.y())
    {
        // qDebug() << "Shortest path. These nodes are equal.";
        return true;
    }
    else
        return false;
}

inline bool operator!= (const Node &lhs, const Node &rhs)
{
    if (lhs.x() != rhs.x() || lhs.y() != rhs.y())
        return true;
    else
        return false;
}

inline bool operator<  (const Node &lhs, const Node &rhs)
{
    if (lhs.y() < rhs.y())
        return true;
    else if (lhs.y() == rhs.y())
    {
        if (lhs.x() < rhs.x())
            return true;
        else
            return false;
    }
    else
        return false;
}

inline uint qHash(const Node& node, uint seed)
{
    return qHash(node.x() + node.y(),seed);
}

#endif // NODE_H
