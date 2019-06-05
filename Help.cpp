#include "Help.h"
#include "ui_Help.h"

Help::Help(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Help)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
}

Help::~Help()
{
    delete ui;
}

void Help::on_pushButton_clicked()
{
    hide();
}
