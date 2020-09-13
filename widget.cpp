#include "widget.h"
#include "ui_widget.h"

#include "tool/inifile.h"
#include <qmessagebox.h>

using namespace  inifile;


#define MAX_BUFFER_SIZE  4096

#define RE_STARTTAG                     0xFF
#define RE_FILETYPE_MICH                0x20
#define RE_FILETYPE_CONFIG              0x21

#define RE_SHAKEHAND                    0x30
#define RE_SHAKEHAND_REPLY              0x31
#define RE_STRING_UPPER                 0x32
#define RE_STRING_UPPER_REPLY           0x33
#define RE_STRING_ADD                   0x34
#define RE_STRING_ADD_REPLY             0x35
#define RE_FILE_PREPARE                 0x36
#define RE_FILE_PREPARE_REPLY           0x37
#define RE_FILE_UPLOAD                  0x38
#define RE_FILE_UPLOAD_REPLY            0x39
#define RE_FILE_UPLOAD_FINISHED         0x40
#define RE_FILE_UPLOAD_FINISHED_REPLY   0x41
#define RE_QUERY_TASK                   0x42
#define RE_QUERY_TASK_REPLY             0x43



const char * RETcpIp="127.0.0.1";
const int RETcpPort=8025;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("client");

    mainCalibration=new MainCalibration();

    IniFile ini=IniFile();
    ini.Load("IpPortconfig.ini");
    std::string ip;
    ini.GetStringValue("setting","ip",&ip);
    int port;
    ini.GetIntValue("setting","port",&port);

    recvLen=procLen=0;
    recvBuf=new char[MAX_BUFFER_SIZE];
    sendBuf=new char[MAX_BUFFER_SIZE];
    body=new char[MAX_BUFFER_SIZE-sizeof(Header)];
    memset(recvBuf,0,MAX_BUFFER_SIZE);
    memset(sendBuf,0,MAX_BUFFER_SIZE);
    memset(&header,0,sizeof(Header));
    memset(body,0,MAX_BUFFER_SIZE-sizeof(Header));

    QObject::connect(this,SIGNAL(changeString(QString)),this,SLOT(showUpper(QString)));
    client_sockfd=new UTcpSocket ();

    if(client_sockfd->Connect(ip.c_str(),port)==0)
    {
        pthread_t tid;
        pthread_create(&tid,NULL,ExecRecvFromServer,(void*)this);
        usleep(200000);
        pthread_detach(tid);

        pthread_t tid2;
        pthread_create(&tid2,NULL,ExecQueryTask,(void*)this);
        usleep(200000);
        pthread_detach(tid2);

    }else{
        QMessageBox::information(this,"notice","connect server failed!");
    }
}

Widget::~Widget()
{
    delete ui;
}

//send  string to upper
void Widget::on_pushButton_clicked()
{
    if(ui->lineEdit->text()=="")
        return;

    std::string sss=ui->lineEdit->text().toStdString();
    const char* str=sss.c_str();
    int len=strlen(str);


    Header l_header;
    l_header.nTag=RE_STARTTAG;
    l_header.nDirection=0x00;
    l_header.nOperation=RE_STRING_UPPER;
    l_header.nBodySize=len;

    memset(sendBuf,0,MAX_BUFFER_SIZE);
    memcpy(sendBuf,&l_header,sizeof(l_header));
    memcpy(sendBuf+sizeof (l_header),str,l_header.nBodySize);

    client_sockfd->Send(sendBuf,sizeof (l_header)+l_header.nBodySize);
}


