#ifndef GAME_H
#define GAME_H
#include <QgraphicsView>
#include <QGraphicsScene>
#include "hexboard.h"
#include <QList>
#include <stdlib.h> // rand
#include <time.h> // time
#include "card.h"
#include "score.h"
class Game : public QGraphicsView
{
    Q_OBJECT

public:
    Game(QWidget* parent= NULL);
    Card* cardHolded;
    HexBoard* hexboard;
    QPointF originalPos;
    Score *SC1;
    Score *SC2;
//methods
    QGraphicsScene * scene;
    inline QString getWhosTurn(){return whosTurn;}
    void setWhosTurn(QString player);
    void mainMenu();
    void pickUpCard(Card *card);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void placeCard(Hex *hexToReplace);
    void gameover();
public slots:
    void startGame();
    void restart();
private:
    //cards
    QList<Card*> player1Cards;
    QList<Card*> player2Cards;
    void createNewCard(QString player);
    void initCards(QString name);

    void removeFromDeck(Card* card,QString player);
    void switchTurn();
    //
    QString whosTurn;
    QGraphicsTextItem *whosTurnText;
    void creatInterface();
    void createPanle(int x, int y, int width,
                     int height, QColor color,
                     double  opacity);
};

#endif // GAME_H
