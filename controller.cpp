#include "controller.h"

const QString Controller::Temp_P = "ptemp";
const QString Controller::Temp_I = "itemp";
const QString Controller::Temp_D = "dtemp";
const QString Controller::Humid_P = "phumid";
const QString Controller::Humid_I = "ihumid";
const QString Controller::Humid_D = "dhumid";

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
    testPgm = new Program(parent);
    chamberParams = new Chamber(parent);
    controlCommands = new ControlCommands(parent);

    chamberParams->setDryTemprature(0);
    chamberParams->setHumidity(0);

    temperaturePID = new PID(parent);
    humidityPID = new PID(parent);

    timer = new QTimer(parent);

    ///initialize class members
    previousStep = new Step();
    currentStep = new Step();
    nextStep = new Step();


    temperaturePID->setDt(2);
    humidityPID->setDt(2.3);
    //load settings
    QSettings settings;
    //assign temperature PID settings to system
    temperaturePID->setKp(settings.value("ptemp", 0).toDouble());
    temperaturePID->setKi(settings.value("itemp", 0).toDouble());
    temperaturePID->setKd(settings.value("dtemp", 0).toDouble());

    //assign humidity PID settings to system
    humidityPID->setKp(settings.value("phumid", 0).toDouble());
    humidityPID->setKi(settings.value("ihumid", 0).toDouble());
    humidityPID->setKd(settings.value("dhumid", 0).toDouble());

    /// self connection
    connect(this, SIGNAL(stepsDone(bool)),
            this, SLOT(on_stepsDone(bool)));

    /// timer connections
    connect(timer, SIGNAL(timeout()),
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

    ///
    connect(chamberParams, SIGNAL(dryTemperatureChanged(double)),
            this, SLOT(on_TemperatureRealChanged(double)));

    //TODO: set up connection between the both pid's setvalues and the
    //the controllers change in stepps (the next set value)
    connect(this, SIGNAL(currentStepChanged()),
            this, SLOT(on_stepChange()));

}

Controller::~Controller()
{
    delete timer;
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
    timer->setSingleShot(true);
    int hrs = currentStep->getHours();
    int min = currentStep->getMinutes();
    int timeOut = hrs * 3600000;
    timeOut += (min * 60000);
    timer->start(timeOut);
//    timer->start(2000);
}

void Controller::changeStep()
{
    if(testPgm->goToNextStep()){
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

    int timeLeft = timer->remainingTime();
}

void Controller::on_stepsDone(bool)
{
    controlCommands->resetAll();
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

void Controller::on_TemperatureRealChanged(double value)
{
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