//send file
void Widget::on_pushButton_2_clicked()
{

    const char *michName="bed0-mich.dat";

    FILE *fp;
    if((fp=fopen("./bed0-mich.dat","rb"))==NULL){
        qDebug()<<"mich file open error !";
        return ;
    }
    fclose(fp);

    if((fp=fopen("./config.ini","rb"))==NULL){
        qDebug()<<"config file open error !";
        return;
    }
    fclose(fp);


    Header l_header;
    l_header.nTag=RE_STARTTAG;
    l_header.nDirection=0x00;
    l_header.nOperation=RE_FILE_PREPARE;
    l_header.nBodySize=strlen(michName);

    memset(sendBuf,0,MAX_BUFFER_SIZE);
    memcpy(sendBuf,&l_header,sizeof(l_header));
    memcpy(sendBuf+sizeof (l_header),michName,l_header.nBodySize);

    client_sockfd->Send(sendBuf,sizeof (l_header)+l_header.nBodySize);



//    pthread_t T_tid;
//    pthread_create(&T_tid,NULL,ExecUploadFile,(void*)this);
//    pthread_detach(T_tid);


}

void* Widget::ExecUploadFile(void *f_arg)
{
    unsigned int filefd=*(unsigned int*)f_arg;
    qDebug()<<filefd;

    IniFile ini=IniFile();
    ini.Load("IpPortconfig.ini");
    std::string ip;
    ini.GetStringValue("setting","ip",&ip);
    int port;
    ini.GetIntValue("setting","port",&port);

    UTcpSocket* file_socket=new UTcpSocket();
    if(file_socket->Connect(ip.c_str(),port)<0)
    {
        qDebug()<<QString("file socket connect error :")+strerror(errno);
        return (void*)1;
    }

    //open mich and config file
    FILE *fp;
    if((fp=fopen("./bed0-mich.dat","rb"))==NULL){
        qDebug()<<"mich file open error !";
        return (void*)1;
    }

    FILE *fp_config;
    if((fp_config=fopen("./config.ini","rb"))==NULL){
        qDebug()<<"config file open error !";
        return (void*)1;
    }

    int len;
    char sendBuf[MAX_BUFFER_SIZE];
    char body[MAX_BUFFER_SIZE-sizeof (Header)-sizeof(FileInfo)];
    bzero(sendBuf,MAX_BUFFER_SIZE);
    bzero(body,sizeof(body));

    qDebug()<<sizeof(body);

    Header l_header;
    FileInfo l_fileinfo;

    l_header.nTag=RE_STARTTAG;
    l_header.nDirection=0x00;
    l_header.nOperation=RE_FILE_UPLOAD;

    l_fileinfo.nFileFd=filefd;

    while(!feof(fp)){

        l_fileinfo.nType=RE_FILETYPE_MICH;

        memset(body,0,sizeof(body));
        len=fread(body,1,sizeof (body),fp);

        l_header.nBodySize=len+sizeof(l_fileinfo);

        qDebug()<<"fread mich len:"+QString::number(len)<<"  pkg"<<sizeof (l_header)+l_header.nBodySize;

        memset(sendBuf,0,MAX_BUFFER_SIZE);
        memcpy(sendBuf,&l_header,sizeof (l_header));
        memcpy(sendBuf+sizeof (l_header),&l_fileinfo,sizeof (l_fileinfo));
        memcpy(sendBuf+sizeof (l_header)+sizeof (l_fileinfo),body,len);

        file_socket->Send(sendBuf,sizeof (l_header)+l_header.nBodySize);
    }

    fclose(fp);

    qDebug()<<"begin process config file";
    while(!feof(fp_config)){

        l_fileinfo.nType=RE_FILETYPE_CONFIG;

        memset(body,0,sizeof(body));
        len=fread(body,1,sizeof (body),fp_config);

        l_header.nBodySize=len+sizeof(l_fileinfo);

        qDebug()<<"fread config len:"+QString::number(len)<<"  pkg"<<sizeof (l_header)+l_header.nBodySize;

        memset(sendBuf,0,MAX_BUFFER_SIZE);
        memcpy(sendBuf,&l_header,sizeof (l_header));
        memcpy(sendBuf+sizeof (l_header),&l_fileinfo,sizeof (l_fileinfo));
        memcpy(sendBuf+sizeof (l_header)+sizeof (l_fileinfo),body,len);

        file_socket->Send(sendBuf,sizeof (l_header)+l_header.nBodySize);
    }

    fclose(fp_config);




    l_header.nTag=RE_STARTTAG;
    l_header.nDirection=0x00;
    l_header.nOperation=RE_FILE_UPLOAD_FINISHED;
    l_header.nBodySize=sizeof (filefd);

    memset(sendBuf,0,MAX_BUFFER_SIZE);
    memcpy(sendBuf,&l_header,sizeof (l_header));
    memcpy(sendBuf+sizeof(l_header),&filefd,sizeof (filefd));

    file_socket->Send(sendBuf,sizeof (l_header)+l_header.nBodySize);


    file_socket->Close();

    return (void*)0;
}



