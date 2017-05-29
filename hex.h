#ifndef HEX_H
#define HEX_H
#include <QGraphicsPolygonItem>
#include <QBrush>
class Hex: public QGraphicsPolygonItem
{
public:
    Hex(QGraphicsItem* parent = NULL);
    //event
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // HEX_H
