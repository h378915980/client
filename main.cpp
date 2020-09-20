#include "widget.h"

#include <QApplication>
#include <QDebug>
#include <set>
#include "./calibration/position/UPositionMap.h"
#include "./ui/positionmap.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();


    return a.exec();
}
