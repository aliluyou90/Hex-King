#include "hex.h"
#include "game.h"
#include <QDebug>
extern Game* game;
Hex::Hex(QGraphicsItem* parent)
{
    QVector<QPointF> hexPoints;
    hexPoints << QPointF(1,0)<<QPointF(2,0)<<QPointF(3,1)
              <<QPointF(2,2)<<QPointF(1,2)<<QPointF(0,1);
    int scaler = 35;
    for (size_t i =0, n =hexPoints.size();i<n ; ++i){
        hexPoints[i]*=scaler;
    }
    QPolygonF hexagon(hexPoints);
    //set color
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(QColor(	139, 137, 137));
    setBrush(brush);
    setPolygon(hexagon);
    // text graphics
}
void Hex::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (game->cardHolded){
        game->placeCard(this);}
}

