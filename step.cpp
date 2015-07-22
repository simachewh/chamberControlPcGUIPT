#include "step.h"

Step::Step(QObject *parent) : QObject(parent)
{

}

//! ************* GETTERS AND SETTERS ******** !//

int Step::getStepNumber() const{
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

Step::WaitState Step::getWaiting() const{
    return waiting;
}

void Step::setWaiting(Step::WaitState value){
    if(waiting != value){
        waiting = value;
        emit waitingChanged(value);
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

