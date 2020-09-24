#include "confirmmasspoint.h"
#include "ui_confirmmasspoint.h"
#include "positionmap.h"

#define OFFSET              2
#define POSITION_MAP_SIZE   256
#define MASS_POINT_SIZE     13

ConfirmMassPoint::ConfirmMassPoint(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfirmMassPoint)
{
    ui->setupUi(this);

    this->rowSet.clear();
    this->colSet.clear();

    initTableWiget();
}

ConfirmMassPoint::~ConfirmMassPoint()
{
    delete ui;
}

void ConfirmMassPoint::initTableWiget()
{
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableWidget->setRowCount(POSITION_MAP_SIZE);
    ui->tableWidget->setColumnCount(POSITION_MAP_SIZE);

    //qDebug()<<ui->tableWidget->rowCount()<<"  "<<ui->tableWidget->columnCount();

    for(int col=0;col<ui->tableWidget->columnCount();col++)
    {
        for(int row=0;row<ui->tableWidget->rowCount();row++)
        {
            ui->tableWidget->setItem(row,col,new QTableWidgetItem(0));
        }
    }

    ui->tableWidget->horizontalHeader()->setVisible(false);
    ui->tableWidget->verticalHeader()->setVisible(false);

    ui->tableWidget->horizontalHeader()->setDefaultSectionSize(3);
    ui->tableWidget->horizontalHeader()->setMinimumSectionSize(3);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(3);
    ui->tableWidget->verticalHeader()->setMinimumSectionSize(3);

    ui->tableWidget->setShowGrid(false);

    ui->tableWidget->setMinimumSize(800,800);

    if(markDirection==DirectionType::ROW)
    {
        ui->pushButton->setText("row");
    }
    else
    {
        ui->pushButton->setText("column");
    }
}

//change mark direction
void ConfirmMassPoint::on_pushButton_clicked()
{
    if(markDirection==DirectionType::ROW)
    {
        ui->pushButton->setText("column");
        markDirection=DirectionType::COLUMN;
    }
    else
    {
        ui->pushButton->setText("row");
        markDirection=DirectionType::ROW;
    }
}

//refresh
void ConfirmMassPoint::on_pushButton_4_clicked()
{

    this->rowSet.clear();
    this->colSet.clear();

    showPositionMapOnTable();
}

//mark
void ConfirmMassPoint::on_tableWidget_cellClicked(int row, int column)
{
    qDebug()<< "selected  row:"<<row<<" column:"<<column;

    //identify direction
    if(markDirection==DirectionType::ROW)
    {
        bool isInRowSet=false;
        int  posInSet=row-OFFSET;

        //is selected item  in the row set
        for(;posInSet<=row+OFFSET;posInSet++)
        {
            if(rowSet.find(posInSet)!=rowSet.end())
            {
                isInRowSet=true;
                break;
            }
        }

        if(isInRowSet==false)
        {
            //not in the row set, mark
            for(int i=0;i<ui->tableWidget->columnCount();i++)
            {
                ui->tableWidget->item(row,i)->setBackground(QBrush(QColor(255, 0, 0)));
                rowSet.insert(row);
            }
        }
        else
        {
            //in the row set, unmark
            unsigned int value=0;
            for(int j=0;j<ui->tableWidget->columnCount();j++)
            {
                value=specPosMap[posInSet*ui->tableWidget->columnCount()+j];
                ui->tableWidget->item(posInSet,j)->setBackground(QBrush(QColor(0 , 0, value)));
            }

            //erase this row pos in row set
            rowSet.erase(posInSet);
        }


    }
    else
    {
        bool isInColSet=false;
        int posInSet=column-OFFSET;

        //is selected item  in the col set
        for(;posInSet<=column+OFFSET;posInSet++)
        {
            if(colSet.find(posInSet)!=colSet.end())
            {
                isInColSet=true;
                break;
            }
        }

        if(isInColSet==false)
        {
            //not in the col set, mark
            for(int i=0;i<ui->tableWidget->rowCount();i++)
            {
                ui->tableWidget->item(i,column)->setBackground(QBrush(QColor(255, 0, 0)));
                colSet.insert(column);
            }
        }
        else
        {
            //in the col set, unmark
            unsigned int value=0;
            for(int j=0;j<ui->tableWidget->rowCount();j++)
            {
                value=specPosMap[j*ui->tableWidget->columnCount()+posInSet];
                ui->tableWidget->item(j,posInSet)->setBackground(QBrush(QColor(0 , 0, value)));
            }

            //erase this col pos in col set
            colSet.erase(posInSet);
        }
    }
}


void ConfirmMassPoint::setPositionMap(unsigned int *specData)
{
    specPosMap=specData;

    //you need set position map and mass point first ,and then use showPositionMapOnTable by PositionMap.cpp
    //showPositionMapOnTable();
}

void ConfirmMassPoint::setMassPoint(std::set<unsigned char> rowSet, std::set<unsigned char> colSet)
{
    this->rowSet=rowSet;
    this->colSet=colSet;

}



void ConfirmMassPoint::showPositionMapOnTable()
{
    int value = 0;
    for(int row=0;row<ui->tableWidget->rowCount();row++)
    {
        for(int col=0;col<ui->tableWidget->columnCount();col++)
        {
            value=specPosMap[row*ui->tableWidget->rowCount()+col];
            //qDebug()<<value;
            //如何将value映射到0-255范围内?目前来看直接显示是可以的
            ui->tableWidget->item(row,col)->setBackground(QBrush(QColor(0 , 0, value)));
        }
    }

    //show mass point with red line
    for(std::set<unsigned char>::iterator iterRow=rowSet.begin();iterRow!=rowSet.end();iterRow++)
    {
        for(int j=0;j<ui->tableWidget->columnCount();j++)
        {
            ui->tableWidget->item(*iterRow,j)->setBackground(QBrush(QColor(255, 0, 0)));
        }
    }

    for(std::set<unsigned char>::iterator iterCol=colSet.begin();iterCol!=colSet.end();iterCol++)
    {
        for(int i=0;i<ui->tableWidget->rowCount();i++)
        {
            ui->tableWidget->item(i,*iterCol)->setBackground(QBrush(QColor(255, 0, 0)));
        }
    }

}

//save
void ConfirmMassPoint::on_pushButton_3_clicked()
{
    if(rowSet.size()!=MASS_POINT_SIZE || colSet.size()!=MASS_POINT_SIZE)
    {
        QMessageBox::information(this,"notice","the row or column size of mass point is not 13");
        return;
    }

    this->done(DoneCode::SAVE);
}

//save all
void ConfirmMassPoint::on_pushButton_2_clicked()
{

    if(rowSet.size()!=MASS_POINT_SIZE || colSet.size()!=MASS_POINT_SIZE)
    {
        QMessageBox::information(this,"notice","the row or column size of mass point is not 13");
        return;
    }
    this->done(DoneCode::SAVE_ALL);
}

std::set<unsigned char> ConfirmMassPoint::getRowSetOfMassPoint()
{
    return this->rowSet;
}

std::set<unsigned char> ConfirmMassPoint::getColSetOfMassPoint()
{
    return this->colSet;
}
