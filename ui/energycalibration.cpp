#include "energycalibration.h"
#include "ui_energycalibration.h"

EnergyCalibration::EnergyCalibration(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EnergyCalibration)
{
    ui->setupUi(this);
    energyProfile=new UEnergyProfile();


}

EnergyCalibration::~EnergyCalibration()
{
    delete ui;
}

//open button:sample files
void EnergyCalibration::on_pushButton_10_clicked()
{
    QString dirPath=QFileDialog::getExistingDirectory();

    if(dirPath=="")
    {
        qDebug()<<"EnergyProfile: input file path is null";
        return;
    }

    qDebug()<<dirPath;
    ui->lineEdit->setText(dirPath);
}

//open button : positonTable.dat
void EnergyCalibration::on_pushButton_13_clicked()
{
    QString dirPath=QFileDialog::getOpenFileName(this);

    if(dirPath=="")
    {
        qDebug()<<"EnergyProfile: input file path is null";
        return;
    }

    qDebug()<<dirPath;
    ui->lineEdit_3->setText(dirPath);
}

//analysis
void EnergyCalibration::on_pushButton_clicked()
{

    if(ui->lineEdit->text()=="" || ui->lineEdit_3->text()=="")
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

    energyProfile->Clear();

    //create energy profile with sample files
    std::string samplePath=ui->lineEdit->text().toStdString();
    std::string positionTablePath=ui->lineEdit_3->text().toStdString();


    //you need to notify the path of positionTable.dat also
    energyProfile->CreateEnergyProfile(samplePath,positionTablePath);
    energyProfile->SaveEnergyProfile("calibration/EnergyCalibration/energyProfile.dat");

    showEnergyProfile();
}

void EnergyCalibration::setPlotProperty(QwtPlot *qwtPlot, QVector<QPointF> &data)
{

    qwtPlot->setMinimumSize(100,100);

    //canvas
    QwtPlotCanvas *canvas=new QwtPlotCanvas();
    canvas->setPalette(Qt::white);
    qwtPlot->setCanvas(canvas);
    qwtPlot->plotLayout()->setAlignCanvasToScales(true);

    //axis
//    qwtPlot->setAxisTitle(QwtPlot::yLeft,"count");
//    qwtPlot->setAxisTitle(QwtPlot::xBottom,"energy");
    qwtPlot->setAxisScale(QwtPlot::yLeft,0.0,100.0);
    qwtPlot->setAxisScale(QwtPlot::xBottom,0.0,1000.0);
    qwtPlot->setAxisMaxMinor(QwtPlot::xBottom,1);
    qwtPlot->setAxisMaxMinor(QwtPlot::yLeft,2);
    qwtPlot->enableAxis(QwtPlot::yLeft,false);
    qwtPlot->enableAxis(QwtPlot::xBottom,false);

    //curve
    QwtPlotCurve* curve=new QwtPlotCurve();
    curve->setPen(Qt::blue,0.1);
    curve->setRenderHint(QwtPlotItem::RenderAntialiased,true);

    //add point
    curve->setSamples(data);
    //show
    curve->attach(qwtPlot);
}

void EnergyCalibration::setTableWidgetProperty(QTableWidget *tableWidget)
{


    tableWidget->setRowCount(13);
    tableWidget->setColumnCount(13);

    for(int col=0;col<tableWidget->columnCount();col++)
    {
        for(int row=0;row<tableWidget->rowCount();row++)
        {
            tableWidget->setItem(row,col,new QTableWidgetItem(0));
        }
    }


    tableWidget->horizontalHeader()->setVisible(false);
    tableWidget->verticalHeader()->setVisible(false);

    tableWidget->horizontalHeader()->setDefaultSectionSize(300);
    tableWidget->horizontalHeader()->setMinimumSectionSize(100);
    tableWidget->verticalHeader()->setDefaultSectionSize(300);
    tableWidget->verticalHeader()->setMinimumSectionSize(100);

    tableWidget->setShowGrid(false);

    tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);


    //tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    tableWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    tableWidget->setMinimumSize(260,260);
}

void EnergyCalibration::setPlotCurve(QwtPlot *qwtPlot, QVector<QPointF> &data)
{
    QwtPlotItemList itemLists=qwtPlot->itemList(QwtPlotItem::Rtti_PlotCurve);
    if(itemLists.size()==1)
    {
        static_cast<QwtPlotCurve*>(itemLists[0])->setSamples(data);
    }
    else
    {
        qDebug()<<"EnergyCalibration: set plot curve error";
    }

}
void EnergyCalibration::showEnergyProfile()
{
    unsigned int* eneProfile=nullptr;

    for(int i=0;i<13*13;i++)
    {
        eneProfile=energyProfile->GetEnergyProfile(m_nBDMId,m_nDUId,i);

        QVector<QPointF> data;
        for(int j=0;j<1000;j++)
        {
            QPointF point;
            point.setX(j);
            point.setY(eneProfile[j]);
            data.push_back(point);

        }

        QwtPlot* plot=new QwtPlot();
        setPlotProperty(plot,data);

        //if have a widget ,replace and delete previous widget
        ui->gridLayout->addWidget(plot,i/13,i%13);
        //QwtPlot* item=(QwtPlot*)ui->gridLayout->itemAtPosition(i/13,i%13)->widget();
        qDebug()<<ui->gridLayout->itemAtPosition(i/13,i%13)->widget()->objectName();
    }
}

//test button
void EnergyCalibration::on_pushButton_11_clicked()
{
    energyProfile->ReadEnergyProfile("calibration/EnergyCalibration/energyProfile.dat");
    showEnergyProfile();
}

//save energy corr factor
void EnergyCalibration::on_pushButton_3_clicked()
{
    energyProfile->CreateEnergyCorrFactor();
    energyProfile->SaveEnergyCorrFactor("calibration/EnergyCalibration/energyCorrFactor.dat");
}

//bdm  pre
void EnergyCalibration::on_pushButton_4_clicked()
{
    if(m_nBDMId==0)
    {
        m_nBDMId=23;
    }
    else
    {
        m_nBDMId--;
    }

    ui->spinBox->setValue(m_nBDMId);
    showEnergyProfile();
}
//bdm next
void EnergyCalibration::on_pushButton_5_clicked()
{
    if(m_nBDMId==23)
    {
        m_nBDMId=0;
    }
    else
    {
        m_nBDMId++;
    }

    ui->spinBox->setValue(m_nBDMId);
    showEnergyProfile();
}
//du pre
void EnergyCalibration::on_pushButton_6_clicked()
{
    if(m_nDUId==0)
    {
        m_nDUId=3;
    }
    else
    {
        m_nDUId--;
    }

    ui->spinBox_2->setValue(m_nDUId);
    showEnergyProfile();
}
//du next
void EnergyCalibration::on_pushButton_7_clicked()
{
    if(m_nDUId==3)
    {
        m_nDUId=0;
    }
    else
    {
        m_nDUId++;
    }

    ui->spinBox_2->setValue(m_nDUId);
    showEnergyProfile();
}
