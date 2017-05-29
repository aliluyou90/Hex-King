#ifndef HEXBOARD_H
#define HEXBOARD_H
#include <QList>
#include "hex.h"

class HexBoard
{
public:
    HexBoard();
    QList<Hex*> getHexes();
    // place hexec
    void placeHex(int x, int y, int cols, int rows);
    QList<Hex*> hexes;
private:

    void createHexColumn(int x, int y, int RowsNum);
};

#endif // HEXBOARD_H
