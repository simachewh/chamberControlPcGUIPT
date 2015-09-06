#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    communication = new Communication();

    initStyle();

    communication->prepCommunication();
    //!connection to update chamber's dry temperature change to GUI temperature lable!//
    connect(communication->chamberParams, SIGNAL(dryTemperatureChanged(QString)),
            ui->tempRealValueLabel, SLOT(setText(QString)));

    //!connection to update chamber's humidity change to GUI humidity lable!//
    connect(communication->chamberParams, SIGNAL(humidityChanged(QString)),
            ui->humidRealValueLabel, SLOT(setText(QString)));

    connect(communication->controlParams, SIGNAL(chPartChanged(bool,ControlPC::CH_PART)),
            this, SLOT(on_partsChanged(bool,ControlPC::CH_PART)));

    //! connection bettwen ControlPC teperature power change and temperature progress bar !//
    connect(communication->controlParams, SIGNAL(temperaturePowerChanged(int)),
            this, SLOT(on_tempPowerChange(int)));

    //! connection bettwen ControlPC humidity power change and humidity progress bar !//
    connect(communication->controlParams, SIGNAL(humidityPowerChanged(int)),
            this, SLOT(on_humidPowerChange(int)));


    communication->controlParams->setC2V2(true);
    communication->controlParams->setHumidityPower(220);
    communication->controlParams->setTemperaturePower(150);

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
    this->setWindowTitle("Climate Chamber - Monitor");

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
    this->setWindowTitle("Climate Chamber - Program Set");

    ui->monitorButton->setEnabled(true);
    ui->auxButton->setEnabled(true);
    ui->helpButton->setEnabled(true);

    populateProgramsList();
    this->communication->controlParams->setC2V2(!communication->controlParams->getC2V2()); //this is for demonestration
}

/**
 * @brief MainWindow::on_auxButton_clicked slot.
 * Update the stacked widget view to aux data perspective.
 */
void MainWindow::on_auxButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(AUX_INDEX);
    ui->auxButton->setEnabled(false);
    this->setWindowTitle("Climate Chamber - Aux Data");

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
    this->setWindowTitle("Climate Chamber - Help");

    ui->monitorButton->setEnabled(true);
    ui->programButton->setEnabled(true);
    ui->auxButton->setEnabled(true);
}


//! ****************** PUBLIC SLOTS *************** !//

void MainWindow::populateProgramsList(){
    QFileSystemModel *programsListModel = new QFileSystemModel();
    programsListModel->setFilter(QDir::Files);

    ui->programsListView->setModel(programsListModel);
    ui->programsListView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->programsListView->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->programsListView->setRootIndex(programsListModel->index(DataBackup::PROGRAMS_DIR_PATH));
    programsListModel->setRootPath(DataBackup::PROGRAMS_DIR_PATH);

    //trying to select the first item by default, not working correctly
    //ui->programsListView->setSelection(new QItemSelectionModel(programsListModel));
    ui->programsListView->setCurrentIndex(programsListModel->index(0,0));
}

void MainWindow::initStyle(){
    ui->monitorButton->clicked(true);
    ui->loadProgramButton->setEnabled(false);

    ui->tableSpliter->setStretchFactor(0, 1);
    ui->tableSpliter->setStretchFactor(1, 3);

    ui->stepsTableView->setAlternatingRowColors(true);
}

void MainWindow::on_newProgramButton_clicked()
{
    AddProgram *ap = new AddProgram();
    ap->setAttribute(Qt::WA_DeleteOnClose);
    ap->setModal(true);
    ap->show();
}

void MainWindow::on_programsListView_clicked(const QModelIndex &index)
{
    QString pgmName(index.data().toString());
    pgmName = pgmName.left(pgmName.indexOf('.'));

    DataBackup *db = new DataBackup();
    Program *prgmToDisplay = new Program();
    db->loadTestProgram(pgmName, prgmToDisplay);
    StepsModel *stepModel = new StepsModel();
    stepModel->setProgramToShow(prgmToDisplay);

    ui->stepsTableView->setModel(stepModel);
    if(!ui->loadProgramButton->isEnabled()){
        ui->loadProgramButton->setEnabled(true);
    }
    qDebug() << "on_programsListView_clicked: Row" << index.row() << index.column();
}

void MainWindow::on_loadProgramButton_clicked()
{
    ///try to set the test program by loadint it from file
    /// and show a dialogue that allows edditing and startitng the test program.
    Program *prepProgram = new Program();
    QModelIndexList selectedIndeces = ui->programsListView->selectionModel()->selectedIndexes();
    QModelIndex index = selectedIndeces.at(0);
    QString pgmName(index.data().toString());
    pgmName = pgmName.left(pgmName.indexOf('.'));
    DataBackup *db = new DataBackup();
//    db->loadTestProgram(pgmName, communication->controlParams->testProgram);
    db->loadTestProgram(pgmName, prepProgram);
    LoadProgram *lp = new LoadProgram();
    lp->setLoaded(prepProgram);
    lp->setModal(true);
    lp->show();
}

