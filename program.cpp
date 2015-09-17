#include "program.h"

Program::Program(QObject *parent) : QObject(parent)
{
    currentCycle = 0;
    currentStep = 0;

    connect(this, SIGNAL(stepsChanged()),
            this, SLOT(on_stepsChanged()));
}

int Program::getPoint() const{
    return point;
}

void Program::setPoint(int value){
    if(point != value){
        point = value;
        emit pointChanged(value);
    }
}

QString Program::getProgramName() const{
    return programName;
}

void Program::setProgramName(QString name){
    if(programName != name){
        programName = name;
        emit programNameChanged(name);
    }

}

int Program::getCycle() const{
    return cycle;
}

void Program::setCycle(int value){
    if(cycle != value){
        cycle = value;
        emit programParamChanged(value, Cycl);
    }
}

int Program::getNoOfSteps() const{
    return steps.size();
}

void Program::setNoOfSteps(int value){
    if(noOfSteps != value){
        noOfSteps = value;
        emit programParamChanged(value, Stp);
    }
}

int Program::getCurrentCycle()
{
    return currentCycle;
}

int Program::getCurrentStep()
{
    return currentStep;
}

void Program::setCurrentStep(int value)
{
    if(currentStep != value){
        currentStep = value;
        emit programParamChanged(value, Curr_stp);
    }
}

void Program::setCurrentCycle(int value)
{
    if(currentCycle != value){
        currentCycle = value;
        emit programParamChanged(value, Curr_cycl);
    }
}

QMap<int, Step *> Program::getSteps() const{
    return steps;
}

void Program::setSteps(QMap<int, Step *> value){
    if(steps !=(value)){
        steps = value;
        emit stepsChanged();
    }
}

int Program::addStep(Step *s){
    int insertIndex = steps.size() + 1;
    steps.insert(insertIndex, s);
    int stepsSize = steps.size();
    if(insertIndex == stepsSize){
        emit stepsChanged();
    }
    return stepsSize;
}

void Program::on_stepsChanged()
{
    setNoOfSteps(steps.size());
}