void* Widget::ExecRecvFromServer(void *f_arg)
{
    Widget* l_ptr=(Widget*)f_arg;

    while(1){
        qDebug()<<"waitfor new ";
        memset(l_ptr->recvBuf,0,MAX_BUFFER_SIZE);
        int l_recvLen=l_ptr->client_sockfd->Recv(l_ptr->recvBuf,MAX_BUFFER_SIZE);
        if(l_recvLen<=0){
            qDebug()<<"server error!";
            continue;
        }

        l_ptr->recvLen+=l_recvLen;
        while(l_ptr->recvLen>=sizeof(Header)+l_ptr->procLen){
            memcpy(&l_ptr->header,l_ptr->recvBuf+l_ptr->procLen,sizeof(Header));
            if((l_ptr->recvLen - l_ptr->procLen ) < (sizeof(Header) + l_ptr->header.nBodySize))
            {
                qDebug()<<"pkg length is error";
                break;
            }

            memcpy(l_ptr->body,l_ptr->recvBuf+l_ptr->procLen+sizeof (Header),l_ptr->header.nBodySize);

            l_ptr->ProcessPkg(l_ptr->header,l_ptr->body);

            l_ptr->procLen+=sizeof (Header)+l_ptr->header.nBodySize;

            memset(&l_ptr->header,0,sizeof (Header));
            memset(l_ptr->body,0,MAX_BUFFER_SIZE-sizeof (Header));

        }

        memset(l_ptr->recvBuf,0,MAX_BUFFER_SIZE);
        l_ptr->recvLen=l_ptr->procLen=0;
        continue;
    }
    return (void*)0;
}


