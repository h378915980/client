#ifndef POSITIONMAP_H
#define POSITIONMAP_H

#include <QWidget>
#include "confirmmasspoint.h"
namespace Ui {
class PositionMap;
}

class PositionMap : public QWidget
{
    Q_OBJECT

public:
    explicit PositionMap(QWidget *parent = nullptr);
    ~PositionMap();

private slots:
    void on_pushButton_5_clicked();

private:
    Ui::PositionMap *ui;
    ConfirmMassPoint* massPoint;

};

#endif // POSITIONMAP_H
