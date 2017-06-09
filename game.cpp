#include "game.h"
#include "button.h"
#include <QDebug>
#include <QTextEdit>
#include <QLineEdit>
Game::Game(QWidget * parent) : QGraphicsView(parent)
{
 // set up screen
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(1024,768);
    //set up the scene
    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,1024,768);
    setBackgroundBrush(QColor(205, 170, 125));
    setScene(scene);
    cardHolded = nullptr;
    robot = nullptr;
    server = nullptr;
    client = nullptr;

}

void Game::setWhosTurn(QString player)
{

            whosTurn = player;
            whosTurnText->setPlainText(player+QString("'s turn") );
            if (player == QString("PLAYER1")){
              whosTurnText->setDefaultTextColor(QColor(205, 0, 0));
            }else{whosTurnText->setDefaultTextColor(QColor(0, 0 ,205));
        }
}

void Game::mainMenu()
{
    QGraphicsTextItem * title = new QGraphicsTextItem(QString("Hex King"));
    QFont titleFont("comic sans",60);
    title->setFont(titleFont);
    int txPos = this->width()/2-title->boundingRect().width()/2;
    int tyPos = 100;
    title->setPos(txPos,tyPos);
    scene->addItem(title);



    auto* robotPlayButton = new Button(QString("Play with Robot"));
    int prxPos = this->width()/2-robotPlayButton->boundingRect().width()/2;
    int pryPos = 250;
    robotPlayButton->setPos(prxPos,pryPos);
    connect(robotPlayButton,SIGNAL(clicked()),this,SLOT(robotStartGame()));
    scene->addItem(robotPlayButton);

    auto* hostButton = new Button(QString("Host Game"));
    int phxPos = this->width()/2-hostButton->boundingRect().width()/2;
    int phyPos = 330;
    hostButton->setPos(phxPos,phyPos);
    connect(hostButton,SIGNAL(clicked()),this,SLOT(hostStartGame()));
    scene->addItem(hostButton);

    auto* joinButton = new Button(QString("Join Game"));
    int pjxPos = this->width()/2-joinButton->boundingRect().width()/2;
    int pjyPos = 410;
    joinButton->setPos(pjxPos,pjyPos);
    connect(joinButton,SIGNAL(clicked()),this,SLOT(joinGame()));
    scene->addItem(joinButton);

    auto* selfPlayButton = new Button(QString("Start"));
    int pbxPos = this->width()/2-selfPlayButton->boundingRect().width()/2;
    int pbyPos = 490;
    selfPlayButton->setPos(pbxPos,pbyPos);
    connect(selfPlayButton,SIGNAL(clicked()),this,SLOT(startGame()));
    scene->addItem(selfPlayButton);

    auto* quitButton = new Button(QString("Quit"));
    int qbxPos = this->width()/2-quitButton->boundingRect().width()/2;
    auto qbyPos = 570;
    quitButton->setPos(qbxPos,qbyPos);
    connect(quitButton,SIGNAL(clicked()),this,SLOT(close()));
    scene->addItem(quitButton);


}

void Game::pickUpCard(Card *card)
{
    //pick up the specified card
    QString player;
    if (client){
        player = "PLAYER2";
    }else{
         player = "PLAYER1";
    }
    if (card->getOwner() == getWhosTurn()&& player==getWhosTurn() && cardHolded == nullptr){
        cardHolded = card;
        originalPos = card->pos();
        return;
    }

}

void Game::mouseMoveEvent(QMouseEvent *event)
{
    //follow the mouse
    if (cardHolded){
        cardHolded->setPos(event->pos());
    }
    QGraphicsView::mouseMoveEvent(event);
}

void Game::mousePressEvent(QMouseEvent *event)
{
    // make right click to return card
    if(event->button() == Qt::RightButton){
        if(cardHolded){
        cardHolded ->setPos(originalPos);
        cardHolded = nullptr;
        return;
    }
}
    QGraphicsView::mousePressEvent(event);
}
void Game::placeCard(Hex *hexToReplace)
{
// a card replaces a hex;

    if(server && getWhosTurn() == "PLAYER1"){

        decision.append("Dec%");
        decision.append(QString::number(player1Cards.indexOf(cardHolded))+"%");
        decision.append(QString::number(hexboard->hexes.indexOf(hexToReplace)));
       }else if ( getWhosTurn() == "PLAYER2" && client)
    {
        decision.append("Dec%");
        decision.append(QString::number(player2Cards.indexOf(cardHolded))+"%");
        decision.append(QString::number(hexboard->hexes.indexOf(hexToReplace)));
    }
    emit decisionMade();

    cardHolded->setPos(hexToReplace->pos());
    cardHolded->setOnBoard(true);
    cardHolded->NeighbourDetection();
    cardHolded->captureNeignbor();
    removeFromDeck(cardHolded,getWhosTurn());
    cardHolded = nullptr;
// remove the hex from the scene and board
    scene->removeItem(hexToReplace);
    hexboard->hexes.removeAll(hexToReplace);
// game over when no hex left
    if (hexboard->hexes.size() ==0){
         gameover();
         emit cardInited();
         return;
     }
// continue  switch round

    if(!player1Cards.size()){
        initCards("PLAYER1");
    }else if(!player2Cards.size()){
        initCards("PLAYER2");

        }
    switchTurn();
}

