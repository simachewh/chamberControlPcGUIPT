#include "controller.h"

Controller::Controller(QObject *parent) : QObject(parent)
{ 
    testPgm = new Program(parent);
    chamberParams = new Chamber(parent);
    controlCommands = new ControlCommands(parent);

    chamberParams->setDryTemprature(0);
    chamberParams->setHumidity(0);

    timer = new QTimer(parent);

    ///initialize class members
    previousStep = new Step();
    currentStep = new Step();
    nextStep = new Step();

    integralTemp = derivativeTemp = proportionalTemp = 0;
    integralHumid = derivativeHumid = proportionalHumid = 0;
    tempError = previousTempError = 0;
    humidError = previousHumidError = 0;
    tempError = humidError = 0;
    dtTemp = 2.1;
    dtHumid = 3.1;

    kpTemp = 3.2;
    kiTemp = 60;
    kdTemp = 168;

    kpHumd = 3.0;
    kiHumid = 60;
    kdHumid = 152;


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
            this, SLOT(setMeasuredTemp(double)));

    connect(chamberParams, SIGNAL(dryTemperatureChanged(double)),
            this, SLOT(on_TemperatureRealChanged(double)));

    connect(chamberParams, SIGNAL(humidityChanged(double)),
            this, SLOT(setMeasuredHumid(double)));
}

Controller::~Controller()
{
    delete timer;
}

int Controller::pidTempControl()
{
    tempError = temperatureSetValue - measuredTemp;
    integralTemp = integralTemp + (tempError * dtTemp);
    derivativeTemp = (tempError - previousTempError) / dtTemp;

    int tempPower = (kpTemp * tempError)
            + (kiTemp * integralTemp)
            + (kdTemp * derivativeTemp);
    previousTempError = tempError;
    qDebug() << "pidTempControl : output before " << tempPower;
    if(tempPower < MIN_OUT){
        tempPower = MIN_OUT;
    }
    if(tempPower > MAX_OUT){
        tempPower = MAX_OUT;
    }
    qDebug() << "pidTempControl : tempErr " << tempError;
    qDebug() << "pidTempControl : temp integral " << integralTemp;
    qDebug() << "pidTempControl : derivative " << derivativeTemp;
    qDebug() << "pidTempControl : output " << tempPower;
    return tempPower;
}

int Controller::pidHumidControl()
{
    humidError = HumiditySetValue - measuredHumid;
    integralHumid = integralHumid + (humidError * dtHumid);
    derivativeHumid = (humidError - previousHumidError) / dtHumid;

    int humidPower = (kpHumd * humidError) + (kiHumid * integralHumid)
            + (kdHumid * derivativeHumid);
    previousHumidError = humidError;
    if(humidPower < MIN_OUT){
        humidPower = MIN_OUT;
    }
    if(humidPower > MAX_OUT){
        humidPower = MAX_OUT;
    }
    qDebug() << "pidHumidControl : output " << humidPower;
    return humidPower;
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
    int stepNo = testPgm->getCurrentStepNum();

    double setTemperature = currentStep->getTemperature();
    double setHumidity = currentStep->getHumidity();

    int timeLeft = timer->remainingTime();

    double temperatureError = setTemperature -measuredTemp;
    double humidityError = setHumidity - measuredHumid;
    qDebug() << "MeasuredTemp " << measuredTemp << "measuredHumid" << measuredHumid;
    if(temperatureError > 10){
        ///turn heaters on
        controlCommands->switchHeaters(ON);
        controlCommands->setTemperaturePower(pidTempControl());
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
        controlCommands->setHumidityPower(pidHumidControl());
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

double Controller::getPreviousTempError() const
{
    return previousTempError;
}

double Controller::getKpTemp() const
{
    return kpTemp;
}

void Controller::setKpTemp(double value)
{
    kpTemp = value;
}

double Controller::getKiTemp() const
{
    return kiTemp;
}

void Controller::setKiTemp(double value)
{
    kiTemp = value;
}

double Controller::getKdTemp() const
{
    return kdTemp;
}

void Controller::setKdTemp(double value)
{
    kdTemp = value;
}

double Controller::getIntegralTemp() const
{
    return integralTemp;
}

void Controller::setIntegralTemp(double value)
{
    integralTemp = value;
}

double Controller::getProportionalTemp() const
{
    return proportionalTemp;
}

void Controller::setProportionalTemp(double value)
{
    proportionalTemp = value;
}

double Controller::getDerivativeTemp() const
{
    return derivativeTemp;
}

void Controller::setDerivativeTemp(double value)
{
    derivativeTemp = value;
}

double Controller::getDtTemp() const
{
    return dtTemp;
}

void Controller::setDtTemp(double value)
{
    dtTemp = value;
}

double Controller::getKpHumd() const
{
    return kpHumd;
}

void Controller::setKpHumd(double value)
{
    kpHumd = value;
}

double Controller::getKiHumid() const
{
    return kiHumid;
}

void Controller::setKiHumid(double value)
{
    kiHumid = value;
}

double Controller::getKdHumid() const
{
    return kdHumid;
}

void Controller::setKdHumid(double value)
{
    kdHumid = value;
}

double Controller::getIntegralHumid() const
{
    return integralHumid;
}

void Controller::setIntegralHumid(double value)
{
    integralHumid = value;
}

double Controller::getProportionalHumid() const
{
    return proportionalHumid;
}

void Controller::setProportionalHumid(double value)
{
    proportionalHumid = value;
}

double Controller::getDerivativeHumid() const
{
    return derivativeHumid;
}

void Controller::setDerivativeHumid(double value)
{
    derivativeHumid = value;
}

double Controller::getDtHumid() const
{
    return dtHumid;
}

void Controller::setDtHumid(double value)
{
    dtHumid = value;
}

void Controller::on_TemperatureRealChanged(double value)
{
}

void Controller::setPreviousTempError(double value)
{
    previousTempError = value;
}

double Controller::getTempError() const
{
    return tempError;
}

void Controller::setTempError(double value)
{
    tempError = value;
}

double Controller::getPreviousHumidError() const
{
    return previousHumidError;
}

void Controller::setPreviousHumidError(double value)
{
    previousHumidError = value;
}

double Controller::getHumidError() const
{
    return humidError;
}

void Controller::setHumidError(double value)
{
    humidError = value;
}

double Controller::getTemperatureSetValue() const
{
    return temperatureSetValue;
}

void Controller::setTemperatureSetValue(double value)
{
    temperatureSetValue = value;
}

double Controller::getHumiditySetValue() const
{
    return HumiditySetValue;
}

void Controller::setHumiditySetValue(double value)
{
    HumiditySetValue = value;
}

double Controller::getMeasuredTemp() const
{
    return measuredTemp;
}

void Controller::setMeasuredTemp(double value)
{
    measuredTemp = value;
}

double Controller::getMeasuredHumid() const
{
    return measuredHumid;
}

void Controller::setMeasuredHumid(double value)
{
    measuredHumid = value;
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
