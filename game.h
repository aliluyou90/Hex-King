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
#include "robot.h"
#include "gameserver.h"
#include "client.h"
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
    QList<Card*> player2Cards;
    QList<Card*> player1Cards;
    Robot *robot;
    GameServer *server;
    Client * client;
    QByteArray decision;
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
    void createNewCard(QString player);
    void createNewCard(QString player, QByteArray data);
public slots:
    void startGame();
    void hostStartGame();
    void robotStartGame();
    void restart();
    void backToManu();
    void joinGame();
signals:
    void cardInited();
    void decisionMade();
private:
    //cards



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
