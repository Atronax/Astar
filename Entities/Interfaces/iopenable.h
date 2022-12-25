#ifndef IOPENABLE_H
#define IOPENABLE_H

// This is an interface for all objects, that can be opened.
// Since interface is an abstract class with at least one pure virtual function,
// It can not be used to instantiate objects using it.
// However, it may be used to describe self-contained features of other classes, that have to be implemented later.

class iOpenable
{
public:
    void setOpened(bool isOpened) {m_isOpened = isOpened;}
    bool isOpened() const {return m_isOpened;}

    virtual void open() = 0;
    virtual void close() = 0;

private:
    bool m_isOpened;
};

#endif // IOPENABLE_H
