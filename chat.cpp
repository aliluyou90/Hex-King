#include "chat.h"
#include "ui_chat.h"

Chat::Chat(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Chat)
{
    ui->setupUi(this);
}

Chat::~Chat()
{
    delete ui;
}

void Chat::messageReceived(QByteArray message)
{
    ui->receiveTextBrowser->append(message);
}

void Chat::on_sendButton_clicked()
{
    dataSend.append("Msg%");
    dataSend.append( ui->sendLineEdit->text());
    ui->sendLineEdit->clear();
    emit readySend();
}
