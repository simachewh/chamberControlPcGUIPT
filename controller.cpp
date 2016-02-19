#include "controller.h"

const QString Controller::Temp_P = "ptemp";
const QString Controller::Temp_I = "itemp";
const QString Controller::Temp_D = "dtemp";
const QString Controller::Humid_P = "phumid";
const QString Controller::Humid_I = "ihumid";
const QString Controller::Humid_D = "dhumid";
const QString Controller::PlotInterval = "plotInterval";

Program *Controller::getTestPgm() const
{
    return testPgm;
}

void Controller::setTestPgm(Program *value)
{
    testPgm = value;
}

PID *Controller::getTemperaturePID() const
{
    return temperaturePID;
}

void Controller::setTemperaturePID(PID *value)
{
    temperaturePID = value;
}

PID *Controller::getHumidityPID() const
{
    return humidityPID;
}

void Controller::setHumidityPID(PID *value)
{
    humidityPID = value;
}
Controller::Controller(QObject *parent) : QObject(parent)
{ 
    //load settings
    QSettings settings;
    testPgm = new Program(parent);
    chamberParams = new Chamber(parent);
    controlCommands = new ControlCommands(parent);
    db = new DataBackup(parent);

    chamberParams->setDryTemprature(0);
    chamberParams->setHumidity(0);

    temperaturePID = new PID(parent);
    humidityPID = new PID(parent);

    stepTimer = new QTimer(parent);
    plotTimer = new QTimer(parent);
    plotTimer->setInterval(settings.value(PlotInterval).toInt() * 2000);

    previousStep = new Step();
    currentStep = new Step();
    nextStep = new Step();


    temperaturePID->setDt(2);
    humidityPID->setDt(2.3);

    //assign temperature PID settings to system
    temperaturePID->setKp(settings.value(Temp_P, 0).toDouble());
    temperaturePID->setKi(settings.value(Temp_I, 0).toDouble());
    temperaturePID->setKd(settings.value(Temp_D, 0).toDouble());

    //assign humidity PID settings to system
    humidityPID->setKp(settings.value(Humid_P, 0).toDouble());
    humidityPID->setKi(settings.value(Humid_I, 0).toDouble());
    humidityPID->setKd(settings.value(Humid_D, 0).toDouble());

    /// self connection
    connect(this, SIGNAL(stepsDone(bool)),
            this, SLOT(on_stepsDone(bool)));

    /// timer connections
    connect(stepTimer, SIGNAL(timeout()),
            this, SLOT(changeStep()));

    /// Program connections
    connect(this->testPgm, SIGNAL(currentStepChanged(int)),
            this, SLOT(on_stepChange()));

    connect(testPgm, SIGNAL(stepsChanged()),
            this, SLOT(on_stepsChanged()));

    /// chamnberParams connection to pids
    connect(chamberParams, SIGNAL(dryTemperatureChanged(double)),
            temperaturePID, SLOT(setMeasuredValue(double)));

    connect(chamberParams, SIGNAL(humidityChanged(double)),
            humidityPID, SLOT(setMeasuredValue(double)));
    /// NOTE: this connection must come after the two connections above
    /// meaning the connections of new chamber values to pid measured values.
    /// This is because bothe real values on temperature and humidity must be ready
    /// for the slot in this connection, on_realValuesCollected(); to work properly.
    /// if this connection is called before those two, the program will behave
    /// unexpectdly.
    connect(chamberParams, SIGNAL(humidityChanged(double)),
            this, SLOT(on_realValuesCollected()));

    //TODO: set up connection between the both pid's setvalues and the
    //the controllers change in stepps (the next set value)
    connect(this, SIGNAL(currentStepChanged()),
            this, SLOT(on_stepChange()));

    connect(this, SIGNAL(targetReached()),
            this, SLOT(changeStep()));

    connect(plotTimer, SIGNAL(timeout()),
            this, SLOT(on_plotTimerOut()));

}

Controller::~Controller()
{
    delete stepTimer;
}

void Controller::controlTestRun()
{
    temperaturePID->control();
    humidityPID->control();
    runDeviceControll();
    qDebug() << "CController::controlTestRun: control finishsed"
             << temperaturePID->getOutput() << humidityPID->getOutput();
    emit controlready(ControlCommands::I);
}

void Controller::setUpStart()
{
    qDebug() << "Controller::setUpStart: entered";
    stepTimer->setSingleShot(true);
    int hrs = currentStep->getHours();
    int min = currentStep->getMinutes();
    int timeOut = hrs * 3600000;
    timeOut += (min * 60000);
    stepTimer->start(timeOut);
    plotTimer->start();
//    stepTimer->start(2000);
}

void Controller::changeStep()
{
    if(!currentStep->getCompleted() && currentStep->getWaiting()){
        return;
    }
    if(currentStep->getCompleted() && stepTimer->remainingTime() > 0){
        if(testPgm->goToNextStep()){
            setCurrentStep(testPgm->getCurrentStep());
            setNextStep(testPgm->getNextStep());
            setPreviousStep(testPgm->getPreviousStep());
        }else{
            emit stepsDone(true);
        }
        stepTimer->stop();
    }else if(testPgm->goToNextStep()){
        setCurrentStep(testPgm->getCurrentStep());
        setNextStep(testPgm->getNextStep());
        setPreviousStep(testPgm->getPreviousStep());
    }else{
        emit stepsDone(true);
    }
}

void Controller::on_stepChange()
{
    temperaturePID->setSetValue(currentStep->getTemperature());
    humidityPID->setSetValue(currentStep->getHumidity());
    setUpStart();
    controlTestRun();
}

void Controller::on_controlRequested()
{
    controlTestRun();
}

