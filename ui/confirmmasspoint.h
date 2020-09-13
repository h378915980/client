#ifndef CONFIRMMASSPOINT_H
#define CONFIRMMASSPOINT_H

#include <QDialog>

namespace Ui {
class ConfirmMassPoint;
}

class ConfirmMassPoint : public QDialog
{
    Q_OBJECT

public:
    explicit ConfirmMassPoint(QWidget *parent = nullptr);
    ~ConfirmMassPoint();

private:
    Ui::ConfirmMassPoint *ui;
};

#endif // CONFIRMMASSPOINT_H
