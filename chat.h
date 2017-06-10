#ifndef CHAT_H
#define CHAT_H

#include <QDialog>

namespace Ui {
class Chat;
}

class Chat : public QDialog
{
    Q_OBJECT

public:
    explicit Chat(QWidget *parent = 0);
    ~Chat();
    QByteArray dataSend;
    void messageReceived(QByteArray message);
private slots:
    void on_sendButton_clicked();
signals:
    void readySend();
private:
    Ui::Chat *ui;
};

#endif // CHAT_H
