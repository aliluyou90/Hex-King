
#include <QApplication>
#include "game.h"
Game * game;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    srand(time(NULL));
    qDebug()<< int("49"- "00");
    game = new Game();
    game->mainMenu();
    game->show();
    return a.exec();
}
