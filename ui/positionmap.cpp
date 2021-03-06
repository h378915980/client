#include "positionmap.h"
#include "ui_positionmap.h"
#include <QSizePolicy>

#define POSITION_MAP_SIZE       256
#define POSITION_BDM_NUM        24
#define POSITION_DU_NUM         4
#define POSITION_CRYSTAL_SIZE   13

#define MAP_VALUE_THRESHOLD     50


QColor PositionLineColor(0,0,0);
QColor PositionPointColor(255, 0, 0);
QColor PositionBackgroudColor(255,255,255);

PositionMap::PositionMap(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PositionMap)
{
    ui->setupUi(this);
    posMap=new UPositionMap();

//    setTableWidgetProperty(ui->tableWidget_9);
//    ui->groupBox_2->setVisible(false);
//    ui->groupBox_4->setVisible(false);

    ui->groupBox_5->setVisible(false);


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
    posMap->ReadPositionMap("calibration/PositionMap/positionMap.dat");
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
    unsigned int*  map=posMap->GetPositionMap(10,1);

    unsigned char value=0;
    unsigned int  mapValue=0;

    for(int row=0;row<ui->tableWidget_9->rowCount();row++)
    {
        for(int col=0;col<ui->tableWidget_9->columnCount();col++)
        {
            value=table[row*ui->tableWidget_9->rowCount()+col];
            mapValue=map[row*ui->tableWidget_9->rowCount()+col];

            //show map point
            ui->tableWidget_9->item(row,col)->setBackground(QBrush(QColor( 0 ,0,mapValue)));
            //show border line
            if(row!=0 || col!=0)
            {
                if(value!=table[(row-1)*ui->tableWidget_9->rowCount()+col]
                        ||value!=table[row*ui->tableWidget_9->rowCount()+col-1])
                {
                    ui->tableWidget_9->item(row,col)->setBackground(QBrush(PositionLineColor));
                }
            }


            //show number
            //ui->tableWidget_9->item(row,col)->setText(QString::number(mapValue));
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
    showPositionTable();
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
            if(value > MAP_VALUE_THRESHOLD)
            {
                tableWidget->item(row,col)->setBackground(QBrush(PositionPointColor));
            }
            else
            {
                tableWidget->item(row,col)->setBackground(QBrush(PositionBackgroudColor));
            }
        }
    }

    //show mass point with red line
    MassPoint tMassPoint=massPoints[m_nBDMId*POSITION_DU_NUM+DUId];
    for(std::set<unsigned char>::iterator iterRow=tMassPoint.rowSet.begin();iterRow!=tMassPoint.rowSet.end();iterRow++)
    {
        for(int j=0;j<tableWidget->columnCount();j++)
        {
            tableWidget->item(*iterRow,j)->setBackground(QBrush(PositionLineColor));
        }
    }

    for(std::set<unsigned char>::iterator iterCol=tMassPoint.colSet.begin();iterCol!=tMassPoint.colSet.end();iterCol++)
    {
        for(int i=0;i<tableWidget->rowCount();i++)
        {
            tableWidget->item(i,*iterCol)->setBackground(QBrush(PositionLineColor));
        }
    }
}
//show specific DU's position table
void PositionMap::showSpecificPositionTable(QTableWidget *tableWidget, unsigned int DUId)
{

    unsigned int* map=posMap->GetPositionMap(m_nBDMId,DUId);
    unsigned char* table=posMap->GetPositionTable(m_nBDMId,DUId);

    unsigned char tableValue=0;
    unsigned int  mapValue=0;



    for(int row=0;row<tableWidget->rowCount();row++)
    {
        for(int col=0;col<tableWidget->columnCount();col++)
        {
            tableValue=table[row*tableWidget->rowCount()+col];
            mapValue=map[row*tableWidget->rowCount()+col];

            //show map point
            if(mapValue > MAP_VALUE_THRESHOLD)
            {
                tableWidget->item(row,col)->setBackground(QBrush(PositionPointColor));
            }
            else
            {
                tableWidget->item(row,col)->setBackground(QBrush(PositionBackgroudColor));
            }
            //tableWidget->item(row,col)->setBackground(QBrush(QColor( 0 ,0,mapValue)));

            //show line color
            if(tableValue!=table[(row-1)*tableWidget->rowCount()+col]
                    ||tableValue!=table[row*tableWidget->rowCount()+col-1])
            {
                tableWidget->item(row,col)->setBackground(QBrush(PositionLineColor));
            }
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



