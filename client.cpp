#include "client.h"
#include "ui_client.h"
#include <QDebug>
#include <QMessageBox>
#include "game.h"
extern Game* game;
Client::Client(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Client)
{
    m_pClientsocket = new QTcpSocket(this);
    ui->setupUi(this);
    connect(m_pClientsocket,SIGNAL(readyRead()),this,SLOT(readyRead()));
    connect(m_pClientsocket,SIGNAL(disconnected()),this,SLOT(disconnected()));
    ui->textIP->setText("127.0.0.1");
    ui->textPort->setText("1234");
    setAttribute(Qt::WA_TranslucentBackground);
    connect(game,SIGNAL(decisionMade()),this,SLOT(sendMove()));
}

Client::~Client()
{
    delete ui;
    qDebug() << "client destructed";
}

void Client::clientInitGame(QList<QByteArray> &data)
{
    game->startGame();
    game->setWhosTurn("PLAYER2");
    for (int i =0 ; i < 5 ;++ i){
        game->createNewCard("PLAYER1",data.at(0));
        data.removeFirst();
        auto* card = game->player1Cards[i];
        card->setPos(13,250+75*i);
        game->scene->addItem(card);
    }
    for (int i =0 ; i < 5 ;++ i){
        game->createNewCard("PLAYER2",data.at(0));
        data.removeFirst();
        auto* card = game->player2Cards[i];
        card->setPos(13+874,250+75*i);
        game->scene->addItem(card);
    }


}

void Client::enemyMove(QList<QByteArray> &data)
{
    game->cardHolded = game->player2Cards[int(data[0][0]-'0')];
    if (data[1].size() == 2){
    game->placeCard(game->hexboard->hexes[int(data[1][0]-'0')*10+int(data[1][1]-'0')]);
    }else{
        game->placeCard(game->hexboard->hexes[int(data[1][0]-'0')]);
    }
}

void Client::disconnected()
{
   qDebug()<< "disconnected by server!";
    deleteLater();
    game->backToManu();

}

void Client::readyRead()
{
    QByteArray rawData = m_pClientsocket->readAll();
    //QList<QString> q = Data.split("\r\n");
    QList<QByteArray> dataPackage = rawData.split('%');

    if (dataPackage[0] == "Init"){
        qDebug()<< "init the card";
        dataPackage.removeFirst();
        clientInitGame(dataPackage);
    }else if(dataPackage[0] == "Dec"){
        qDebug()<< "enemy move";
        dataPackage.removeFirst();
        enemyMove(dataPackage);
    }

    else{
        qDebug() << rawData;
    }



}

void Client::sendMove()
{
    m_pClientsocket->write( game->decision);
    qDebug()<< game->decision;
    m_pClientsocket->waitForBytesWritten();
    game->decision.clear();

}



void Client::displayError ( QAbstractSocket::SocketError socketError )
{
    switch (socketError) {
         case QAbstractSocket::RemoteHostClosedError:
             break;
         case QAbstractSocket::HostNotFoundError:
             QMessageBox::information(this, tr("Fortune Client"),
                                      tr("The host was not found. Please check the "
                                         "host name and port settings."));
             break;
         case QAbstractSocket::ConnectionRefusedError:
             QMessageBox::information(this, tr("Fortune Client"),
                                      tr("The connection was refused by the peer. "
                                         "Make sure the fortune server is running, "
                                         "and check that the host name and port "
                                         "settings are correct."));
             break;
         default:
             QMessageBox::information(this, tr("Fortune Client"),
                                      tr("The following error occurred: %1.")
                                      .arg(m_pClientsocket->errorString()));
         }

        game->restart();
}



void Client::on_connectButton_clicked()
{
    m_pClientsocket->connectToHost(ui->textIP->toPlainText(),qintptr(ui->textPort->toPlainText().toInt()) );
    connect(m_pClientsocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
    this->hide();

}

void Client::on_sendButton_clicked()
{
    QString message = ui->textMessage->toPlainText().trimmed();

    // Only send the text to the chat server if it's not empty:
    if(!message.isEmpty())
    {
        m_pClientsocket->write(QString(message + " ").toUtf8());
    }

    // Clear out the input box so they can type something else:
    ui->textMessage->clear();

    // Put the focus back into the input box so they can type again:
     ui->textMessage->setFocus();

}

void Client::on_backButton_clicked()
{
    game->backToManu();
}
