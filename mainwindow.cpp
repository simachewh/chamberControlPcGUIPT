#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    communication = new Communication();

    initStyle();

    //!connection to update chamber's dry temperature change to
    //! GUI temperature lable!//
    connect(communication->pidController->chamberParams,
            SIGNAL(dryTemperatureChanged(QString)),
            ui->tempRealValueLabel, SLOT(setText(QString)));

    //!connection to update chamber's humidity change to GUI humidity lable!//
    connect(communication->pidController->chamberParams,
            SIGNAL(humidityChanged(QString)),
            ui->humidRealValueLabel, SLOT(setText(QString)));

    connect(communication->pidController->controlCommands,
            SIGNAL(chPartChanged(bool,ControlCommands::CH_PART)),
            this, SLOT(on_partsChanged(bool,ControlCommands::CH_PART)));

    //! connection bettwen ControlPC teperature power change and
    //! temperature progress bar !//
    connect(communication->pidController->controlCommands,
            SIGNAL(temperaturePowerChanged(int)),
            this, SLOT(on_tempPowerChange(int)));

    //! connection bettwen ControlPC humidity power change and
    //! humidity progress bar !//
    connect(communication->pidController->controlCommands,
            SIGNAL(humidityPowerChanged(int)),
            this, SLOT(on_humidPowerChange(int)));

    //! program connections to gui !//
    connect(communication->pidController->testPgm,
            SIGNAL(programNameChanged(QString)),
            this, SLOT(on_testProgramNameChanged(QString)));
    connect(communication->pidController->testPgm,
            SIGNAL(programParamChanged(int,Program::PGM_PARAM)),
            this, SLOT(on_testProgramParamChanged(int,Program::PGM_PARAM)));

//    communication->pidController->controlCommands->setC2V2(true);
//    communication->pidController->controlCommands->setHumidityPower(220);
//    communication->pidController->controlCommands->setTemperaturePower(150);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete communication;
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
    ui->tabWidget->setCurrentIndex(MONITOR_INDEX);
    ui->loadProgramButton->setEnabled(false);
    ui->renameProgramButton->setEnabled(false);
    ui->deleteProgramButton->setEnabled(false);
    ui->startStopButton->setEnabled(false);

    ui->tableSpliter->setStretchFactor(0, 1);
    ui->tableSpliter->setStretchFactor(1, 3);

    ui->stepsTableView->setAlternatingRowColors(true);
}

void MainWindow::on_newProgramButton_clicked()
{
    AddProgram *ap = new AddProgram();
    ap->setAttribute(Qt::WA_DeleteOnClose); // so all objects created with it will also be deleted
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
    if(!ui->renameProgramButton->isEnabled()){
        ui->renameProgramButton->setEnabled(true);
    }
    if(!ui->deleteProgramButton->isEnabled()){
        ui->deleteProgramButton->setEnabled(true);
    }
    if(!ui->startStopButton->isEnabled()){
        ui->startStopButton->setEnabled(true);
    }
    qDebug() << "on_programsListView_clicked: Row" << index.row() << index.column();
}

void MainWindow::on_loadProgramButton_clicked()
{
    ///try to set the test program by loadint it from file
    /// and show a dialogue that allows edditing and startitng the test program.
    Program *prepProgram = new Program();
//    QModelIndexList selectedIndeces = ui->programsListView->selectionModel()->selectedIndexes();
//    QModelIndex index = selectedIndeces.at(0);
//    QString pgmName(index.data().toString());
//    pgmName = pgmName.left(pgmName.indexOf('.'));
    QString pgmName(ui->programsListView->currentIndex().data().toString());
    pgmName = pgmName.left(pgmName.indexOf('.'));
    DataBackup *db = new DataBackup();
//    db->loadTestProgram(pgmName, communication->controlParams->testProgram);
    db->loadTestProgram(pgmName, prepProgram);
    LoadProgram *lp = new LoadProgram();
    lp->setLoaded(prepProgram);
    lp->setModal(true);
    lp->show();
}

