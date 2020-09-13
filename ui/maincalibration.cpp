#include "maincalibration.h"
#include "ui_maincalibration.h"


QWidget* preWidget;

MainCalibration::MainCalibration(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainCalibration)
{
    ui->setupUi(this);
    positionMap=new PositionMap();
    energyCalibration=new EnergyCalibration();


}

MainCalibration::~MainCalibration()
{
    delete ui;
}

void MainCalibration::on_pushButton_clicked()
{
    ui->lineEdit->setText(QString::number(num++));
}

//position map
void MainCalibration::on_pushButton_2_clicked()
{
    removePreWidget();
    positionMap->show();
    ui->verticalLayout_2->addWidget(positionMap);
    preWidget = positionMap;
}
//energy calibration
void MainCalibration::on_pushButton_3_clicked()
{
    removePreWidget();
    energyCalibration->show();
    ui->verticalLayout_2->addWidget(energyCalibration);
    preWidget = energyCalibration;
}

void MainCalibration::removePreWidget()
{
    if (preWidget != NULL) {
        ui->verticalLayout_2->removeWidget(preWidget);
        preWidget->hide();
        //delete preWidget;
    }
}