void MainWindow::on_partsChanged(bool value, ControlPC::CH_PART part)
{
    switch (part) {
    case ControlPC::H1:
        if(value){
            ui->h1Label->setStyleSheet("background-color: #0ABAF5");
            ui->h1Label->update();
        }else{
            ui->h1Label->setStyleSheet("background-color: ");
            ui->h1Label->update();
        }
        break;
    case ControlPC::H2:
        if(value){
            ui->h2Label->setStyleSheet("background-color: #0ABAF5");
            ui->h2Label->update();
        }else{
            ui->h2Label->setStyleSheet("background-color: ");
            ui->h2Label->update();
        }
        break;
    case ControlPC::T1:
        if(value){
            ui->t1Label->setStyleSheet("background-color: #0ABAF5");
            ui->t1Label->update();
        }else{
            ui->t1Label->setStyleSheet("background-color: ");
            ui->t1Label->update();
        }
        break;
    case ControlPC::T2:
        if(value){
            ui->t2Label->setStyleSheet("background-color: #0ABAF5");
            ui->t2Label->update();
        }else{
            ui->t2Label->setStyleSheet("background-color: ");
            ui->t2Label->update();
        }
        break;
    case ControlPC::P1:
        if(value){
            ui->p1Label->setStyleSheet("background-color: #0ABAF5");
            ui->p1Label->update();
        }else{
            ui->p1Label->setStyleSheet("background-color: ");
            ui->p1Label->update();
        }
        break;
    case ControlPC::P2:
        if(value){
            ui->p2Label->setStyleSheet("background-color: #0ABAF5");
            ui->p2Label->update();
        }else{
            ui->p2Label->setStyleSheet("background-color: ");
            ui->p2Label->update();
        }
        break;
    case ControlPC::P3:
        if(value){
            ui->p3Label->setStyleSheet("background-color: #0ABAF5");
            ui->p3Label->update();
        }else{
            ui->p3Label->setStyleSheet("background-color: ");
            ui->p3Label->update();
        }
        break;
    case ControlPC::LNU:
        if(value){
            ui->lnvLabel->setStyleSheet("background-color: #0ABAF5");
            ui->lnvLabel->update();
        }else{
            ui->lnvLabel->setStyleSheet("background-color: ");
            ui->lnvLabel->update();
        }
        break;
    case ControlPC::C1:
        if(value){
            ui->c1Label->setStyleSheet("background-color: #0ABAF5");
            ui->c1Label->update();
        }else{
            ui->c1Label->setStyleSheet("background-color: ");
            ui->c1Label->update();
        }
        break;
    case ControlPC::C2V2:
        if(value){
            ui->c2Label->setStyleSheet("background-color: #0ABAF5");
            ui->v2Label->setStyleSheet("background-color: #0ABAF5");
            ui->c2Label->update();
            ui->v2Label->update();
        }else{
            ui->c2Label->setStyleSheet("background-color: ");
            ui->v2Label->setStyleSheet("background-color: ");
            ui->c2Label->update();
            ui->v2Label->update();
        }
        break;
    case ControlPC::V1:
        if(value){
            ui->v1Label->setStyleSheet("background-color: #0ABAF5");
            ui->v1Label->update();
        }else{

        }
        break;
    case ControlPC::V3:
        if(value){
            ui->v3Label->setStyleSheet("background-color: #0ABAF5");
            ui->v3Label->update();
        }else{
            ui->v3Label->setStyleSheet("background-color: ");
            ui->v3Label->update();
        }
        break;
    case ControlPC::V4:
        if(value){
            ui->v4Label->setStyleSheet("background-color: #0ABAF5");
            ui->v4Label->update();
        }else{
            ui->v4Label->setStyleSheet("background-color: ");
            ui->v4Label->update();
        }
        break;
    case ControlPC::FAN:
        if(value){
            ui->fn1Label->setStyleSheet("background-color: #0ABAF5");
            ui->fn1Label->update();
        }else{
            ui->fn1Label->setStyleSheet("background-color: ");
            ui->fn1Label->update();
        }
        break;
    default:
        break;
    }
}

void MainWindow::on_tempPowerChange(int value)
{
    int res = ((float)value / 255.0) * 100;
    ui->t1ProgressBar->setValue(res);
    ui->t2ProgressBar->setValue(res);
    ui->t1ProgressBar->update();
    ui->t2ProgressBar->update();
}

void MainWindow::on_humidPowerChange(int value)
{
    int res = ((float)value / 255) * 100;
    ui->h1ProgressBar->setValue(res);
    ui->h2ProgressBar->setValue(res);
    ui->h1ProgressBar->update();
    ui->h2ProgressBar->update();
}