void Controller::runDeviceControll()
{
    bool on = true;
    bool off = false;

    controlCommands->setTemperaturePower(temperaturePID->getOutput());
    controlCommands->setHumidityPower(humidityPID->getOutput());

    if (temperaturePID->getOutput() <= 0){
        controlCommands->setV1(on);
        controlCommands->setC1(on);
        controlCommands->switchHeaters(off);
    }else if(temperaturePID->getOutput() > 0){
        controlCommands->switchHeaters(on);
        controlCommands->switchCooler(off);
    }

    if(humidityPID->getOutput() <= 0){
        controlCommands->switchHumidifiers(off);
    }else if (humidityPID->getOutput() > 0){
        controlCommands->switchHumidifiers(on);
    }

    int timeLeft = stepTimer->remainingTime();
}

void Controller::on_stepsDone(bool)
{
    DataBackup db;
    stepTimer->stop();
    plotTimer->stop();
    controlCommands->setIdle(false);
    controlCommands->resetAll();
    db.on_testFinished(testPgm->getProgramName());

    qDebug() << "Steps are finished and reset all devices";
}

void Controller::on_stepsChanged()
{
    testPgm->setCurrentStepNum(0);
    testPgm->setCurrentCycle(1);
    currentStep = testPgm->getCurrentStep();
    nextStep = testPgm->getNextStep();
    previousStep = testPgm->getPreviousStep();
    temperaturePID->setSetValue(currentStep->getTemperature());
    humidityPID->setSetValue(currentStep->getHumidity());
}

void Controller::on_plotTimerOut()
{
    ///TODO: save records here
    double temp = temperaturePID->getMeasuredValue();
    double humid = humidityPID->getMeasuredValue();
    int totalMinutes =currentStep->getHours() * 60 + currentStep->getMinutes();
    int remainingMinutes = (stepTimer->remainingTime() / 1000) / 60;
    int elapsedMinutes = totalMinutes - remainingMinutes;
    ///CNSL:
    qDebug() << totalMinutes << " times in munutes "
             << remainingMinutes << "=" << elapsedMinutes;
    DataBackup db;
    ///Note: how would this function know the file it is looking for
    db.appendPlot(testPgm->getProgramName(), temp, humid, elapsedMinutes);
}

void Controller::startTest(QString programName)
{
    DataBackup db;
    db.loadTestProgram(programName, this->testPgm);
    currentStep = testPgm->getCurrentStep();
    nextStep = testPgm->getNextStep();
    qDebug() << "Controller::startTest -> Current step temperature "
             << currentStep->getTemperature()
             << testPgm->getCurrentStep()->getTemperature();

    controlCommands->setIdle(false);
    db.on_testStarted(programName);
    setUpStart();
    controlTestRun();
}

void Controller::saveTempDefault(QString data)
{
    QStringList list = data.split(" ");
    QSettings setting;
    setting.setValue(Temp_P, list[0].trimmed());
    setting.setValue(Temp_I, list[1].trimmed());
    setting.setValue(Temp_D, list[2].trimmed());
}

void Controller::saveHumidDefault(QString data)
{
    QStringList list = data.split(" ");
    QSettings setting;
    setting.setValue(Humid_P, list[0].trimmed());
    setting.setValue(Humid_I, list[1].trimmed());
    setting.setValue(Humid_D    , list[2].trimmed());
}

void Controller::removePIDDefaults(int choice)
{
    QSettings setting;
    if(choice == 0){
        setting.remove(Temp_P);
        setting.remove(Temp_I);
        setting.remove(Temp_D);
    }
    if(choice == 1){
        setting.remove(Humid_D);
        setting.remove(Humid_I);
        setting.remove(Humid_P);
    }
}

bool Controller::isDefaultSet()
{
    QSettings setting;
    bool ret = setting.value(Temp_P, 0.00).toDouble() != 0.00
            && setting.value(Temp_I, 0.00).toDouble() != 0.00
            && setting.value(Temp_D, 0.00).toDouble() != 0.00
            && setting.value(Humid_P, 0.00).toDouble() != 0.00
            && setting.value(Humid_I, 0.00).toDouble() != 0.00
            && setting.value(Humid_D, 0.00).toDouble() != 0.00;
    return ret;
}

void Controller::startQuickTest(Program *pgm)
{

    setTestPgm(pgm);
    //TODO: prepare start up here
    currentStep = testPgm->getCurrentStep();
    nextStep = testPgm->getNextStep();

    controlCommands->setIdle(false);
    //TODO: start testing here

}

void Controller::on_temperatureRealChanged(double value)
{

}

void Controller::on_humidityRealChanged(double value)
{

}

void Controller::on_realValuesCollected()
{
    double temp = temperaturePID->getMeasuredValue();
    double humid = humidityPID->getMeasuredValue();

    if(temp <= currentStep->getTemperature() + 0.01 &&
            temp >= currentStep->getTemperature() -0.01 &&
            humid <= currentStep->getHumidity() + 0.1 &&
            humid >= currentStep->getHumidity() - 0.1){
        currentStep->setCompleted(true);
        emit targetReached();
    }
}

Step *Controller::getCurrentStep() const
{
    return currentStep;
}

void Controller::setCurrentStep(Step *value)
{
    if(currentStep == value){
        return;
    }else{
        currentStep = value;
        emit currentStepChanged();
    }
}

Step *Controller::getPreviousStep() const
{
    return previousStep;
}

void Controller::setPreviousStep(Step *value)
{
    if(previousStep == value){
        return;
    }else {
        previousStep = value;
        emit previousStepChanged();
    }
}

Step *Controller::getNextStep() const
{
    return nextStep;
}

void Controller::setNextStep(Step *value)
{
    if(nextStep == value){
        return;
    }else {
        nextStep = value;
        emit nextStepChanged();
    }
}
