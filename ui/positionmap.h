#ifndef POSITIONMAP_H
#define POSITIONMAP_H

#include <QWidget>
#include <QDebug>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include "confirmmasspoint.h"
#include "./calibration/position/UPositionMap.h"
namespace Ui {
class PositionMap;
}

typedef struct _MassPoint
{
    set<unsigned char> rowSet;
    set<unsigned char> colSet;
}MassPoint;

enum DoneCode{
    SAVE=1000,
    SAVE_ALL
};

class PositionMap : public QWidget
{
    Q_OBJECT

public:
    explicit PositionMap(QWidget *parent = nullptr);
    ~PositionMap();

private slots:

    /**
     * @brief on_pushButton_9_clicked -- open the file manager window and choose file
     */
    void on_pushButton_9_clicked();

    /**
     * @brief on_pushButton_clicked -- analyse input file and get position map
     */
    void on_pushButton_clicked();

    /**
     * @brief on_pushButton_4_clicked -- begin to create position table and show
     */
    void on_pushButton_4_clicked();

    /**
     * @brief on_pushButton_2_clicked -- get previous bdm id
     */
    void on_pushButton_2_clicked();

    /**
     * @brief on_pushButton_3_clicked -- get next bdm id
     */
    void on_pushButton_3_clicked();

    /**
     * @brief on_tableWidget_cellClicked -- show mass point dialog about 0 DU
     * @param row
     * @param column
     */
    void on_tableWidget_cellClicked(int row, int column);
    /**
     * @brief on_tableWidget_2_cellClicked -- show mass point dialog about 1 DU
     * @param row
     * @param column
     */
    void on_tableWidget_2_cellClicked(int row, int column);
    /**
     * @brief on_tableWidget_3_cellClicked -- show mass point dialog about 2 DU
     * @param row
     * @param column
     */
    void on_tableWidget_3_cellClicked(int row, int column);
    /**
     * @brief on_tableWidget_4_cellClicked -- show mass point dialog about 3 DU
     * @param row
     * @param column
     */
    void on_tableWidget_4_cellClicked(int row, int column);

    void on_pushButton_6_clicked();

private:
    Ui::PositionMap *ui;
    UPositionMap* posMap;

    unsigned int m_nBDMId=0;


    MassPoint massPoints[24*4];  //BDM_NUM * DU_NUM

    /**
     * @brief setTableWidgetProperty -- set property of table widget
     * @param tableWidget
     */
    void setTableWidgetProperty(QTableWidget* tableWidget);

    /**
     * @brief initUI -- init ui
     */
    void initUI();

    /**
     * @brief showSpecificPositionMap -- show Position map by specific DUId and table widget
     * @param tableWidget
     * @param DUId -- DU id
     */
    void showSpecificPositionMap(QTableWidget* tableWidget,unsigned int DUId);
    /**
     * @brief showPositionMap -- show 0~3 DU's position map
     */
    void showPositionMap();

    void showSpecificPositionTable(QTableWidget* tableWidget,unsigned int DUId);

    void showPositionTable();

    void clearMassPoints();

    void showMassPointDialog(unsigned int DUId);
};

#endif // POSITIONMAP_H
