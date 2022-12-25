#include "board.h"

#include <QGraphicsLineItem>

Board::Board(QWidget *parent)
    : QWidget(parent)
{
    loadMap("D:/map.xml");
    prepareMap();
    prepareLayout();
}

Board::~Board()
{

}


void Board::prepareLayout()
{
    m_layout = new QGridLayout(this);

    m_layout->addWidget(m_mapView);
    setLayout(m_layout);
}

void Board::loadMap(const QString &filename)
{
    // Check if the format of XML is correct for this type of app.
    QFileInfo fi (filename);
    if (fi.suffix() != "xml")
    {
        qDebug() << "This file isn't of XML format.";
        return;
    }

    QFile file (filename);
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);

        QString xml = stream.readAll();
        parseXML(xml);        

        file.close();
    }
}

void Board::parseXML(const QString &content)
{
    // Generate weight map using symbolic map and feed it to grid.
    // Generate tiles      using symbolic map and factory method and add it to scene.
    // Later: multilayered symbolic maps, that allows adding various sorts of object on tiles and add those as
    //        some sort of entities on top of tiles

    // Then. Add timer (1000/fps) and do relevant stuff on each tick in its connected slot.


    // Check if this is an actual XML and make a DOM model for it, if it is.
    QDomDocument document;
    document.setContent(content);

    if (document.isNull())
    {
        qDebug() << "Could not parse the fed XML.";
        return;
    }

    // Parse map data:
    // - map is an 2d array of chars, which is used by factory method to fill the map with tiles,
    //                                here it will be used to set corresponding weights for nodes
    // - types are pairs<QChar, int>, that are used to replace the symbolic map with relevant weights

    parseSymbolicMap (document.elementsByTagName("map"));
    parseWeightsTable(document.elementsByTagName("types"));    
}

void Board::parseSymbolicMap(const QDomNodeList &mapNodes)
{
    // Check if any of the lists is empty.
    // There is no reason to parse the XML file further, if there is no relevant data.
    // When we'll have multilayered maps, mapNodes will have several symbolic arrays, each for each special type of objects.
    // All of those would be parsed at one place.

    if (mapNodes.isEmpty())
    {
        qDebug() << "There is no any map here.";
        return;
    }

    // Grab symbolic map from the map node
    // Index 0 - tiles   map
    // Index 1 - objects map
    // Index 2 - enemies map (for example)
    qDebug() << "Symbolic map found. Count of maps here: " << mapNodes.size();

    QDomNodeList maps  = mapNodes.at(0).childNodes();
    qDebug() << "Symbolic. Count of maps: " << maps.size();

    QDomNode tiles     = maps.at(0);
    QDomNode objects   = maps.at(1);
    QDomNode creatures = maps.at(2);
    QDomNode items     = maps.at(3);

    m_width  = mapNodes.at(0).attributes().namedItem("width").nodeValue().toInt();
    m_height = mapNodes.at(0).attributes().namedItem("height").nodeValue().toInt();

    // Clean the map. Dirty maps are not good for us.
    m_symbolicMap       = cleanMap(tiles.toElement().text());
    m_symbolicObjects   = cleanMap(objects.toElement().text());
    m_symbolicCreatures = cleanMap(creatures.toElement().text());
    m_symbolicItems     = cleanMap(items.toElement().text());

    qDebug() << "Symbolic map size: " << m_width << "x" << m_height;
    qDebug() << QString("Symbolic map:     %1").arg(m_symbolicMap);
    qDebug() << QString("Symbolic objects: %2").arg(m_symbolicObjects);
    qDebug() << QString("Symbolic enemies: %3").arg(m_symbolicCreatures);
    qDebug() << QString("Symbolic items:   %4").arg(m_symbolicItems);
}

