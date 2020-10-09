#ifndef ENERGYCALIBRATION_H
#define ENERGYCALIBRATION_H

#include <QWidget>
#include <qwt.h>
#include <qwt_plot.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_layout.h>
#include <qwt_symbol.h>
#include <qwt_plot_item.h>
#include <qwt_plot_scaleitem.h>
#include <qwt_plot_marker.h>
#include <QFileDialog>
#include <QMessageBox>
#include <QTableWidget>
#include "./calibration/energy/UEnergyProfile.h"

namespace Ui {
class EnergyCalibration;
}

class EnergyCalibration : public QWidget
{
    Q_OBJECT

public:
    explicit EnergyCalibration(QWidget *parent = nullptr);
    ~EnergyCalibration();

private slots:
    void on_pushButton_10_clicked();

    void on_pushButton_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_tableWidget_cellClicked(int row, int column);

private:
    Ui::EnergyCalibration *ui;
    UEnergyProfile* energyProfile;

    unsigned int m_nBDMId=0;
    unsigned int m_nDUId=0;

    void setTableWidgetProperty(QTableWidget *tableWidget);

    void setPlotProperty(QwtPlot* qwtPlot,QVector<QPointF> &data);

    void showEnergyProfile();

};

#endif // ENERGYCALIBRATION_H
