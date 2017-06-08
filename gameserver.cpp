#include "gameserver.h"
#include <QDebug>
#include <QTcpSocket>
#include "game.h"
#include <QNetworkInterface>
#include <QHostInfo>
extern Game* game;
GameServer::GameServer(QObject *parent):QTcpServer(parent)
{
    connect(game,SIGNAL(cardInited()),this,SLOT(sendInitedCard()));
    connect(game,SIGNAL(decisionMade()),this,SLOT(sendMove()));

}

GameServer::~GameServer()
{
    qDebug()<< "Server destructed";
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
          //第一个IP地址；
          qDebug() << address ;
    }



/*
    QString detail="";
        QList<QNetworkInterface> list=QNetworkInterface::allInterfaces();
        for(int i=0;i<list.count();i++)
        {
            QNetworkInterface interface=list.at(i);
            detail=detail+tr("")+interface.name()+"\n";
            QList<QNetworkAddressEntry> entryList=interface.addressEntries();
            for(int j=0;j<entryList.count();j++)
            {
                QNetworkAddressEntry entry=entryList.at(j);
                detail=detail+"\t"+tr("IP")+entry.ip().toString()+"\n";
            }
        }
        qDebug() << detail ;
        */

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


void GameServer::readyRead()
{
    QByteArray rawData = client->readAll();
    QList<QByteArray> dataPackage = rawData.split('%');

    if(dataPackage[0] == "Dec"){
        qDebug()<< "enemy move";
        dataPackage.removeFirst();
        enemyMove(dataPackage);
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

void GameServer::sendInitedCard()
{
    QByteArray data;
// problem! putting the length in the data to be sent!!!!!!
    if(game->getWhosTurn() == "PLAYER1"){
        data.append("P1%");
        for (int i =0, n = game->player1Cards.size();i<n ; i++){
            data.append( game->player1Cards[i]->getCardString());
        }
    }else{
        data.append("P2%");
        for (int i =0, n = game->player2Cards.size();i<n ; i++){
            data.append( game->player2Cards[i]->getCardString());
        }
    }
    client->write(data);
    client->waitForBytesWritten();


}

void GameServer::sendMove()
{
    client->write(game->decision);
    client->waitForBytesWritten();
    game->decision.clear();

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
   // client->write("Hello client\r\n");
  //  client->waitForBytesWritten(100);
    //client->flush();
    connect(client, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(client, SIGNAL(disconnected()), this, SLOT(disconnected()));

    game->startGame();
    QByteArray data;
    data.append("Init%");// "1" is cardinfo data identifier
    for (int i =0, n = game->player2Cards.size();i<n ; i++){
        data.append( game->player2Cards[i]->getCardString());
    }
    for (int i =0, n = game->player1Cards.size();i<n ; i++){
        data.append( game->player1Cards[i]->getCardString());
    }
    client->write(data);
    client->waitForBytesWritten();



}

