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
    commun = new Communication();
    commun->openPort();
    connect(commun, SIGNAL(newData(QByteArray)), this, SLOT(on_newDataArived(QByteArray)));

    controler = new ControlPC();

    commun->sendData(controler->idelCommand());

}

MainWindow::~MainWindow()
{
    delete ui;
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
    qDebug() << "Data" << data;
}
