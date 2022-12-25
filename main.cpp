#include "board.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Board board;
    board.show();

    return a.exec();
}