void Widget::ProcessPkg(const Header &f_header, char *f_body)
{
    if(f_header.nTag!=RE_STARTTAG || f_header.nDirection!= 0x01){
        qDebug()<<"invalid header";
        return;
    }

    switch (f_header.nOperation) {
    case RE_STRING_UPPER_REPLY:
    {
        qDebug()<<"recv  upper  string ";

        QString sss(f_body);
        qDebug()<<sss;
        changeString(sss);

    }
    break;


    case RE_SHAKEHAND_REPLY:
    {
        qDebug()<<"recv  shake hand reply";

    }
    break;

    case RE_STRING_ADD_REPLY:
    {
        qDebug()<<"recv add suffix reply";
        QString sss(f_body);
        changeString(sss);

    }
    break;

    case RE_FILE_PREPARE_REPLY:
    {
        qDebug()<<"server prepare,please upload your file";
        unsigned int filefd;
        memcpy(&filefd,f_body,f_header.nBodySize);

        qDebug()<<filefd;

        pthread_t T_tid;
        pthread_create(&T_tid,NULL,ExecUploadFile,(void*)&filefd);
        usleep(200000);
        pthread_detach(T_tid);

    }
    break;

    case RE_FILE_UPLOAD_FINISHED_REPLY:
    {
        qDebug()<<"server recv your file";
    }
    break;

    case RE_QUERY_TASK_REPLY:
    {
        qDebug()<<"query task reply";
        int taskNum;
        memcpy(&taskNum,f_body,sizeof(int));

        qDebug()<<"get"<<taskNum<<"Tasks";
        if(taskNum==0)
        {
            ui->label_2->setText("non task");
        }
        else
        {
            ui->label_2->setText("task info as follows");
            ui->tableWidget->setRowCount(taskNum);
            for(int i=0;i<taskNum;i++)
            {
                TaskInfo taskInfo;
                memcpy(&taskInfo,f_body+sizeof(int)+i*sizeof(TaskInfo),sizeof(TaskInfo));

                QString state;
                switch(taskInfo.nState)
                {
                case TASK_ERROR:
                    state="ERROR";
                    break;
                case TASK_READY:
                    state="Ready";
                    break;
                case TASK_PROCESSING:
                    state="Processing";
                    break;
                case TASK_FINISHED:
                    state="Finished";
                    break;
                default:
                    state="Unknow";
                    break;
                }

                qDebug()<<taskInfo.nId<<taskInfo.nName<<taskInfo.nStep<<state;

                ui->tableWidget->setItem(i,0,new QTableWidgetItem(QString::number(taskInfo.nId)));
                ui->tableWidget->setItem(i,1,new QTableWidgetItem(taskInfo.nName));
                ui->tableWidget->setItem(i,2,new QTableWidgetItem(QString::number(taskInfo.nStep)));
                ui->tableWidget->setItem(i,3,new QTableWidgetItem(state));
            }
        }

    }
    break;

    default:
        qDebug()<<"invailed operation";
        return;
    }

    return;

}




void Widget::showUpper(QString str)
{
    ui->lineEdit_2->setText(str);
}



//add suffix
void Widget::on_pushButton_5_clicked()
{
    if(ui->lineEdit_3->text()=="")
        return;

    std::string sss=ui->lineEdit_3->text().toStdString();
    const char* str=sss.c_str();
    int len=strlen(str);


    Header l_header;
    l_header.nTag=RE_STARTTAG;
    l_header.nDirection=0x00;
    l_header.nOperation=RE_STRING_ADD;
    l_header.nBodySize=len;

    memset(sendBuf,0,MAX_BUFFER_SIZE);
    memcpy(sendBuf,&l_header,sizeof(l_header));
    memcpy(sendBuf+sizeof (l_header),str,l_header.nBodySize);

    client_sockfd->Send(sendBuf,sizeof (l_header)+l_header.nBodySize);
}

//shake hand
void Widget::on_pushButton_4_clicked()
{
    Header l_header;
    l_header.nTag=RE_STARTTAG;
    l_header.nDirection=0x00;
    l_header.nOperation=RE_SHAKEHAND;
    l_header.nBodySize=0;

    memset(sendBuf,0,MAX_BUFFER_SIZE);
    memcpy(sendBuf,&l_header,sizeof(l_header));

    client_sockfd->Send(sendBuf,sizeof (l_header)+l_header.nBodySize);
}

void* Widget::ExecQueryTask(void *f_arg)
{
    Widget* l_ptr=(Widget*)f_arg;

    char* sendBuf=new char[MAX_BUFFER_SIZE];
    memset(sendBuf,0,MAX_BUFFER_SIZE);

    Header l_header;
    l_header.nTag=RE_STARTTAG;
    l_header.nDirection=0x00;
    l_header.nOperation=RE_QUERY_TASK;
    l_header.nBodySize=0;

    memset(sendBuf,0,MAX_BUFFER_SIZE);
    memcpy(sendBuf,&l_header,sizeof(l_header));

    while(1){

//        if(true)
//        {
//            sleep(1);
//            continue;
//        }
        qDebug()<<"widget: exec query task";
        l_ptr->client_sockfd->Send(sendBuf,sizeof (l_header)+l_header.nBodySize);
        sleep(1);
    }

    return (void*)0;
}



void Widget::on_pushButton_3_clicked()
{
    mainCalibration->show();
}