void MainWindow::on_partsChanged(bool value, ControlCommands::CH_PART part)
{
    switch (part) {
    case ControlCommands::H1:
        if(value){
            ui->h1Label->setStyleSheet("background-color: #0ABAF5");
            ui->h1Label->update();
        }else{
            ui->h1Label->setStyleSheet("background-color: ");
            ui->h1Label->update();
        }
        break;
    case ControlCommands::H2:
        if(value){
            ui->h2Label->setStyleSheet("background-color: #0ABAF5");
            ui->h2Label->update();
        }else{
            ui->h2Label->setStyleSheet("background-color: ");
            ui->h2Label->update();
        }
        break;
    case ControlCommands::T1:
        if(value){
            ui->t1Label->setStyleSheet("background-color: #0ABAF5");
            ui->t1Label->update();
        }else{
            ui->t1Label->setStyleSheet("background-color: ");
            ui->t1Label->update();
        }
        break;
    case ControlCommands::T2:
        if(value){
            ui->t2Label->setStyleSheet("background-color: #0ABAF5");
            ui->t2Label->update();
        }else{
            ui->t2Label->setStyleSheet("background-color: ");
            ui->t2Label->update();
        }
        break;
    case ControlCommands::P1:
        if(value){
            ui->p1Label->setStyleSheet("background-color: #0ABAF5");
            ui->p1Label->update();
        }else{
            ui->p1Label->setStyleSheet("background-color: ");
            ui->p1Label->update();
        }
        break;
    case ControlCommands::P2:
        if(value){
            ui->p2Label->setStyleSheet("background-color: #0ABAF5");
            ui->p2Label->update();
        }else{
            ui->p2Label->setStyleSheet("background-color: ");
            ui->p2Label->update();
        }
        break;
    case ControlCommands::P3:
        if(value){
            ui->p3Label->setStyleSheet("background-color: #0ABAF5");
            ui->p3Label->update();
        }else{
            ui->p3Label->setStyleSheet("background-color: ");
            ui->p3Label->update();
        }
        break;
    case ControlCommands::LNU:
        if(value){
            ui->lnvLabel->setStyleSheet("background-color: #0ABAF5");
            ui->lnvLabel->update();
        }else{
            ui->lnvLabel->setStyleSheet("background-color: ");
            ui->lnvLabel->update();
        }
        break;
    case ControlCommands::C1:
        if(value){
            ui->c1Label->setStyleSheet("background-color: #0ABAF5");
            ui->c1Label->update();
        }else{
            ui->c1Label->setStyleSheet("background-color: ");
            ui->c1Label->update();
        }
        break;
    case ControlCommands::C2V2:
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
    case ControlCommands::V1:
        if(value){
            ui->v1Label->setStyleSheet("background-color: #0ABAF5");
            ui->v1Label->update();
        }else{

        }
        break;
    case ControlCommands::V3:
        if(value){
            ui->v3Label->setStyleSheet("background-color: #0ABAF5");
            ui->v3Label->update();
        }else{
            ui->v3Label->setStyleSheet("background-color: ");
            ui->v3Label->update();
        }
        break;
    case ControlCommands::V4:
        if(value){
            ui->v4Label->setStyleSheet("background-color: #0ABAF5");
            ui->v4Label->update();
        }else{
            ui->v4Label->setStyleSheet("background-color: ");
            ui->v4Label->update();
        }
        break;
    case ControlCommands::FAN:
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

void MainWindow::on_testProgramNameChanged(QString value)
{
    ui->progNameLabel->setText("Pgm. Name: " + value);
}

void MainWindow::on_testProgramParamChanged(int value, Program::PGM_PARAM param)
{
    switch (param) {
    case Program::Cycl:
    {
        QString lbl("Cycle %1 of %2");
        ui->cycleLabel->setText(lbl.
                                arg(communication->pidController->testPgm->getCurrentCycle()).
                                arg(value));
    }
        break;
    case Program::Curr_cycl:
    {
        QString lbl("Cycle %1 of %2");
        ui->cycleLabel->setText(lbl.
                                arg(value).
                                arg(communication->pidController->testPgm->getCycle()));
    }

        break;
    case Program::Stp:
    {
        QString lbl("Step %1 of %2");
        ui->stepLabel->setText(lbl.
                               arg(communication->pidController->testPgm->getCurrentStep()).
                               arg(value));
    }
        break;
    case Program::Curr_stp:
    {
         QString lbl("Step %1 of %2");
         ui->stepLabel->setText(lbl.
                                arg(value).
                                arg(communication->pidController->testPgm->getNoOfSteps()));
    }
        break;
    default:
        break;
    }
}


void MainWindow::on_humidPowerChange(int value)
{
    int res = ((float)value / 255) * 100;
    ui->h1ProgressBar->setValue(res);
    ui->h2ProgressBar->setValue(res);
    ui->h1ProgressBar->update();
    ui->h2ProgressBar->update();
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    if(index == HELP_INDEX){
        this->setWindowTitle("Climate Chamber - Help");
    }else if(index == PROGRAM_INDEX){
        this->setWindowTitle("Climate Chamber - Program");
        ui->loadProgramButton->setEnabled(false);
        ui->renameProgramButton->setEnabled(false);
        ui->deleteProgramButton->setEnabled(false);
        ui->startStopButton->setEnabled(false);
        populateProgramsList();
//        this->communication->pidController->controlCommands->
//                setC2V2(!communication->pidController->controlCommands->getC2V2()); //this is for demonestration
    }else if(index == MONITOR_INDEX){
        this->setWindowTitle("Climate Chamber - Monitor");
    }else if(index == AUX_INDEX){
        this->setWindowTitle("Climate Chamber - AUX Data");
    }
}

void MainWindow::on_removeStepFromSelectedButton_clicked()
{
    QString pgmName(ui->programsListView->currentIndex().data().toString());
    pgmName = pgmName.left(pgmName.indexOf('.'));

}

void MainWindow::on_addStepOnSelectedButton_clicked()
{

}

void MainWindow::on_renameProgramButton_clicked()
{
    QString pgmName(ui->programsListView->currentIndex().data().toString());
    pgmName = pgmName.left(pgmName.indexOf('.'));
    RenameDialog *rd = new RenameDialog();
    rd->setMessage("Renaming the program " + pgmName);
    rd->setOldName(pgmName);
    rd->setModal(true);
    rd->show();
}

void MainWindow::on_deleteProgramButton_clicked()
{
    QString pgmName(ui->programsListView->currentIndex().data().toString());
    pgmName = pgmName.left(pgmName.indexOf('.'));
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Warning", "Are you sure you want to dlete program "
                                  + pgmName, QMessageBox::Yes | QMessageBox::No,
                                  QMessageBox::No);
    if(reply == QMessageBox::No){
        return;
    }
    DataBackup db;
    bool removed = db.removeProgram(pgmName);
    if(removed){
        QMessageBox::information(this, "Success", "Program " + pgmName + "is removed.",
                                 QMessageBox::Ok, QMessageBox::NoButton);
    }else{
        QMessageBox::information(this, "Failure", "Removing program " + pgmName + "failed.",
                                 QMessageBox::Ok, QMessageBox::NoButton);
    }
}

void MainWindow::on_startStopButton_clicked()
{
    QString pgmName(ui->programsListView->currentIndex().data().toString());
    pgmName = pgmName.left(pgmName.indexOf('.'));
    QMessageBox::StandardButton reply =
            QMessageBox::question(this, "Confirm", "Start runing program "
                                  + pgmName, QMessageBox::Yes | QMessageBox::No,
                                  QMessageBox::No);
    if(reply == QMessageBox::No){
        return;
    }else{
        DataBackup db;
        db.loadTestProgram(pgmName, communication->pidController->testPgm);
        communication->pidController->controlCommands->setIdle(false);
        communication->pidController->startTest();
    }

}
