#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QCoreApplication::setOrganizationName("technobothnia");
    QCoreApplication::setOrganizationDomain("simachew.com");
    QCoreApplication::setApplicationName("climate_chamber_controller");
    ui->setupUi(this);    
    communication = new Communication();

    //no need to reserve memory for a quick program, it can be collected
    //from the quick steps view table's model.
    //quickPgm = new Program(parent);
    initStyle();

    /// Internal connections ///
    connect(this, SIGNAL(quickStepAboutToAdd()),
            this, SLOT(on_quickStepAboutToAdd()));

    //!communication object signals connection to this
    connect(communication, SIGNAL(connectionLost(bool)),
            this, SLOT(on_connectionLost(bool)));

    //!chamber connection to gui!//
    connect(communication->pidController->chamberParams,
            SIGNAL(dryTemperatureChanged(QString)),
            ui->tempRealValueLabel, SLOT(setText(QString)));

    connect(communication->pidController->chamberParams,
            SIGNAL(humidityChanged(QString)),
            ui->humidRealValueLabel, SLOT(setText(QString)));

    //! controlCommands connection to gui !//
    connect(communication->pidController->controlCommands,
            SIGNAL(chPartChanged(bool,ControlCommands::CH_PART)),
            this, SLOT(on_partsChanged(bool,ControlCommands::CH_PART)));

    connect(communication->pidController->controlCommands,
            SIGNAL(temperaturePowerChanged(int)),
            this, SLOT(on_tempPowerChange(int)));

    connect(communication->pidController->controlCommands,
            SIGNAL(humidityPowerChanged(int)),
            this, SLOT(on_humidPowerChange(int)));

    connect(communication->pidController->controlCommands, SIGNAL(idleStateChanged(bool)),
            this, SLOT(on_idleStateChanged(bool)));

    //! program connections to gui !//
    connect(communication->pidController->testPgm,
            SIGNAL(programNameChanged(QString)),
            this, SLOT(on_testProgramNameChanged(QString)));
    connect(communication->pidController->testPgm,
            SIGNAL(programParamChanged(int,Program::PGM_PARAM)),
            this, SLOT(on_testProgramParamChanged(int,Program::PGM_PARAM)));

    //! controller connection to gui !//
    connect(communication->pidController, SIGNAL(stepsDone(bool)),
            this, SLOT(on_testFinished()));

    connect(communication->pidController, SIGNAL(stepsDone(bool)),
            this->ui->stopButton, SLOT(setDisabled(bool)));

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
    if(!ui->startButton->isEnabled()){
        ui->startButton->setEnabled(true);
    }
    if(!ui->addStepOnSelectedButton->isEnabled())
    {
        ui->addStepOnSelectedButton->setEnabled(true);
    }    
