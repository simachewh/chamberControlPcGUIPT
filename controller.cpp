#include "controller.h"

Controller::Controller(QObject *parent) : QObject(parent)
{
    testPgm = new Program(parent);
    chamberParams = new Chamber(parent);
    controlCommands = new ControlCommands(parent);
    chamberParams->setDryTemprature(0);
    chamberParams->setHumidity(0);

    timer = new QTimer(parent);

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

void Controller::controlTestRun()
{

    runDeviceControll();
    qDebug() << "CController::controlTestRun: control finishsed";
    emit controlready(ControlCommands::A);
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
    if(tempErr > 25){
        controlCommands->setT1(true);
        controlCommands->setT2(true);
        controlCommands->setTemperaturePower(255);
    }else if(tempErr > 5 && tempErr < 25){
        controlCommands->setT1(true);
        controlCommands->setT2(false);
        controlCommands->setTemperaturePower(127);
    }else if(tempErr < 5 && tempErr > 0){
        controlCommands->setT1(true);
        controlCommands->setT2(false);
        controlCommands->setTemperaturePower(8);
    }else if(tempErr <= 0.01){
        //check time and use switch
        qDebug() << "Temperror less than 0";
        controlCommands->setTemperaturePower(0);
        controlCommands->setT1(false);
        controlCommands->setT2(false);
        qDebug() << "Temperror less than 0, values changed";
    }

    //! humidifier On or Off
    if(humErr > 25){
        controlCommands->setH1(true);
        controlCommands->setH2(true);
        controlCommands->setHumidityPower(255);
    }else if(humErr > 5 && humErr < 25){
        controlCommands->setH1(true);
        controlCommands->setH2(false);
        controlCommands->setHumidityPower(127);
    }else if(humErr < 5 && humErr > 0){
        controlCommands->setH1(true);
        controlCommands->setH2(false);
        controlCommands->setHumidityPower(16);
    }else if(humErr <= 0.1){
        qDebug() << "Humerror less than 0";
        controlCommands->setHumidityPower(0);
        controlCommands->setH1(false);
        controlCommands->setH2(false);
    }

}

void Controller::on_stepsDone(bool)
{
    controlCommands->resetAll();
}

void Controller::startTest()
{
    setUpStart();
    controlTestRun();
}
