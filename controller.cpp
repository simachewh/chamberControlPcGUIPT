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


    temperaturePID->setDt(2.1);
    humidityPID->setDt(3.1);
    //FIXME:load PID params from setting instead.
    QSettings settings;
    //CONSL:
    qDebug() <<"CHecking settins file" << settings.fileName();
    qDebug() <<"CHecking settins value" <<settings.value("ptemp", "No value").toDouble();
    temperaturePID->setKp(settings.value("ptemp", 0).toDouble());
    temperaturePID->setKi(settings.value("itemp", 0).toDouble());
    temperaturePID->setKd(settings.value("dtemp", 0).toDouble());
    //FIXME: Load PID params from settings instead.
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

    /// chamnber params connection
    connect(chamberParams, SIGNAL(dryTemperatureChanged(double)),
            temperaturePID, SLOT(setMeasuredValue(double)));

    connect(chamberParams, SIGNAL(dryTemperatureChanged(double)),
            this, SLOT(on_TemperatureRealChanged(double)));

    connect(chamberParams, SIGNAL(humidityChanged(double)),
            humidityPID, SLOT(setMeasuredValue(double)));
    //TODO: set up connection between the both pid's setvalues and the
    //the controllers change in stepps (the next set value)
}

Controller::~Controller()
{
    delete timer;
}

void Controller::controlTestRun()
{

    runDeviceControll();
    qDebug() << "CController::controlTestRun: control finishsed";
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
    //timer->start(timeOut);
    timer->start(7000);
}

void Controller::changeStep()
{
    if(testPgm->goToNextStep()){
        currentStep = testPgm->getCurrentStep();
        nextStep = testPgm->getNextStep();
        previousStep = testPgm->getPreviousStep();
    }else{
        emit stepsDone(true);
    }
}

void Controller::on_stepChange()
{
    setUpStart();
    controlTestRun();
}

void Controller::on_controlRequested()
{
    controlTestRun();
}

void Controller::runDeviceControll()
{
    bool ON = true;
    bool OFF = false;

    int timeLeft = timer->remainingTime();
//TODO: check how the values bellow are changing.
    double temperatureError = temperaturePID->getError();
    double humidityError = humidityPID->getError();
    qDebug() << "MeasuredTemp " << temperaturePID->getMeasuredValue()
             << "measuredHumid" << humidityPID->getMeasuredValue();
    if(temperatureError > 10){
        ///turn heaters on
        controlCommands->switchHeaters(ON);
        controlCommands->setTemperaturePower(temperaturePID->control());
        /// trurn coolers off
        controlCommands->switchCooler(OFF);
        controlCommands->switchValves(OFF);
        qDebug() << ">10";
    }else if(temperatureError < 10 && temperatureError > -10){
        controlCommands->setTemperaturePower(0);
        controlCommands->switchHeaters(OFF);
        controlCommands->switchCooler(OFF);
        qDebug() << "-10<x<10";
    }else if(temperatureError < -10){
        controlCommands->switchValves(ON);
        controlCommands->switchCooler(ON);
        /// switch heaters off
        controlCommands->setTemperaturePower(0);
        controlCommands->switchHeaters(OFF);
        qDebug() << "x<-10";
    }

    if(humidityError > 6){
        controlCommands->switchHumidifiers(ON);
        controlCommands->setHumidityPower(humidityPID->control());
    }else{
        controlCommands->switchHumidifiers(OFF);
        controlCommands->setHumidityPower(0);
    }

    qDebug() << "runDeviceControll : tempErr: " << temperatureError
             <<" humErr: " << humidityError;

}

void Controller::on_stepsDone(bool)
{
    controlCommands->resetAll();
    qDebug() << "Steps are finished and reset all devices";
}

void Controller::startTest(QString programName)
{
    DataBackup db;
    db.loadTestProgram(programName, this->testPgm);
    currentStep = testPgm->getCurrentStep();
    nextStep = testPgm->getNextStep();
    qDebug() << "Current step temperature "
             << currentStep->getTemperature();

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
    currentStep = value;
}

Step *Controller::getPreviousStep() const
{
    return previousStep;
}

void Controller::setPreviousStep(Step *value)
{
    previousStep = value;
}

Step *Controller::getNextStep() const
{
    return nextStep;
}

void Controller::setNextStep(Step *value)
{
    nextStep = value;
}
