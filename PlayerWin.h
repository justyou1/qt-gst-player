#ifndef PLAYERWIN_H
#define PLAYERWIN_H

#include <QMainWindow>
#include "player.h"
QT_BEGIN_NAMESPACE
namespace Ui { class PlayerWin; }
QT_END_NAMESPACE

class PlayerWin : public QMainWindow
{
    Q_OBJECT
    Player player;
public:
    PlayerWin(QWidget *parent = nullptr);
    ~PlayerWin();
    WId getWid();
private slots:
    void on_pushButton_clicked();

private:
    Ui::PlayerWin *ui;
};
#endif // PLAYERWIN_H
