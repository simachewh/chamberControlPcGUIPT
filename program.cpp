#include "program.h"

Program::Program(QObject *parent) : QObject(parent)
{

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
        emit cycleChanged(value);
    }
}

int Program::getNoOfSteps() const{
    return noOfSteps;
}

void Program::setNoOfSteps(int value){
    if(noOfSteps != value){
        noOfSteps = value;
        emit noOfStepsChanged(value);
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
