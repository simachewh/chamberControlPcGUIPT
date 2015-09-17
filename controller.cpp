#include "controller.h"

Controller::Controller(QObject *parent) : QObject(parent)
{
    timer = new QTimer(parent);
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

void Controller::conrtolTestRun(ControlPC *controlParams, Chamber *climateChamber, Program *testPgm)
{
    double realTemp = climateChamber->getDryTemperature();
    double realHum = climateChamber->getHumidity();
    double setTemp = testPgm->getSteps().value(testPgm->getCurrentStep())->getTemperature();
    double setHum = testPgm->getSteps().value(testPgm->getCurrentStep())->getHumidity();

    controlParams->setH1(h1On(setHum, realHum));
    controlParams->setH2(h2On(setHum, realHum));
    controlParams->setT1(t1On(setTemp, realTemp));
    controlParams->setT2(t2On(setTemp, realTemp));
}

void Controller::setUpStart(ControlPC *conrolParams, Chamber *climateChamber, Program *testPgm)
{
    timer->setSingleShot(true);
    int hrs = testPgm->getSteps().value(testPgm->getCurrentStep())->getHours();
    int min = testPgm->getSteps().value(testPgm->getCurrentStep())->getMinutes();
    int timeOut = hrs * 3600000;
    timeOut += (min * 60000);
    timer->start(timeOut);
}

void Controller::changeStep()
{

}
