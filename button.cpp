#include "button.h"
#include <QBrush>
#include <QFont>
Button::Button(QString name,QGraphicsItem* parent):QGraphicsRectItem(parent)
{
    setRect(0,0,200,50);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::darkGray);
    text = new QGraphicsTextItem(name,this);// this is parent of the text, when parent deleted , text too
    text->setFont(QFont("times",20));
    int xPos = rect().width()/2 - text->boundingRect().width()/2;

    int yPos = rect().height()/2 - text->boundingRect().height()/2;
    text->setPos(xPos,yPos);

    // hover events

    setAcceptHoverEvents(true);


}

void Button::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit clicked();
}

void Button::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    // change color
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::green);
    setBrush(brush);
}
void Button::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    // change color
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::darkGray);
    setBrush(brush);
}
