#include "PlayerWin.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PlayerWin w;
    w.show();
    return a.exec();
}

