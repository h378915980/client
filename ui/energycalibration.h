#ifndef ENERGYCALIBRATION_H
#define ENERGYCALIBRATION_H

#include <QWidget>

namespace Ui {
class EnergyCalibration;
}

class EnergyCalibration : public QWidget
{
    Q_OBJECT

public:
    explicit EnergyCalibration(QWidget *parent = nullptr);
    ~EnergyCalibration();

private:
    Ui::EnergyCalibration *ui;
};

#endif // ENERGYCALIBRATION_H
