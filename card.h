#ifndef CARD_H
#define CARD_H
#include "hex.h"

class Card : public Hex
{
public:
    Card(QGraphicsItem* parent = NULL);
    Card(Card* card);

    virtual void NeighbourDetection();
    void switchOwner();
    void captureNeignbor();
    void showSideNum();
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    QString getCardString();
    //Modifier
    void setOwner(QString onwer);
    inline void setOnBoard(bool state){isOnBoard = state;}

    void setSideNum(int side,int num);
    // accessor
    inline QString getOwner(){return owner;}
    inline bool getOnBoard(){return isOnBoard;}
    inline int getSideNum(int side){return sideNum[side];}
private:
    bool isOnBoard;
    QString owner;
    int sideNum[6];
    QList<QGraphicsTextItem*> attackText;
    QLineF q;
public:

    QList<std::pair<int,Card*>> neighbours;



};

#endif // CARD_H
