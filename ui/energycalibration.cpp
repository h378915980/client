#include "energycalibration.h"
#include "ui_energycalibration.h"

EnergyCalibration::EnergyCalibration(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EnergyCalibration)
{
    ui->setupUi(this);
}

EnergyCalibration::~EnergyCalibration()
{
    delete ui;
}
