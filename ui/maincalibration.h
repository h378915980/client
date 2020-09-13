#ifndef MAINCALIBRATION_H
#define MAINCALIBRATION_H

#include <QWidget>
#include "positionmap.h"
#include "energycalibration.h"
namespace Ui {
class MainCalibration;
}

class MainCalibration : public QWidget
{
    Q_OBJECT

public:
    explicit MainCalibration(QWidget *parent = nullptr);
    ~MainCalibration();

private slots:
    /*Test Function*/
    void on_pushButton_clicked();

    /*Turn to positon map and table*/
    void on_pushButton_2_clicked();

    /*Turn to Energy calibration*/
    void on_pushButton_3_clicked();

private:
    Ui::MainCalibration *ui;
    PositionMap* positionMap;
    EnergyCalibration* energyCalibration;


    int num=1;

    void removePreWidget();

};

#endif // MAINCALIBRATION_H
