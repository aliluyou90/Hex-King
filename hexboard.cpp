#include "hexboard.h"
#include "game.h"
extern Game * game;
HexBoard::HexBoard()
{

}

QList<Hex *> HexBoard::getHexes()
{
    return hexes;
}

void HexBoard::placeHex(int x, int y, int cols, int rows)
{

    for (size_t i =0; i<cols; ++i){
       int y_shift = 35;

        createHexColumn(x+70*i,y+y_shift*i,rows);
    }
}

void HexBoard::createHexColumn(int x, int y, int RowsNum)
{
   // create a col of Hexes
    for(size_t i =0;i<RowsNum;++i){
        auto* hex = new Hex();

        hex->setPos(x,y+70*i);
        hexes.append(hex);
        game->scene->addItem(hex);
    }

}


