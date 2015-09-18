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
            this->controlCommands, SLOT(setIdle(bool)));
}

Controller::~Controller()
{
    delete timer;
}

bool Controller::h1On(double setValue, double feedback)
{
    if(setValue > feedback){
        return true;
    }else{
        return false;
    }
}

bool Controller::h2On(double setValue, double feedback)
{
    if(setValue > feedback){
        return true;
    }else{
        return false;
    }
}

bool Controller::t1On(double tsetVal, double tfeedback)
{
    if(tsetVal > tfeedback){
        return true;
    }else{
        return false;
    }
}

bool Controller::t2On(double tsetVal, double tfeedback)
{
    if(tsetVal > tfeedback){
        return true;
    }else{
        return false;
    }
}

void Controller::controlTestRun()
{
    double realTemp = chamberParams->getDryTemperature();
    double realHum = chamberParams->getHumidity();
    double setTemp = testPgm->getSteps().value(testPgm->getCurrentStep())->getTemperature();
    double setHum = testPgm->getSteps().value(testPgm->getCurrentStep())->getHumidity();

    controlCommands->setH1(h1On(setHum, realHum));
    controlCommands->setH2(h2On(setHum, realHum));
    controlCommands->setT1(t1On(setTemp, realTemp));
    controlCommands->setT2(t2On(setTemp, realTemp));
    controlCommands->setC1(true);
    controlCommands->setC2V2(true);
    controlCommands->setLNU(true);
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
    timer->start(3000);
}

void Controller::changeStep()
{
    int totalSteps = this->testPgm->getSteps().size();
    int nowStep = this->testPgm->getCurrentStep();
    if(nowStep < totalSteps){
        this->testPgm->setCurrentStep(nowStep + 1);
    }else if(nowStep == totalSteps){
        emit stepsFinished(totalSteps);
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

void Controller::startTest()
{
    setUpStart();
    controlTestRun();
}
