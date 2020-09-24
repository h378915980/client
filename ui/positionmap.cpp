#include "positionmap.h"
#include "ui_positionmap.h"
#include <QSizePolicy>

#define POSITION_MAP_SIZE       256
#define POSITION_BDM_NUM        24
#define POSITION_DU_NUM         4
#define POSITION_CRYSTAL_SIZE   13

const unsigned char ColorR[13]={255,200,240,161,102,200,94,77,60,92,199,150};
const unsigned char ColorG[13]={100,150,240,226,170,250,162,138,87,68,9,0};

PositionMap::PositionMap(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PositionMap)
{
    ui->setupUi(this);
    ui->groupBox_5->setVisible(false);
    //setTableWidgetProperty(ui->tableWidget_9);

    posMap=new UPositionMap();


//    ui->groupBox_2->setVisible(false);
//    ui->groupBox_4->setVisible(false);

    initUI();
    clearMassPoints();

    showPositionMap();
    showPositionTable();
}

PositionMap::~PositionMap()
{
    delete ui;
}

//test table button
void PositionMap::on_pushButton_6_clicked()
{
    qDebug()<<"test table button clicked";
    posMap->ReadPositionTable("calibration/PositionMap/positionTable.dat");
    //showPositionTable();

    for(int col=0;col<ui->tableWidget_9->columnCount();col++)
    {
        for(int row=0;row<ui->tableWidget_9->rowCount();row++)
        {
            ui->tableWidget_9->setItem(row,col,new QTableWidgetItem(0));
        }
    }
    unsigned char* table=posMap->GetPositionTable(10,1);

    unsigned char value=0;
    unsigned char cur[255];
    unsigned char topValue[255];
    std::set<unsigned char> type;
    for(int row=0;row<ui->tableWidget_9->rowCount();row++)
    {
        for(int col=0;col<ui->tableWidget_9->columnCount();col++)
        {
            value=table[row*ui->tableWidget_9->rowCount()+col];
            //show color
            //变色思路：
            //发现：现在数字是从小往大依次递增的，必然一行对应13个
            //
            if(row==0)
            {
                topValue[col]=value;
                cur[col]=0;
            }
            else
            {
                if(value!=topValue[col])
                {
                    topValue[col]=value;
                    cur[col]++;
                }
            }

            ui->tableWidget_9->item(row,col)->setBackground(QBrush(
                QColor( ColorR[(int)(value/POSITION_CRYSTAL_SIZE)] ,
                        ColorG[(int)(value/POSITION_CRYSTAL_SIZE)],
                        100)));

            //show number
            //ui->tableWidget_9->item(row,col)->setText(QString::number(value));
        }
    }

}



void PositionMap::initUI()
{
    ui->spinBox_5->setValue(m_nBDMId);
    ui->spinBox_5->setEnabled(false);

    //table widget to show position map
    setTableWidgetProperty(ui->tableWidget);
    setTableWidgetProperty(ui->tableWidget_2);
    setTableWidgetProperty(ui->tableWidget_3);
    setTableWidgetProperty(ui->tableWidget_4);

    //table widget to show position table
    setTableWidgetProperty(ui->tableWidget_5);
    setTableWidgetProperty(ui->tableWidget_6);
    setTableWidgetProperty(ui->tableWidget_7);
    setTableWidgetProperty(ui->tableWidget_8);
}

void PositionMap::setTableWidgetProperty(QTableWidget *tableWidget)
{


    tableWidget->setRowCount(POSITION_MAP_SIZE);
    tableWidget->setColumnCount(POSITION_MAP_SIZE);

    for(int col=0;col<tableWidget->columnCount();col++)
    {
        for(int row=0;row<tableWidget->rowCount();row++)
        {
            tableWidget->setItem(row,col,new QTableWidgetItem(0));
        }
    }


    tableWidget->horizontalHeader()->setVisible(false);
    tableWidget->verticalHeader()->setVisible(false);

    tableWidget->horizontalHeader()->setDefaultSectionSize(1);
    tableWidget->horizontalHeader()->setMinimumSectionSize(1);
    tableWidget->verticalHeader()->setDefaultSectionSize(1);
    tableWidget->verticalHeader()->setMinimumSectionSize(1);

    tableWidget->setShowGrid(false);

    tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);


    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    tableWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    tableWidget->setMinimumSize(260,260);
}

//open dir
void PositionMap::on_pushButton_9_clicked()
{
    QString dirPath=QFileDialog::getExistingDirectory();

    if(dirPath=="")
    {
        qDebug()<<"PositionMap: input file path is null";
        return;
    }

    qDebug()<<dirPath;
    ui->lineEdit->setText(dirPath);

}

