#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include <QGridLayout>

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QList>

#include "mapmodel.h"
#include "mapview.h"

class Board : public QWidget
{
    Q_OBJECT    

public:    
    Board(QWidget *parent = nullptr);
    ~Board();

private:    
    void prepareLayout();
    void prepareMap();    

    // Editor relevant stuff (move to canvas).
    // All the methods concerning creating, changing and removing tiles \ entities will go here.
    void placeCell(const QPoint& coords);
    void removeCell(const QPoint& coords);

    // Load and generate the map using XML file.
    void loadMap     (const QString& filename);

    // XML parsing stuff.
    // -----------------
    // 1. Generates DOM model of loaded file.
    // 2. Checks if it is actual map, that is allowed to use with this app.
    // 3. Parses relevant data from existing file and fills the necessary blocks of data in both model and view.
    // 4. When it's done, the loading procedure then clears both model and view and prepares them for new one.
    void parseXML           (const QString& content);
    void parseSymbolicMap   (const QDomNodeList& nodes);
    void parseWeightsTable  (const QDomNodeList& nodes);
    void generateWeightsMap ();    

    bool weightExistsFor (const QChar& symbol);
    QString cleanMap (const QString& dirtyMap);

    // MapModel is logic map, which is used to calculate the movement and other algorithmic intensive stuff
    // MapView  is visual representation for map, which is a list of entities(tiles), their graphics and other things, that changes based on project type.
    //    Controller for this case is integrated into view for simplicity purposes.
    MapModel* m_mapModel;
    MapView*  m_mapView;

    // Weight map stores the weights for loaded map, this is lated fed to map model for pathfinding system
    // Symbolic map XML holds an array of symbols, which represent the tile types; being parsed, they can be used to generate map
    // multilayered map can hold other stuff, that builds on top of previous stage, those can be used to fill the map with other objects, both static and dynamic
    // the same goes for other types of entities, that fills the map with life or whatsoever
    // table of weights connected to symbols (tile types), that are used by SPT algorithm, those are then feeded to graph
    QString m_symbolicMap;
    QString m_symbolicObjects;
    QString m_symbolicCreatures;
    QString m_symbolicItems;
    QMap<QChar,int> m_weightTable;
    QString m_weightMap;

    int m_width;
    int m_height;    
    int m_cellsize;

    QGridLayout* m_layout;

signals:
    void foundPath(const QVector<Node>& path);

public slots:
    void onFindPath (const Node& start, const Node& end);
};
#endif // BOARD_H
