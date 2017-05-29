#ifndef SCORE_H
#define SCORE_H
#include <QGraphicsTextItem>

class Score:public QGraphicsTextItem
{
public:
    Score(QGraphicsItem *parent=0);
    inline void increase(){score ++;setPlainText(QString("Score: ")+ QString::number(score));}
    inline void decrease(){score --;setPlainText(QString("Score: ")+ QString::number(score));}
    inline int getScore()const{return this->score;}
private:
    int score;

};

#endif // SCORE_H
