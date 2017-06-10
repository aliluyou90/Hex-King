#include "gameserver.h"
#include <QDebug>
#include <QTcpSocket>
#include "game.h"
#include <QNetworkInterface>
#include <QHostInfo>
extern Game* game;
GameServer::GameServer(QObject *parent):QTcpServer(parent)
{
    connect(game,SIGNAL(decisionMade()),this,SLOT(sendMove()));
   // connect(game->chat,SIGNAL(readySend()),this,SLOT(sendChat()));
}

GameServer::~GameServer()
{
}

void GameServer::startServer()
{
    if(!this->listen(QHostAddress::AnyIPv4, 1234))
    {
        qDebug() << "Cannot start the server";
    }else{
        qDebug() << "server started";
        QHostInfo info = QHostInfo::fromName(QHostInfo::localHostName());
          QHostAddress address = info.addresses().at(this->serverAddress().AnyIPv4);
          qDebug() << address ;
    }
}

void GameServer::enemyMove(QList<QByteArray> &data)
{
    game->cardHolded = game->player2Cards[int(data[0][0]-'0')];
    if (data[1].size() == 2){
    game->placeCard(game->hexboard->hexes[int(data[1][0]-'0')*10+int(data[1][1]-'0')]);
    }else{
        game->placeCard(game->hexboard->hexes[int(data[1][0]-'0')]);
    }

}

void GameServer::clientdisconnect()
{
    client->disconnectFromHost();
}


void GameServer::readyRead()
{
    QByteArray rawData = client->readAll();
    QList<QByteArray> dataPackage = rawData.split('%');

    if(dataPackage[0] == "Dec"){
        qDebug()<< "enemy move";
        dataPackage.removeFirst();
        enemyMove(dataPackage);
    }else if(dataPackage[0]=="Msg")
    {
        game->chat->messageReceived(dataPackage[1]);

    }
    else{
        qDebug() << rawData;
    }

}

void GameServer::disconnected()
{
    QTcpSocket *client = (QTcpSocket*)sender();
    qDebug() << "Client disconnected:" << client->peerAddress().toString();
    game->backToManu();


}


void GameServer::sendMove()
{
    client->write(game->decision);
    client->waitForBytesWritten();
    game->decision.clear();

}

void GameServer::sendChat()
{
    client->write(game->chat->dataSend);
    client->waitForBytesWritten();
    game->chat->dataSend.clear();
}



void GameServer::incomingConnection(qintptr socketfd)
{
    client = new QTcpSocket();
    if(!client->setSocketDescriptor(socketfd))
    {
        qDebug()<< "connection error";
        return;
    }
    qDebug() << "Connection from " << client->socketDescriptor();
    connect(client, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(client, SIGNAL(disconnected()), this, SLOT(disconnected()));
    game->setSeed(time(NULL));
    game->startGame();
    QByteArray data;
    data.append("Init%");
    data.append(game->getSeed());
    qDebug()<< data;
    client->write(data);
    client->waitForBytesWritten();



}

