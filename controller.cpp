#include "controller.h"


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
Controller::Controller(QObject *parent) : QObject(parent)
{
    testPgm = new Program(parent);
    chamberParams = new Chamber(parent);
    controlCommands = new ControlCommands(parent);
    chamberParams->setDryTemprature(0);
    chamberParams->setHumidity(0);

    timer = new QTimer(parent);

    integralTemp = derivativeTemp = proportionalTemp = 0;
    integralHumid = derivativeHumid = proportionalHumid = 0;
    tempError = previousTempError = 0;
    humidError = previousHumidError = 0;
    tempError = humidError = 0;
    dtTemp = 2.1;
    dtHumid = 3.1;


    connect(timer, SIGNAL(timeout()),
            this, SLOT(changeStep()));
    //connect
    connect(this->testPgm, SIGNAL(currentStepChanged(int)),
            this, SLOT(on_stepChange()));
    connect(this, SIGNAL(stepsDone(bool)),
            this, SLOT(on_stepsDone(bool)));
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

    int tempPower = (kpTemp * tempError) + (kiTemp * integralTemp)
            + (kdTemp * derivativeTemp);
    return tempPower;
}

int Controller::pidHumidControl()
{
    humidError = HumiditySetValue - measuredHumid;
    integralHumid = integralHumid + (humidError * dtHumid);
    derivativeHumid = (humidError - previousHumidError) / dtHumid;

    int humidPower = (kpHumd * humidError) + (kiHumid * integralHumid)
            + (kdHumid * derivativeHumid);
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
    int hrs = testPgm->getSteps().value(testPgm->getCurrentStep())->getHours();
    int min = testPgm->getSteps().value(testPgm->getCurrentStep())->getMinutes();
    int timeOut = hrs * 3600000;
    timeOut += (min * 60000);
    //timer->start(timeOut);
    timer->start(5000);
}

void Controller::changeStep()
{
    //! check if wait is on and relvals reached setvals
    //! and return
    int totalSteps = this->testPgm->getSteps().size();
    int nowStep = this->testPgm->getCurrentStep();
    int totalCycle = this->testPgm->getCycle();
    int nowCycle = this->testPgm->getCurrentCycle();

    if(nowStep < totalSteps){
        this->testPgm->setCurrentStep(nowStep + 1);        
    }else if(nowStep == totalSteps && nowCycle < totalCycle){
        testPgm->setCurrentCycle(nowCycle + 1);
        testPgm->setCurrentStep(1);
    }else if(nowStep == totalSteps && nowCycle == totalCycle){
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
    double realTemp = chamberParams->getDryTemperature();
    double realHumid = chamberParams->getHumidity();
    int stepNo = testPgm->getCurrentStep();
    double setTemp = testPgm->getSteps().value(stepNo)
            ->getTemperature();
    double setHumid = testPgm->getSteps().value(stepNo)
            ->getHumidity();
    int timeLeft = timer->remainingTime();

    double tempErr = setTemp -realTemp;
    double humErr = setHumid -realHumid;

    qDebug() << "runDeviceControll : tempErr: " << tempErr
             <<" humErr: " << humErr;

    //! heaters On or Off
    controlCommands->resetAll();


}

void Controller::on_stepsDone(bool)
{
    controlCommands->resetAll();
}

void Controller::startTest()
{
    //controlCommands->setH1(true);
    setUpStart();
    controlTestRun();
}

double Controller::getPreviousTempError() const
{
    return previousTempError;
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