//analysis
void PositionMap::on_pushButton_clicked()
{
    if(ui->lineEdit->text()=="")
    {
        QMessageBox::information(this,"notice","Please input file first!");
        return ;
    }

    //TODO:if input file error
    if(false)
    {
        QMessageBox::information(this,"notice","Error input file!");
        return ;
    }


    //success and clear previous data
    clearMassPoints();
    posMap->clear();
    showPositionMap();
    showPositionTable();

    //read and create
    std::string dirPath=ui->lineEdit->text().toStdString();
    //notice the sample file name is changed
    posMap->CreatePositionMap(dirPath);
    posMap->SavePositionMap("calibration/PositionMap/positionMap.dat");

    //show the 0~3 DU's Position Map of current BDM on table widget
    showPositionMap();

}

//begin button Position Table
void PositionMap::on_pushButton_4_clicked()
{
    //detect if  all DU's mass point is setted
    for(int i=0;i<POSITION_BDM_NUM * POSITION_DU_NUM;i++)
    {
        if(massPoints[i].rowSet.size()!=POSITION_CRYSTAL_SIZE || massPoints[i].colSet.size()!=POSITION_CRYSTAL_SIZE)
        {
            QString str="mass point is incomplete: check  BDM:"+QString::number((int)(i/POSITION_DU_NUM))+"  DU:"+QString::number(i%POSITION_DU_NUM);
            QMessageBox::information(this,"notice",str);
            return;
        }
    }

    //create positoin table
    unsigned int BDMId=0;
    unsigned int DUId=0;
    for(int i=0;i<POSITION_BDM_NUM * POSITION_DU_NUM;i++)
    {
        BDMId=(unsigned int)(i/POSITION_DU_NUM);
        DUId=(unsigned int)i%POSITION_DU_NUM;
        posMap->SetMassPoint(BDMId,DUId,massPoints[i].rowSet,massPoints[i].colSet);
    }

    posMap->CreatePositionTable();
    posMap->SavePositionTable("calibration/PositionMap/positionTable.dat");


    //show position table
    showPositionTable();
}

