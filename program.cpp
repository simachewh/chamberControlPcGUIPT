#include "program.h"

Program::Program(QObject *parent) : QObject(parent)
{
    currentCycle = 0;
    currentStepNum = 0;
    steps = new QMap<int, Step*>;

    connect(this, SIGNAL(stepsChanged()),
            this, SLOT(on_stepsChanged()));
}

Step *Program::getCurrentStep()
{
    return steps->value(getCurrentStepNum(), 0);
}

Step *Program::getNextStep()
{
        return steps->value(getCurrentStepNum() + 1, 0);
}

Step *Program::getPreviousStep()
{
    return steps->value(getCurrentStepNum() - 1, 0);
}

bool Program::goToNextStep()
{
    qDebug() << "cycle " << cycle
             << "Curr cycl " << currentCycle
             << "no of steps " << noOfSteps
             << "curr step " << currentStepNum;
    ///if step and cycle are the last ones
    if(currentCycle == cycle && currentStepNum == noOfSteps){
        qDebug() << "Finnnnished";
        return false;
    }
    ///if cycle is not the last one but step is the last for this cycle
    if(currentStepNum == noOfSteps){
        qDebug() << "Nextttt cyc";
        setCurrentStepNum(1);
        setCurrentCycle(currentCycle + 1);
        return true;
    }
    qDebug() << "Nexttt";
    setCurrentStepNum(currentStepNum + 1);
    return true;
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
    return steps->size();
}

void Program::setNoOfSteps(int value){
    if(noOfSteps != value){
        noOfSteps = value;
        emit programParamChanged(value, Stp);
    }
}

int Program::getCurrentCycleNum()
{
    return currentCycle;
}

int Program::getCurrentStepNum()
{
    return currentStepNum;
}

void Program::setCurrentStepNum(int value)
{
    if(currentStepNum != value && value <= noOfSteps){
        currentStepNum = value;
        emit programParamChanged(value, Curr_stp);
        emit currentStepChanged(value);
    }
}

void Program::setCurrentCycle(int value)
{
    if(currentCycle != value && value <= cycle){
        currentCycle = value;
        emit programParamChanged(value, Curr_cycl);
        emit currentCycleChanged(value);
    }
}

QMap<int, Step *> Program::getSteps() const{
    return *steps;
}

void Program::setSteps(QMap<int, Step *> value){
    if(*steps !=(value)){
        *steps = value;
        emit stepsChanged();
    }
}

int Program::removeStep(int key){
    int sizeBefore = steps->size();
    int retVal = steps->remove(key);

    QList<Step*> stp = steps->values();
    steps->clear();
    int i = 0;
    foreach (Step *s, stp) {
        s->setStepNumber(i);
        steps->insert(i, s);
        i++;
    }

    int sizeAfter = steps->size();
    if(sizeBefore > sizeAfter){
        emit stepsChanged();
    }
    return retVal;
}

int Program::addStep(Step *s){
    int insertIndex = steps->size();
    steps->insert(insertIndex, s);
    int stepsSize = steps->size();
    if(insertIndex == stepsSize){
        emit stepsChanged();
    }
    return stepsSize;
}

void Program::on_stepsChanged()
{
    setNoOfSteps(steps->size());
}
