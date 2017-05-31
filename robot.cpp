#include "robot.h"
#include "game.h"
#include <QDebug>
#include <QTimer>
extern Game* game;
Robot::Robot()
{

}

void Robot::easyMove()
{

    findBestPos(0);
    QTimer::singleShot(1000,this,SLOT(makeTheMove()));


}

void Robot::findBestPos(int index)
{
    thisCard = new Card(game->player2Cards[index]);
    game->scene->addItem(thisCard);
    auto availiblePos = game->hexboard->hexes;
    for (int i =0 , n = availiblePos.size(); i< n ;++ i){
        thisCard->setPos(availiblePos[i]->pos());
        thisCard->NeighbourDetection();

        int thisCardScore = scoreAnalysis();

        if(thisCardScore){
            decisionList.insert(thisCardScore,std::make_pair(index,i));
        }
        thisCard->neighbours.clear();
    }
    game->scene->removeItem(thisCard);
    delete thisCard;
    makeDecision();



}

int Robot::scoreAnalysis()
{
    int thisCardScore = 0;
    for ( size_t i=0, n = thisCard->neighbours.size(); i<n ; ++i){
        if(thisCard->neighbours[i].second->getOwner()!= thisCard->getOwner()){
            int thisNum =0, enemyNum =0;
            thisNum = thisCard->getSideNum(thisCard->neighbours[i].first);
            enemyNum = thisCard->neighbours[i].second->getSideNum((thisCard->neighbours[i].first+3+6)%6);

            if(thisNum > enemyNum ){
                //neighbours[i].second-> switchOwner();
                thisCardScore++;
}
        }
    }
    return thisCardScore;
}

void Robot::makeDecision()
{
    if (!decisionList.isEmpty()){
        finalDecision = decisionList.last();
   // qDebug() <<decisionList<< endl;
    decisionList.clear();
    }else{
        int randHex =  rand()% game->hexboard->hexes.size() + 0;
      ////  qDebug() << game->hexboard->hexes.size()<< "Hexes"<< endl;
;        finalDecision = std::make_pair(rand()%game->player2Cards.size(),randHex);
    }

   // qDebug() << finalDecision << endl;
}

void Robot::makeTheMove()
{
    game->cardHolded = game->player2Cards[finalDecision.first];
    game->placeCard(game->hexboard->hexes[finalDecision.second]);
}
