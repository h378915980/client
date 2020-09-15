#include "widget.h"

#include <QApplication>
#include <QDebug>
#include "./tool/compress/include/quazip/JlCompress.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    Widget w;
//    w.show();
   // qDebug()<<JlCompress::compressDir("test.zip","test");
    JlCompress::extractDir("test.zip","test");
    return a.exec();
}
