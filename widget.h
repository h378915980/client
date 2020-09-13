#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include "utcpsocket.h"
#include "./ui/maincalibration.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE



enum TaskState
{
    TASK_ERROR=0,
    TASK_READY,
    TASK_PROCESSING,
    TASK_PAUSE,
    TASK_FINISHED
};



class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

#pragma pack(push,1)
    typedef struct _Header{
        unsigned char nTag;
        unsigned char nDirection;
        unsigned char nOperation;
        unsigned int  nBodySize;
    }Header;

    typedef struct _FileInfo{
        unsigned char nType;
        unsigned int  nFileFd;
    } FileInfo;

    /*task info*/
    typedef struct _TaskInfo{
        unsigned long   nId;
        char            nName[40];
        double          nStep;
        TaskState      nState;
    }TaskInfo;
#pragma pack(pop)




signals:
    void changeString(QString);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void showUpper(QString);


    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::Widget *ui;

    MainCalibration* mainCalibration;

    char* recvBuf;
    char* sendBuf;
    UTcpSocket* client_sockfd;

    unsigned int recvLen;
    unsigned int procLen;

    Header header;
    char*  body;



    static void* ExecRecvFromServer(void* f_arg);
    void ProcessPkg(const Header& f_header,char* f_body);

    static void* ExecUploadFile(void* f_arg);

    static void* ExecQueryTask(void* f_arg);

};
#endif // WIDGET_H
