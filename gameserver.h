#ifndef GAMESERVER_H
#define GAMESERVER_H
#include <QObject>
#include <QTcpServer>


class GameServer : public QTcpServer
{
    Q_OBJECT

public:
    GameServer(QObject *parent =0);
    ~GameServer();
    void startServer();
    void enemyMove(QList<QByteArray> &data);
    void clientdisconnect();
private slots:
    void readyRead();
    void disconnected();
    void sendMove();
    void sendChat();
protected:
    void incomingConnection(qintptr socketfd);

private:
    QTcpSocket* client;
};


#endif // GAMESERVER_H
