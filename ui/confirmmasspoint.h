#ifndef CONFIRMMASSPOINT_H
#define CONFIRMMASSPOINT_H

#include <QDialog>
#include <QDebug>
#include <set>
namespace Ui {
class ConfirmMassPoint;
}


enum DirectionType
{
    ROW=0,
    COLUMN
};

class ConfirmMassPoint : public QDialog
{
    Q_OBJECT

public:
    explicit ConfirmMassPoint(QWidget *parent = nullptr);
    ~ConfirmMassPoint();

    /**
     * @brief setPositionMap -- set and show position map from specific BDM and DU id
     * @param specData -- specific data
     */
    void setPositionMap(unsigned int* specData);

    void setMassPoint(std::set<unsigned char> rowSet,std::set<unsigned char> colSet);

    std::set<unsigned char> getRowSetOfMassPoint();
    std::set<unsigned char> getColSetOfMassPoint();

    void showPositionMapOnTable();

private slots:
    /**
     * @brief on_pushButton_clicked -- change the direction of marking
     */
    void on_pushButton_clicked();

    /**
     * @brief on_pushButton_4_clicked -- refresh position map
     */
    void on_pushButton_4_clicked();

    /**
     * @brief on_tableWidget_cellClicked -- mark or unmark the mass point
     * @param row -- selected row of the table widget
     * @param column -- selected column of the table widget
     */
    void on_tableWidget_cellClicked(int row, int column);

    /**
     * @brief on_pushButton_3_clicked -- save the position of mass point
     */
    void on_pushButton_3_clicked();

    /**
     * @brief on_pushButton_2_clicked -- save the position of mass point to all DU
     */
    void on_pushButton_2_clicked();

private:
    Ui::ConfirmMassPoint *ui;


    std::set<unsigned char> rowSet;
    std::set<unsigned char> colSet;

    /**
     * @brief markDirection-- 0 is row , 1 is column
     */
    char            markDirection = DirectionType::ROW;
    /**
     * @brief specPosMap -- specific BDM and DU's PositionMap
     */
    unsigned int*   specPosMap;

    /**
     * @brief rowSet -- save the position of mass point which is selectd by user
     */


    void initTableWiget();


};

#endif // CONFIRMMASSPOINT_H