void Game::gameover()
{
    // show gameover window
    for (size_t i =0, n = scene->items().size(); i<n; ++i){
        scene->items()[i]->setEnabled(false);
    }
    createPanle(0,0,1024,768,Qt::black,0.65);
    createPanle(312,184,400,400,Qt::lightGray,0.75);
    auto* playAgain = new Button(QString("Restart"));
    playAgain->setPos(410,300);
    scene->addItem(playAgain);
    connect(playAgain,SIGNAL(clicked()),this,SLOT(restart()));
    auto* quitButton = new Button(QString("Quit"));
    quitButton->setPos(410,375);
    connect(quitButton,SIGNAL(clicked()),this,SLOT(close()));
    scene->addItem(quitButton);

    // FInd who wins
    QString message;
    if (SC1->getScore()>SC2->getScore()){
        message = QString("Player1 WINS!");
    }else if (SC1->getScore()<SC2->getScore()){
        message = QString("Player2 WINS!");
    }else{
        message = QString("Tie Game!");
    }
    auto * p1 = new QGraphicsTextItem(message);
    QFont titleFont("comic sans",15);
    p1->setFont(titleFont);
    p1->setPos(1024/2-p1->boundingRect().width()/2,250);
    scene->addItem(p1);
}


void Game::switchTurn()
{
    if(getWhosTurn() == QString("PLAYER1")){
        setWhosTurn(QString("PLAYER2"));
        if(robot){
        robot->easyMove();}

    }else{
        setWhosTurn(QString("PLAYER1"));
    }
}

void Game::startGame()
{
   setSeed(time(NULL));
   srand(randSeed);
   scene->clear();
   hexboard = new HexBoard();
   hexboard->placeHex(240,30,7,7);

   creatInterface();

   initCards(QString("PLAYER1"));
   initCards(QString("PLAYER2"));
}


void Game::hostStartGame()
{

    server = new GameServer();
    server->startServer();

}

void Game::robotStartGame()
{
    robot = new Robot();
    startGame();
}

void Game::restart()
{
    player1Cards.clear();
    player2Cards.clear();
    scene->clear();
    startGame();
}

void Game::backToManu()
{
    if (client){
        client->deleteLater();
        client = nullptr;
    }
    if(server){
        //server->close();
        server->clientdisconnect();
        delete server;
        server = nullptr;
    }
    player1Cards.clear();
    player2Cards.clear();
    if(cardHolded){
    delete cardHolded;
    cardHolded = nullptr;
}
    scene->clear();
    mainMenu();
}

void Game::joinGame()
{

    client = new Client();
    scene->clear();
    client->show();

}

void Game::createNewCard(QString player )
{
    auto* card = new Card();
    card->setOwner(player);
    card->setOnBoard(false);

    for (size_t i =0; i<6; ++i){

        int randNum = rand() %6 + 1; // always same number
        card->setSideNum(i,randNum);
    }
    card->showSideNum();
    if (player == QString("PLAYER1"))
        player1Cards.append(card);
    else
        player2Cards.append(card);
}



void Game::initCards(QString name)
{
    for (size_t i = 0; i<5 ;++i){
        createNewCard(name);

    }
    if ( QString("PLAYER1") == name)     {
        for (size_t i = 0, n = player1Cards.size(); i<n ;++i){
            auto* card = player1Cards[i];
            card->setPos(13,250+75*i);
            scene->addItem(card);
        }
    }else{
        for (size_t i = 0, n = player2Cards.size(); i<n ;++i){
            auto* card = player2Cards[i];
            card->setPos(13+874,250+75*i);
            scene->addItem(card);
        }
    }
}


void Game::creatInterface()
{

   auto * p1 = new QGraphicsTextItem(QString("Player 1"));
   QFont titleFont("comic sans",30);
   p1->setFont(titleFont);
   p1->setPos(10,0);
   scene->addItem(p1);
   auto * p2 = new QGraphicsTextItem(QString("Player 2"));
   p2->setFont(titleFont);
   p2->setPos(834,0);
   scene->addItem(p2);

   auto* backButton = new Button(QString("Quit"));
   backButton->setPos(800,670);
   connect(backButton,SIGNAL(clicked()),this,SLOT(backToManu()));
   scene->addItem(backButton);

   SC1 = new Score();
   SC1->setFont(titleFont);
   SC1->setPos(10,50);
   scene->addItem(SC1);
   SC2 = new Score();
   SC2->setFont(titleFont);
   SC2->setPos(834,50);
   scene->addItem(SC2);


   whosTurnText = new QGraphicsTextItem();
   setWhosTurn(QString("PLAYER1"));
   whosTurnText->setFont(QFont("comic sans",30));
   whosTurnText->setPos(420,0);
   scene->addItem(whosTurnText);

}

void Game::createPanle(int x, int y, int width, int height, QColor color, double opacity)
{
    //draw
    auto* panel = new QGraphicsRectItem (x,y,width,height);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(color);
    panel->setBrush(brush);
    panel->setOpacity(opacity);
    scene->addItem(panel);
}

void Game::removeFromDeck(Card *card, QString player)
{
    if (player == QString("PLAYER1")){
        // remove from p1
        player1Cards.removeAll(card);
        SC1->increase();

    }
    if (player == QString("PLAYER2")){
        // remove from p2
        player2Cards.removeAll(card);
        SC2->increase();
    }
}
