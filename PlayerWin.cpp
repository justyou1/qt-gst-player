#include "PlayerWin.h"
#include "ui_PlayerWin.h"

PlayerWin::PlayerWin(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PlayerWin)
{
    ui->setupUi(this);
    player.attach(getWid());
}

PlayerWin::~PlayerWin()
{
    delete ui;
}
WId PlayerWin::getWid(){
    return ui->widget->winId();
}

void PlayerWin::on_pushButton_clicked()
{
    player.play(ui->lineEdit->text().toUtf8().data());
}
