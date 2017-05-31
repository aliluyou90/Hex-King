#ifndef ROBOT_H
#define ROBOT_H
#include "card.h"
#include <QList>
class Robot : public QObject
{
    Q_OBJECT
    QMultiMap<int,std::pair<int,int>> decisionList;
    Card* thisCard;
    std::pair<int,int> finalDecision;
public:
    Robot();
    void easyMove();
    void normalMove();
    void findBestPos(int index);
    int scoreAnalysis();
    void makeDecision();
public slots:
    void makeTheMove();

};

#endif // ROBOT_H
