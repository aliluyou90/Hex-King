#include "card.h"
#include <QDebug>
#include <game.h>
extern Game* game;
Card::Card(QGraphicsItem *parent): Hex(parent)
{
    isOnBoard = false;

    QPointF sideNumLoc[6] = {QPointF(42,0),QPointF(15,7),QPointF(15,30),
                             QPointF(42,45),QPointF(70,30),QPointF(70,7)};
    QGraphicsTextItem* sideNumText;
    for (size_t i =0; i<6; ++i){
        sideNum[i] = 0;
        sideNumText = new QGraphicsTextItem(QString::number(0),this);
        attackText.append(sideNumText);
        sideNumText->setFont(QFont("comic sans",15));
        sideNumText->setPos(sideNumLoc[i]);

    }
    for (size_t i =0, n = attackText.size(); i<n; ++i){
        attackText[i]->setVisible(false);
    }
    QPointF center(this->pos().x()+60,this->pos().y()+40);
    QPointF endP(center.x(),center.y()-65);
    q = QLineF(center,endP);
}

Card::Card(Card *card)
{
   QPointF sideNumLoc[6] = {QPointF(42,0),QPointF(15,7),QPointF(15,30),
                                 QPointF(42,45),QPointF(70,30),QPointF(70,7)};
    isOnBoard = card->isOnBoard;
    owner = card->owner;
    attackText = card->attackText;
    q = card->q;

    for (size_t i =0; i<6; ++i){
        sideNum[i] = card->sideNum[i];
    }


}



void Card::NeighbourDetection()
{
    // QGraphicsLineItem D;
    // Detector = new QGraphicsLineItem(ln,this);
     for(size_t i =0; i<6; i++){

         q.setAngle(90+60*i);
         QGraphicsLineItem D(q,this);
         QList<QGraphicsItem*> cItems = D.collidingItems();
         for(size_t j =0, n = cItems.size(); j<n; ++j){

             auto* Item = dynamic_cast<Card*>(cItems[j]);
             if( Item && cItems[j]!=this){
                 neighbours.append(std::make_pair(i,Item));


             }
         }


     }

}

void Card::switchOwner()
{
    if(getOwner() == QString("PLAYER1")){
        setOwner(QString("PLAYER2"));
        game->SC1->decrease();
        game->SC2->increase();
    }else if(getOwner() == QString("PLAYER2"))
    {
        setOwner(QString("PLAYER1"));
        game->SC2->decrease();
        game->SC1->increase();
    }
}

void Card::captureNeignbor()
{
    for ( size_t i=0, n = neighbours.size(); i<n ; ++i){

            //isEnemy = true;
        if(neighbours[i].second->getOwner()!= this->getOwner()){
            int thisNum =0, enemyNum =0;
            thisNum = this->getSideNum(neighbours[i].first);
            enemyNum = neighbours[i].second->getSideNum((neighbours[i].first+3+6)%6);

            if(thisNum > enemyNum ){
                neighbours[i].second-> switchOwner();
}
        }
    }
}

void Card::showSideNum()
{
    for (size_t i =0, n = attackText.size(); i<n; ++i){
        attackText[i]->setVisible(true);
    }
}

void Card::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(getOnBoard() == false){
        game->pickUpCard(this);
    }
  //  mousePressEvent(event);
}

void Card::setOwner(QString onwer)
{
    owner = onwer;
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    if (owner == QString("PLAYER1")){
        brush.setColor(QColor(205, 0, 0));

    }
    else if (owner == QString("PLAYER2")) {
        brush.setColor(QColor(0, 0, 255));

    }
    setBrush(brush);
}

void Card::setSideNum(int side, int num)
{
    sideNum[side]  = num;
    attackText[side]->setPlainText(QString::number(num));
}

