#include "step.h"

Step::Step(QObject *parent) : QObject(parent)
{

}

int Step::getStepNumber() const{
    qDebug() << "getStepNumber: step no " << stepNumber;
    return stepNumber;
}

void Step::setStepNumber(int value){
    if(stepNumber != value){
        stepNumber = value;
        emit stepNumberChanged(value);
    }
}

double Step::getTemperature() const{
    return temperature;
}

void Step::setTemperature(double value){
    if(temperature != value){
        temperature = value;
        emit temperatureChanged(value);
    }
}

double Step::getHumidity() const{
    return humidity;
}

void Step::setHumidity(double value){
    if(humidity != value){
        humidity = value;
        emit humidityChanged(value);
    }
}

int Step::getHours() const{
    return hours;
}

void Step::setHours(int value){
    if(hours != value){
        hours = value;
        emit hoursChanged(value);
    }
}

int Step::getMinutes() const{
    return minutes;
}

void Step::setMinutes(int value){
    if(minutes != value){
        minutes = value;
        emit minutesChanged(value);
    }
}

int Step::getWaiting() const{
    return waiting;
}

void Step::setWaiting(int value){
    if(waiting != value){
        waiting = value;
        emit waitingChanged(value);
    }
}

int Step::getHR() const{
    return hR;
}

void Step::setHR(int value){
    if(hR != value){
        hR = value;
        emit hRChanged(value);
    }
}

int Step::getOne() const {
    return one;
}

int Step::getTwo() const{
    return two;
}

int Step::getThree() const{
    return three;
}

void Step::setOne(int value){
    if(one != value){
        one = value;
        emit oneChanged(value);
    }
}

void Step::setTwo(int value){
    if(two != value){
        two = value;
        emit twoChanged(value);
    }
}

void Step::setThree(int value){
    if(three != value){
        three = value;
        emit threeChanged(value);
    }
}

bool Step::operator ==(const Step &step){
    return stepNumber == step.stepNumber &&
            temperature == step.temperature &&
            humidity == step.humidity &&
            hours == step.hours &&
            minutes == step.minutes &&
            waiting == step.waiting;
}

