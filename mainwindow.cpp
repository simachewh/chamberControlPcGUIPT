#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "communication.h"
#include "controlpc.h"
#include "chamber.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->stackedWidget->setCurrentIndex(0);
    ui->monitorButton->setEnabled(false);

    communication = new Communication();
    controlerPc = new ControlPC();
    climateChamber = new Chamber();

    //! no programs are runing at start up !//
    ControlPC::isIdel = true;

    //!port is set up and open at startup !//
    communication->openPort();

    //!connecting new data arival to a method that update the ui !//
    connect(communication, SIGNAL(newData(QByteArray)), this, SLOT(on_newDataArived(QByteArray)));



//    if(controlerPc->isIdel){
//        forever{
//            communication->sendData(controlerPc->iyCommand());
//            communication->serial->waitForReadyRead(1000);
//            communication->sendData(controlerPc->aqCommand());
//            communication->serial->waitForReadyRead(1000);
//            communication->sendData(controlerPc->brCommand());
//            communication->serial->waitForReadyRead(1000);
//            communication->sendData(controlerPc->idelCommand());
//        }
//    }



}

MainWindow::~MainWindow()
{
    delete ui;
    delete communication;
    delete climateChamber;
}

void MainWindow::on_monitorButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->monitorButton->setEnabled(false);
    ui->titleLabel->setText("<h3>Monitor</h3>");

    ui->auxButton->setEnabled(true);
    ui->programButton->setEnabled(true);
    ui->helpButton->setEnabled(true);
}

void MainWindow::on_programButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->programButton->setEnabled(false);
    ui->titleLabel->setText("<h3>Program Set</h3>");

    ui->monitorButton->setEnabled(true);
    ui->auxButton->setEnabled(true);
    ui->helpButton->setEnabled(true);
}

void MainWindow::on_auxButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->auxButton->setEnabled(false);
    ui->titleLabel->setText("<h3>Aux Data</h3>");

    ui->monitorButton->setEnabled(true);
    ui->programButton->setEnabled(true);
    ui->helpButton->setEnabled(true);
}

void MainWindow::on_helpButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    ui->helpButton->setEnabled(false);
    ui->titleLabel->setText("<h3>Help</h3>");

    ui->monitorButton->setEnabled(true);
    ui->programButton->setEnabled(true);
    ui->auxButton->setEnabled(true);
}

void MainWindow::on_newDataArived(QByteArray data)
{
    QString str(data);
    QStringList list;
    QString temp, humid;

    if(data.size() > 22){
        list = str.split(" ");
        qDebug() << "here" << list[0] << "cut: " << list[0].left(3);

        if(list[0].left(3).endsWith('A', Qt::CaseInsensitive)){

            temp = list[0].mid(4,9);
            humid = list[2].left(4);
            this->ui->tempRealValueLabel->setText(temp);
            this->ui->humidRealValueLabel->setText(humid);
        }

    }
}