void Board::parseWeightsTable(const QDomNodeList &nodes)
{
    qDebug() << "in Board::parseWeightsTable";

    if (nodes.isEmpty())
    {
        qDebug() << "No types definitions here";
        return;
    }

    // Grab all the needed type data for each of the nodes with the tag {"type"}
    for (int i = 0; i < nodes.size(); ++i)
    {
        QDomNode type = nodes.at(i);

        // For each tile type, we should grab its mark (char) and weight (int)
        // Push all the child nodes into another list and do some sorcery using them.
        QString mark;
        QString weight;

        qDebug() << "type node" << type.toElement().text();

        // List of all the <type></type> blocks
        QDomNodeList data = type.childNodes();
        for (int j = 0; j < data.size(); ++j)
        {
            // One <type></type> block. It has <mark></mark> block and <weight></weight> block,
            // Which are its children. To parse them, that the relevant child node and grab text from there.
            QDomNode block = data.at(j);

            // Block is a node, holding data of format {mark-cost}

            QStringList type = block.toElement().text().split(" - ");
            mark   = type.at(0);
            weight = type.at(1);

            // Now, that we have both char and corresponding weight value,
            // Compose a pair out of it and append it to our weight table.
            // When filled, it may be used to generate weight map for our map.
            // Feed it to map model to get the 2-dimensional pathfinding system for our entities.
            // What for:
            // - assume we have some unit on a tile and it is active unit
            // - when we click the mouse button on some other tile, we can select its node
            // - the pathfinding system then builds SPT and finds shortest path based on weights and tile types
            //   and returns the shortest path (and total weight sum, that could be used as action cost),
            //   that is used for moving using frame timer whatsoever.

            qDebug() << QString("Successfuly parsed new type. Mark: %1. Weight: %2").arg(mark).arg(weight);
            m_weightTable.insert(mark[0], weight.toInt());            

        }
    }

    qDebug() << "There.";
}

void Board::generateWeightsMap()
{
    int weight;

    // Replace the symbols of symbolic map with corresponding weight.
    // This will allow us to get the weight map, which is used to fill the map model
    // With all the neccesary data for our pathfinding system to work properly.
    for (int i = 0; i < m_symbolicMap.size(); ++i)
    {
        QChar symbol = m_symbolicMap.at(i);
        if (weightExistsFor(symbol))
        {
            weight = m_weightTable.value(symbol);
            m_weightMap.append(QString("%1").arg(weight));
        }
        else
            m_weightMap.append(m_symbolicMap.at(i));
    }

    qDebug() << "Weight map generated: \n " << m_weightMap;
}

bool Board::weightExistsFor(const QChar &symbol)
{
    if (m_weightTable.isEmpty())
    {
        qDebug() << "Weight table is empty.";
        return false;
    }

    // Look through the table to find the {symbol}, sent as a parameter.
    QList<QChar> allSymbols = m_weightTable.keys();
    foreach (QChar symb, allSymbols)
    {
        if (symb == symbol)
            return true;
    }

    return false;
}

QString Board::cleanMap(const QString &dirtyMap)
{
    QString result;

    // Clean the map out of irrelevant symbols
    for (int i = 0; i < dirtyMap.size(); ++i)
    {
        if (dirtyMap.at(i) == '\r' || dirtyMap.at(i) == '\n' || dirtyMap.at(i) == '\t')
            continue;

        result.append(dirtyMap.at(i));
    }

    // Restore the array form of the map
    /*
    for (int j = 0; j < result.size(); ++j)
    {
        if (j != 0 && j % (width + 1) == 0)
            result.insert(j, "\n");
    }
    */

    return result;
}

void Board::prepareMap()
{
    // When symbolic map (maps) and weight table are loaded from the xml file,
    // We turn to next stage. To prepare the map, we need
    // 1. Initialize the map model and fill it with all the neccesary data
    // 2. Initialize the map view  and fill it with all the neccesary objects based on loaded symbolic multilayered maps    

    // int size = WIDTH * HEIGHT / CELLSIZE * CELLSIZE;
    // m_map = new QList<Tile>();

    // Prepare model and view using loaded data.
    generateWeightsMap();

    m_mapModel = new MapModel(m_width, m_height, m_cellsize);
    m_mapModel->setWeights(m_weightMap);

    m_mapView  = new MapView(m_width, m_height);
    m_mapView->buildMap(m_symbolicMap, m_weightMap);

    connect (m_mapView, SIGNAL(findPath (const Node&, const Node&)), this     , SLOT(onFindPath (const Node&, const Node&)));
    connect (this,      SIGNAL(foundPath(const QVector<Node>&))    , m_mapView, SLOT(onFoundPath(const QVector<Node>&)));
}

void Board::placeCell(const QPoint &coords)
{
    Q_UNUSED(coords);
    // generate new cell with relevant properties and coordinates:coords, add it to the scene and holding list

}

void Board::removeCell(const QPoint &coords)
{
    Q_UNUSED(coords);
    // find the cell with coordinates:coords, remove the object from the scene and holding list
}

void Board::onFindPath(const Node &from, const Node &to)
{
    qDebug() << QString("Looking for shortest path between nodes %1 and %2").arg(from.toString()).arg(to.toString());
    QVector<Node> path = m_mapModel->shortestPath(from, to);

    qDebug() << "Shortest Path: ";
    for (int i = 0; i < path.size(); ++i)
        qDebug() << path.at(i).toString();

    emit foundPath(path);
}
