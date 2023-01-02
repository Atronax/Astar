#ifndef SPRITESHEET_H
#define SPRITESHEET_H

#include <QObject>

#include <QGraphicsItem>
#include <QImage>
#include <QTimer>

class Spritesheet : public QObject
{
    Q_OBJECT

public:
    explicit Spritesheet(const QString& filename, int frameWidth, int frameHeight, int fps, QGraphicsItem* parent = nullptr);

    bool empty() const;
    const QImage& currentFrame() const;

    inline void operator= (const Spritesheet& rhs)
    {
        m_atlas = rhs.m_atlas;
        m_fps   = rhs.m_fps;
        m_frameWidth  = rhs.m_frameWidth;
        m_frameHeight = rhs.m_frameHeight;

        cutAnimation(m_atlas, m_frameWidth, m_frameHeight);
        prepareAnimation(m_fps);
    }

private:
    void setFPS (int fps);
    void loadFromFile(const QString& filename, int frameWidth, int frameHeight);
    void cutAnimation(const QImage&  atlas,    int frameWidth, int frameHeight);
    void prepareAnimation(int fps);

    QGraphicsItem* m_parent;

    QImage m_atlas;
    QList<QImage> *m_frames;

    int    m_currentFrameIndex;

    int m_fps;
    int m_frameWidth;
    int m_frameHeight;

    bool forward = true;
    QTimer m_animationTimer;

signals:

public slots:
    void onAnimationTick();

};

#endif // SPRITESHEET_H
