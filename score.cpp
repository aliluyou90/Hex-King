#include "score.h"
#include <QFont>
Score::Score(QGraphicsItem *parent):QGraphicsTextItem(parent)
{
score = 0;
// Draw text
setPlainText(QString("Score: ")+ QString::number(score));
setFont(QFont("times",30));

}

