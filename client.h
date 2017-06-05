#ifndef CLIENT_H
#define CLIENT_H

#include <QDialog>
#include <QTcpSocket>
namespace Ui {
class Client;
}

class Client : public QDialog
{
    Q_OBJECT
public:
    explicit Client(QWidget *parent = 0);
    ~Client();
     QTcpSocket *m_pClientsocket;
     void clientInitGame(QList<QByteArray> &data);
     void enemyMove(QList<QByteArray> &data);
private slots:
    void disconnected();
    void readyRead();
    void sendMove();
private slots:
    void displayError ( QAbstractSocket::SocketError socketError );

    void on_connectButton_clicked();

    void on_sendButton_clicked();

private:
    Ui::Client *ui;
};

#endif // CLIENT_H
