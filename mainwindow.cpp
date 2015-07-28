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
    communication = new Communication();
    ap = new AddProgram();

    ui->stackedWidget->setCurrentIndex(0);
    ui->monitorButton->setEnabled(false);


    communication->prepCommunication();
    //!connection to update chamber's dry temperature change to GUI temperature lable!//
    connect(communication->chamberParams, SIGNAL(dryTemperatureChanged(QString)),
            ui->tempRealValueLabel, SLOT(setText(QString)));

    //!connection to update chamber's humidity change to GUI humidity lable!//
    connect(communication->chamberParams, SIGNAL(humidityChanged(QString)),
            ui->humidRealValueLabel, SLOT(setText(QString)));

}

MainWindow::~MainWindow()
{
    delete ui;
    delete communication;
}

/**
 * @brief MainWindow::on_monitorButton_clicked slot.
 * Updates the sacked widet view to monitors perspective.
 */
void MainWindow::on_monitorButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(MONITOR_INDEX);
    ui->monitorButton->setEnabled(false);
    ui->titleLabel->setText("<h3>Monitor</h3>");

    ui->auxButton->setEnabled(true);
    ui->programButton->setEnabled(true);
    ui->helpButton->setEnabled(true);
}

/**
 * @brief MainWindow::on_programButton_clicked slot.
 * Updates the stacked widget view to program set perspective.
 */
void MainWindow::on_programButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(PROGRAM_INDEX);
    ui->programButton->setEnabled(false);
    ui->titleLabel->setText("<h3>Program Set</h3>");

    ui->monitorButton->setEnabled(true);
    ui->auxButton->setEnabled(true);
    ui->helpButton->setEnabled(true);

    populateProgramsList();
}

/**
 * @brief MainWindow::on_auxButton_clicked slot.
 * Update the stacked widget view to aux data perspective.
 */
void MainWindow::on_auxButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(AUX_INDEX);
    ui->auxButton->setEnabled(false);
    ui->titleLabel->setText("<h3>Aux Data</h3>");

    ui->monitorButton->setEnabled(true);
    ui->programButton->setEnabled(true);
    ui->helpButton->setEnabled(true);
}

/**
 * @brief MainWindow::on_helpButton_clicked slot.
 * Updates the stacked widget view to help's perspective.
 */
void MainWindow::on_helpButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(HELP_INDEX);
    ui->helpButton->setEnabled(false);
    ui->titleLabel->setText("<h3>Help</h3>");

    ui->monitorButton->setEnabled(true);
    ui->programButton->setEnabled(true);
    ui->auxButton->setEnabled(true);
}


void MainWindow::on_newProgramButton_clicked()
{
    ap->show();
}

//! ****************** PUBLIC SLOTS *************** !//

void MainWindow::populateProgramsList(){
    QFileSystemModel *programsListModel = new QFileSystemModel();
    programsListModel->setFilter(QDir::Files);
    ui->programsTableView->setModel(programsListModel);

    ui->programsTableView->setAlternatingRowColors(true);
    ui->programsTableView->setColumnHidden(1, true);
    ui->programsTableView->setColumnHidden(2, true);


    //programsListModel
    programsListModel->setRootPath(DataBackup::PROGRAMS_DIR_PATH);

    ui->programsTableView->setRootIndex(programsListModel->index(DataBackup::PROGRAMS_DIR_PATH));



}