//    qDebug() << "on_programsListView_clicked: Row" << index.row() << index.column();
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
            ui->h1Label->setStyleSheet("background-color: #FB6B00");
            ui->h1Label->update();
        }else{
            ui->h1Label->setStyleSheet("background-color: #FFFFFF");
            ui->h1Label->update();
        }
        break;
    case ControlCommands::H2:
        if(value){
            ui->h2Label->setStyleSheet("background-color: #FB6B00");
            ui->h2Label->update();
        }else{
            ui->h2Label->setStyleSheet("background-color: #FFFFFF");
            ui->h2Label->update();
        }
        break;
    case ControlCommands::T1:
        if(value){
            ui->t1Label->setStyleSheet("background-color: #FB6B00");
            ui->t1Label->update();
        }else{
            ui->t1Label->setStyleSheet("background-color: #FFFFFF");
            ui->t1Label->update();
        }
        break;
    case ControlCommands::T2:
        if(value){
            ui->t2Label->setStyleSheet("background-color: #FB6B00");
            ui->t2Label->update();
        }else{
            ui->t2Label->setStyleSheet("background-color: #FFFFFF");
            ui->t2Label->update();
        }
        break;
    case ControlCommands::P1:
        if(value){
            ui->p1Label->setStyleSheet("background-color: #FB6B00");
            ui->p1Label->update();
        }else{
            ui->p1Label->setStyleSheet("background-color: #FFFFFF");
            ui->p1Label->update();
        }
        break;
    case ControlCommands::P2:
        if(value){
            ui->p2Label->setStyleSheet("background-color: #FB6B00");
            ui->p2Label->update();
        }else{
            ui->p2Label->setStyleSheet("background-color: #FFFFFF");
            ui->p2Label->update();
        }
        break;
    case ControlCommands::P3:
        if(value){
            ui->p3Label->setStyleSheet("background-color: #FB6B00");
            ui->p3Label->update();
        }else{
            ui->p3Label->setStyleSheet("background-color: #FFFFFF");
            ui->p3Label->update();
        }
        break;
    case ControlCommands::LNU:
        if(value){
            ui->lnvLabel->setStyleSheet("background-color: #FB6B00");
            ui->lnvLabel->update();
        }else{
            ui->lnvLabel->setStyleSheet("background-color: #FFFFFF");
            ui->lnvLabel->update();
        }
        break;
    case ControlCommands::C1:
        if(value){
            ui->c1Label->setStyleSheet("background-color: #FB6B00");
            ui->c1Label->update();
        }else{
            ui->c1Label->setStyleSheet("background-color: #FFFFFF");
            ui->c1Label->update();
        }
        break;
    case ControlCommands::C2V2:
        if(value){
            ui->c2Label->setStyleSheet("background-color: #FB6B00");
            ui->v2Label->setStyleSheet("background-color: #FB6B00");
            ui->c2Label->update();
            ui->v2Label->update();
        }else{
            ui->c2Label->setStyleSheet("background-color: #FFFFFF");
            ui->v2Label->setStyleSheet("background-color: #FFFFFF");
            ui->c2Label->update();
            ui->v2Label->update();
        }
        break;
    case ControlCommands::V1:
        if(value){
            ui->v1Label->setStyleSheet("background-color: #FB6B00");
            ui->v1Label->update();
        }else{
            ui->v1Label->setStyleSheet("background-color: #FFFFFF");
            ui->v1Label->update();
        }
        break;
    case ControlCommands::V3:
        if(value){
            ui->v3Label->setStyleSheet("background-color: #FB6B00");
            ui->v3Label->update();
        }else{
            ui->v3Label->setStyleSheet("background-color: #FFFFFF");
            ui->v3Label->update();
        }
        break;
    case ControlCommands::V4:
        if(value){
            ui->v4Label->setStyleSheet("background-color: #FB6B00");
            ui->v4Label->update();
        }else{
            ui->v4Label->setStyleSheet("background-color: #FFFFFF");
            ui->v4Label->update();
        }
        break;
    case ControlCommands::FAN:
        if(value){
            ui->fn1Label->setStyleSheet("background-color: #FB6B00");
            ui->fn1Label->update();
        }else{
            ui->fn1Label->setStyleSheet("background-color: #FFFFFF");
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
    qDebug() << "MainWindow::on_tempPowerChange: value" << value;
    qDebug() << "MainWindow::on_tempPowerChange: res" << res;
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
                                arg(communication->pidController->testPgm->getCurrentCycleNum()).
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
                               arg(communication->pidController->testPgm->getCurrentStepNum()).
                               arg(value));
    }
        break;
    case Program::Curr_stp:
    {
         QString lbl("Step %1 of %2");
         ui->stepLabel->setText(lbl.
                                arg(value).
                                arg(communication->pidController->testPgm->getNoOfSteps()));
         double humSet = communication->pidController
                 ->testPgm->getSteps().value(communication->pidController
                                             ->testPgm->getCurrentStepNum())
                 ->getHumidity();
         double tempset = communication->pidController
                 ->testPgm->getSteps().value(communication->pidController
                                             ->testPgm->getCurrentStepNum())
                 ->getTemperature();
         ui->humidSetValueLabel->setText(QString("%1").arg(humSet));
         ui->tempSetValueLabel->setText(QString("%1").arg(tempset));
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

void MainWindow::initStyle(){
    ui->tabWidget->setCurrentIndex(MONITOR_INDEX);
    ui->loadProgramButton->setEnabled(false);
    ui->renameProgramButton->setEnabled(false);
    ui->deleteProgramButton->setEnabled(false);
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(false);

    ui->programStepsSpliter->setStretchFactor(0, 1);
    ui->programStepsSpliter->setStretchFactor(1, 3);

    ui->stepsTableView->setAlternatingRowColors(true);
    ui->stepsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    //! options tab !//
    QButtonGroup *optionsButtonGroup = new QButtonGroup(this);
    optionsButtonGroup->addButton(ui->sysInfoToolButton);
    optionsButtonGroup->addButton(ui->sysParamToolButton);
    optionsButtonGroup->addButton(ui->controlParamToolButton);
    optionsButtonGroup->setExclusive(true);
    //! options tab - system params
    ui->maxHighLineEdit->setValidator(new QIntValidator(25, 150, this));
    ui->maxLowLineEdit->setValidator(new QIntValidator(-10, -40, this));
    ui->pointLineEdit->setValidator(new QIntValidator(20, 40));
    //! options tab - pid params !//


    //! quick start tab !//
    ui->quickStartTableView->setAlternatingRowColors(true);
    ui->quickStartTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->quickStartTableView->setModel(new StepsModel());
    QRegExp str("[a-zA-Z]+");
    QValidator *strValidator = new QRegExpValidator(str, this);
    ui->nameLineEdit->setValidator(strValidator);
    ui->nameLineEdit->setPlaceholderText("untitled");
    ui->cycleLineEdit->setValidator(new QIntValidator(1, 50, this));
    ui->cycleLineEdit->setPlaceholderText("1");
    ui->quickStartButton->setEnabled(false);
    ui->quickStopButton->setEnabled(false);
}
void MainWindow::programTabInit()
{
    this->setWindowTitle("Climate Chamber - Program");
    ui->loadProgramButton->setEnabled(false);
    ui->renameProgramButton->setEnabled(false);
    ui->deleteProgramButton->setEnabled(false);
    ui->startButton->setEnabled(false);
    populateProgramsList();
}

void MainWindow::monitorTabInit()
{
    this->setWindowTitle("Climate Chamber - Monitor");
}

void MainWindow::optionsTabInit()
{
    this->setWindowTitle("Climate Chamber - Options");
    ui->sysInfoToolButton->setChecked(true);
    ui->optionsStackedWidget->setCurrentIndex(SYS_INFO);
}

void MainWindow::quickStartTabInit()
{
    this->setWindowTitle("Climate Chamber - Quick Start");
//    quickPgm->setProgramName("untitled");
//    quickPgm->setCycle(1);
//    StepsModel *stepsModel = new StepsModel();
    //stepsModel->setProgramToShow(quickPgm);
//    stepsModel->setProgramToShow(new Program());
//    ui->quickStartTableView->setModel(stepsModel);
}

void MainWindow::auxTabInit()
{
    this->setWindowTitle("Climate Chamber - AUX Data");
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    if(index == MONITOR_INDEX){
        monitorTabInit();
    }else if(index == PROGRAM_INDEX){
        programTabInit();
    }else if(index == OPTIONS_INDEX){
        optionsTabInit();
    }else if(index == QUICK_START){
        quickStartTabInit();
    }else if(index == AUX_INDEX){
        auxTabInit();
    }
}

void MainWindow::on_removeStepFromSelectedButton_clicked()
{
    QString pgmName(ui->programsListView->currentIndex().data().toString());
    pgmName = pgmName.left(pgmName.indexOf('.'));
    int selectedRow = ui->stepsTableView->selectionModel()->currentIndex().row();
    qDebug() << "on_removeStepFromSelectedButton_clicked: selectedRow"
             << selectedRow;
    StepsModel *stepsModel = (StepsModel*)ui->stepsTableView->model();
    stepsModel->removeRows(selectedRow, 1, QModelIndex());
}

void MainWindow::on_addStepOnSelectedButton_clicked()
{
    StepsModel *stepsModel = (StepsModel*)ui->stepsTableView->model();

    AddStep *ads = new AddStep();
    connect(ads, SIGNAL(stepFormSubmitted(QString,QString,QString,
                                          QString,QString,QString,
                                          QString,QString,QString)),
            stepsModel, SLOT(on_addStepFormSubmitted(QString,QString,QString,
                                                     QString,QString,QString,
                                                     QString,QString,QString)));
    QString pgmName(ui->programsListView->currentIndex().data().toString());
    pgmName = pgmName.left(pgmName.indexOf('.'));
    ads->setWindowTitle("Adding step to " + pgmName);
    ads->setModal(true);
    ads->show();

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


void MainWindow::on_testFinished()
{
    QMessageBox::information(this, "Information", "Runing Test Program has Finished",
                             QMessageBox::Ok, QMessageBox::NoButton);
    return;
}

void MainWindow::on_stopButton_clicked()
{
    communication->pidController->controlCommands->setIdle(true);
    communication->pidController->testPgm = new Program();
    ui->stopButton->setEnabled(false);
}

void MainWindow::on_startButton_clicked()
{
    if(!communication->pidController->controlCommands->isIdle()){
        QMessageBox::information(this, "Info","A Test Program is already running. Stop "
                                             "the Program first");
        return;
    }
    QString pgmName(ui->programsListView->currentIndex().data().toString());
    pgmName = pgmName.left(pgmName.indexOf('.'));
    QMessageBox::StandardButton reply =
            QMessageBox::question(this, "Confirm", "Start runing program "
                                  + pgmName, QMessageBox::Yes | QMessageBox::No,
                                  QMessageBox::No);
    if(reply == QMessageBox::No){
        return;
    }else{
        communication->pidController->startTest(pgmName);
        ui->stopButton->setEnabled(true);
        ui->startButton->setEnabled(false);
        ui->quickStartButton->setEnabled(false);
    }
}

void MainWindow::on_connectionLost(bool disconnected)
{
    if(disconnected){       
         this->statusBar()->
                showMessage("Connection to chamber is lost!", 1000);
    }else{

        this->statusBar()->showMessage("Connection to chamber established", 2000);
    }
}

void MainWindow::on_stepsTableView_clicked(const QModelIndex &index)
{
    if(!ui->removeStepFromSelectedButton->isEnabled()){
        ui->removeStepFromSelectedButton->setEnabled(true);
    }
    int selected = index.row();
    //int selected = ui->stepsTableView->selectionModel()->currentIndex().row();
    qDebug() << "on_stepsTableView_clicked: selected row: " << selected;
}

void MainWindow::on_quickStartButton_clicked(bool checked)
{
    Q_UNUSED(checked)
    if(!communication->pidController->controlCommands->isIdle()){
        QMessageBox::information(this, "Info","A Test Program is already running. Stop "
                                             "the Program first");
        return;
    }
    QString pgmName = ui->nameLineEdit->text();
    QString cycleStr = ui->cycleLineEdit->text();
    int cycle = 1;
    QMessageBox::StandardButton reply =
            QMessageBox::question(this, "Confirm", "Start runing quick test "
                                  + pgmName + "?", QMessageBox::Yes | QMessageBox::No,
                                  QMessageBox::No);
    if(reply == QMessageBox::No){
        return;
    }else{
        if(pgmName.isEmpty()){
            pgmName = "untitled";
        }
        if(!cycleStr.isEmpty()){
            cycle = cycleStr.toInt();
        }
//        Program *pgm = new Program();
        Program *pgm = ((StepsModel*)ui->quickStartTableView->model())
                ->getProgramToShow();
        //CONSL:
        qDebug() << "###### About to run pgm with " << pgm->getNoOfSteps()
                 << " steps #####";
        pgm->setProgramName(pgmName);
        pgm->setCycle(cycle);

        communication->pidController->startQuickTest(pgm);
    }

}

void MainWindow::on_quickStopButton_clicked()
{
    //TODO: implement on quick stop here
}

void MainWindow::on_idleStateChanged(bool state)
{
    ui->startButton->setEnabled(state);
    ui->quickStartButton->setEnabled(state);
    ui->loadProgramButton->setEnabled(state);
    ui->newProgramButton->setEnabled(state);
    ui->renameProgramButton->setEnabled(state);
    ui->deleteProgramButton->setEnabled(state);

    ui->stopButton->setEnabled(!state);
    ui->quickStopButton->setEnabled(!state);
}

void MainWindow::on_sysInfoToolButton_clicked(bool checked)
{
    ui->optionsStackedWidget->setCurrentIndex(SYS_INFO);
}

void MainWindow::on_sysParamToolButton_clicked(bool checked)
{
    ui->optionsStackedWidget->setCurrentIndex(SYS_PARAM);
}

void MainWindow::on_controlParamToolButton_clicked(bool checked)
{
    ui->optionsStackedWidget->setCurrentIndex(CONTROL_PARAM);
    ui->tempPIDListView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tempPIDListView->setSelectionMode(QAbstractItemView::SingleSelection);
    DataBackup db;
    PidListModel *tempPidModel = new PidListModel();
    tempPidModel->setPidList(db.loadPIDList(0));
    ui->tempPIDListView->setModel(tempPidModel);

    PidListModel *humidPidModel = new PidListModel();
    humidPidModel->setPidList(db.loadPIDList(1));
    ui->humidPIDListView->setModel(humidPidModel);

    if(communication->pidController->getTemperaturePID()->getKp() == 0){
        QMessageBox::information(this, "Info", "PID params default has not been set yet."
                                               "Remember to set defaults before runing tests",
                                 QMessageBox::Ok);
    }
}

void MainWindow::on_quickAddStepButton_clicked()
{
    AddStep *ads = new AddStep();
    StepsModel *stepsModel = (StepsModel*)ui->quickStartTableView->model();
    Program * pgm = stepsModel->getProgramToShow();
    connect(ads, SIGNAL(stepFormSubmitted(QString,QString,QString,
                                          QString,QString,QString,
                                          QString,QString,QString)),
            stepsModel, SLOT(on_addStepFormSubmitted(QString,QString,QString,
                                                     QString,QString,QString,
                                                     QString,QString,QString)));

    ads->setWindowTitle("Adding step to " + pgm->getProgramName());
    ads->setModal(true);
    ads->exec();
    emit quickStepAboutToAdd();
}

void MainWindow::on_quickStepAboutToAdd()
{
    int size = ((StepsModel*)ui->quickStartTableView->model())->
            getProgramToShow()->getNoOfSteps();
//    int size = quickPgm->getNoOfSteps();
    //CONSL: on_quickStepAboutToAdd
    qDebug () << "###### " << size << " #####";
    if(size > 0){
        ui->quickStartButton->setEnabled(true);
    }
}

void MainWindow::on_useButton_clicked()
{
    settings;
    //TODO: load the entered PID values for use in this context.
//    if(ui->pTempDoubleSpinBox->text().toDouble() == 0 ||
//            ui->iTempDoubleSpinBox->text().toDouble() == 0 ||
//            ui->dTempDoubleSpinBox->text().toDouble() == 0 ||
//            ui->pHumidDoubleSpinBox->text().toDouble() == 0 ||
//            ui->iHumidDoubleSpinBox->text().toDouble() == 0 ||
//            ui->dHumidDoubleSpinBox->text().toDouble() == 0){
//        QMessageBox::warning(this, "Empty Field", "Fields can not be empty or zero",
//                            QMessageBox::Ok);
//        return;
//    }
//    settings.setValue("pid/ptemp", ui->pTempDoubleSpinBox->text());
//    settings.setValue("pid/itemp", ui->iTempDoubleSpinBox->text());
//    settings.setValue("pid/dtemp", ui->dTempDoubleSpinBox->text());
//    settings.setValue("pid/phumid", ui->pHumidDoubleSpinBox->text());
//    settings.setValue("pid/ihumid", ui->iHumidDoubleSpinBox->text());
//    settings.setValue("pid/dhumid", ui->dHumidDoubleSpinBox->text());
    qDebug() << "" << settings.value("pid/ptemp").toString();

}

void MainWindow::on_makeDefaultButton_clicked()
{
    QSettings settings;
    //TODO: save the PID values entered, to persistance.
    int index = ui->pidTabWidget->currentIndex();
    if(index == Temperature_Index){
        ui->tempPIDListView->currentIndex().data().toString();
        //TODO: Call method to save default temp pid setting based on this data
    }else if(index == Humidity_Index){
        ui->humidPIDListView->currentIndex().data().toString();
        //TODO: Call method to save default humid pid setting based on this data
    }
}

void MainWindow::on_plusTPButton_clicked()
{
    AddPid addPid;
    addPid.setModal(true);
    addPid.exec();
}

void MainWindow::on_plusHPButton_clicked()
{
    AddPid addPid;
    addPid.setModal(true);
    addPid.exec();
}

void MainWindow::on_pidTabWidget_currentChanged(int index)
{
    switch (index) {
    case Temperature_Index:
    {
        DataBackup db;
//        QList<PID> pidList = db.loadPIDList(1);
        PidListModel *tempPidModel = new PidListModel();
        tempPidModel->setPidList(db.loadPIDList(0));
        ui->tempPIDListView->setModel(tempPidModel);
        ui->tempPIDListView->repaint();
    }
        break;
    case Humidity_Index:

        break;
    default:
        break;
    }
}
