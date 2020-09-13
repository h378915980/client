#include "positionmap.h"
#include "ui_positionmap.h"

PositionMap::PositionMap(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PositionMap)
{
    ui->setupUi(this);
    //ui->groupBox_2->setVisible(false);

    massPoint=new ConfirmMassPoint();
}

PositionMap::~PositionMap()
{
    delete ui;
}

void PositionMap::on_pushButton_5_clicked()
{
    massPoint->exec();
}