//get previous bdm id
void PositionMap::on_pushButton_2_clicked()
{
    if(m_nBDMId==0)
    {
        m_nBDMId=23;
    }
    else
    {
        m_nBDMId--;
    }

    ui->spinBox_5->setValue(m_nBDMId);

    showPositionMap();
    showPositionTable();
}
//get next bdm id
void PositionMap::on_pushButton_3_clicked()
{
    if(m_nBDMId==23)
    {
        m_nBDMId=0;
    }
    else
    {
        m_nBDMId++;
    }

    ui->spinBox_5->setValue(m_nBDMId);

    showPositionMap();
    showPositionTable();
}
//show specific DU's position map
void PositionMap::showSpecificPositionMap(QTableWidget *tableWidget, unsigned int DUId)
{
    unsigned int* map=posMap->GetPositionMap(m_nBDMId,DUId);
    int value = 0;
    for(int row=0;row<tableWidget->rowCount();row++)
    {
        for(int col=0;col<tableWidget->columnCount();col++)
        {
            value=map[row*tableWidget->rowCount()+col];
            tableWidget->item(row,col)->setBackground(QBrush(QColor(0 , 0, value)));
        }
    }

    //show mass point with red line
    MassPoint tMassPoint=massPoints[m_nBDMId*POSITION_DU_NUM+DUId];
    for(std::set<unsigned char>::iterator iterRow=tMassPoint.rowSet.begin();iterRow!=tMassPoint.rowSet.end();iterRow++)
    {
        for(int j=0;j<tableWidget->columnCount();j++)
        {
            tableWidget->item(*iterRow,j)->setBackground(QBrush(QColor(255, 0, 0)));
        }
    }

    for(std::set<unsigned char>::iterator iterCol=tMassPoint.colSet.begin();iterCol!=tMassPoint.colSet.end();iterCol++)
    {
        for(int i=0;i<tableWidget->rowCount();i++)
        {
            tableWidget->item(i,*iterCol)->setBackground(QBrush(QColor(255, 0, 0)));
        }
    }
}
//show specific DU's position table
void PositionMap::showSpecificPositionTable(QTableWidget *tableWidget, unsigned int DUId)
{
    unsigned char* table=posMap->GetPositionTable(m_nBDMId,DUId);
    unsigned char value=0;
    for(int row=0;row<tableWidget->rowCount();row++)
    {
        for(int col=0;col<tableWidget->columnCount();col++)
        {
            value=table[row*tableWidget->rowCount()+col];
            //show color
            tableWidget->item(row,col)->setBackground(QBrush(
                        QColor( ColorR[(int)(value/POSITION_CRYSTAL_SIZE)] ,
                                ColorG[(int)(value/POSITION_CRYSTAL_SIZE)],
                                value)));
            //show number
            //tableWidget->item(row,col)->setText(QString::number(value));
        }
    }

    //show mass point with red line
    MassPoint tMassPoint=massPoints[m_nBDMId*POSITION_DU_NUM+DUId];
    for(std::set<unsigned char>::iterator iterRow=tMassPoint.rowSet.begin();iterRow!=tMassPoint.rowSet.end();iterRow++)
    {
        for(int j=0;j<tableWidget->columnCount();j++)
        {
            tableWidget->item(*iterRow,j)->setBackground(QBrush(QColor(255, 0, 0)));
        }
    }

    for(std::set<unsigned char>::iterator iterCol=tMassPoint.colSet.begin();iterCol!=tMassPoint.colSet.end();iterCol++)
    {
        for(int i=0;i<tableWidget->rowCount();i++)
        {
            tableWidget->item(i,*iterCol)->setBackground(QBrush(QColor(255, 0, 0)));
        }
    }

}
//show all position map with specific BDM
void PositionMap::showPositionMap()
{
    showSpecificPositionMap(ui->tableWidget,0);
    showSpecificPositionMap(ui->tableWidget_2,1);
    showSpecificPositionMap(ui->tableWidget_3,2);
    showSpecificPositionMap(ui->tableWidget_4,3);
}
//show all position table with specific BDM
void PositionMap::showPositionTable()
{
    showSpecificPositionTable(ui->tableWidget_5,0);
    showSpecificPositionTable(ui->tableWidget_6,1);
    showSpecificPositionTable(ui->tableWidget_7,2);
    showSpecificPositionTable(ui->tableWidget_8,3);
}
//clear mass points
void PositionMap::clearMassPoints()
{
    //clear
    for(int i=0;i< POSITION_BDM_NUM * POSITION_DU_NUM;i++)
    {
        massPoints[i].colSet.clear();
        massPoints[i].rowSet.clear();
    }
}

//open mass point dialog and show DU position map and mark mass point
//0 DU
void PositionMap::on_tableWidget_cellClicked(int row, int column)
{
    showMassPointDialog(0);
}
//1 DU
void PositionMap::on_tableWidget_2_cellClicked(int row, int column)
{
    showMassPointDialog(1);
}
//2 DU
void PositionMap::on_tableWidget_3_cellClicked(int row, int column)
{
    showMassPointDialog(2);
}

//3 DU
void PositionMap::on_tableWidget_4_cellClicked(int row, int column)
{
    showMassPointDialog(3);
}

void PositionMap::showMassPointDialog(unsigned int DUId)
{
    ConfirmMassPoint* massPointDialog = new ConfirmMassPoint();

    //set position map
    massPointDialog->setPositionMap(posMap->GetPositionMap(m_nBDMId,DUId));
    //set mass point
    massPointDialog->setMassPoint(massPoints[m_nBDMId*POSITION_DU_NUM + DUId].rowSet,massPoints[m_nBDMId*POSITION_DU_NUM + DUId].colSet);
    //show position map and mass point
    massPointDialog->showPositionMapOnTable();

    switch (massPointDialog->exec()) {
    case DoneCode::SAVE:
    {
        qDebug()<<"save the bdm:"<<m_nBDMId<<" DU:"<<DUId<<" mass point";
        massPoints[m_nBDMId*POSITION_DU_NUM + DUId].rowSet=massPointDialog->getRowSetOfMassPoint();
        massPoints[m_nBDMId*POSITION_DU_NUM + DUId].colSet=massPointDialog->getColSetOfMassPoint();

        showPositionMap();
    }
        break;
    case DoneCode::SAVE_ALL:
    {
        qDebug()<<"save the bdm:"<<m_nBDMId<<" DU:"<<DUId<<" mass point to all !!!";
        for(int i=0;i<POSITION_BDM_NUM * POSITION_DU_NUM;i++)
        {
            massPoints[i].rowSet=massPointDialog->getRowSetOfMassPoint();
            massPoints[i].colSet=massPointDialog->getColSetOfMassPoint();
        }

        showPositionMap();
    }
        break;

    default:
        break;
    }

}



