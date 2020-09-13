#include "confirmmasspoint.h"
#include "ui_confirmmasspoint.h"

ConfirmMassPoint::ConfirmMassPoint(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfirmMassPoint)
{
    ui->setupUi(this);
}

ConfirmMassPoint::~ConfirmMassPoint()
{
    delete ui;
}
