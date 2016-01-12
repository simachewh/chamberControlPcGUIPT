#include "pid.h"
#include <QDataStream>
#include <QDebug>


double PID::getError() const
{
    return error;
}

void PID::setError(double value)
{
    error = value;
}

double PID::getIntegral() const
{
    return integral;
}

void PID::setIntegral(double value)
{
    integral = value;
}

double PID::getProportional() const
{
    return proportional;
}

void PID::setProportional(double value)
{
    proportional = value;
}

double PID::getDerivative() const
{
    return derivative;
}

void PID::setDerivative(double value)
{
    derivative = value;
}

double PID::getKi() const
{
    return ki;
}

void PID::setKi(double value)
{
    ki = value;
}

double PID::getKp() const
{
    return kp;
}

void PID::setKp(double value)
{
    kp = value;
}

double PID::getKd() const
{
    return kd;
}

void PID::setKd(double value)
{
    kd = value;
}

double PID::getDt() const
{
    return dt;
}

void PID::setDt(double value)
{
    dt = value;
}

double PID::getSetValue() const
{
    return setValue;
}

void PID::setSetValue(double value)
{
    setValue = value;
}

double PID::getMeasuredValue() const
{
    return measuredValue;
}

void PID::setMeasuredValue(double value)
{
    measuredValue = value;
}
PID::PID(QObject *parent) : QObject(parent)
{
    proportional = integral = derivative = 0;
    error = previousError = 0;
    dt = 1;
    ki = 1;
    kd = 1;
    kp = 1;
}

PID::PID(const PID &other)
{
    kp = other.getKp();
    ki = other.getKi();
    kd = other.getKd();
}

PID &PID::operator=(const PID &other)
{
    kp = other.getKp();
    ki = other.getKi();
    kd = other.getKd();
}

int PID::control()
{
    error = setValue - measuredValue;
    integral = integral + (error * dt);
    derivative = (error - previousError) / dt;
    proportional = kp * error;

    int output = proportional + (ki * integral) + (kd * derivative);
    previousError = error;

    if(output < MIN_OUT){
        output = MIN_OUT;
    }else if(output > MAX_OUT){
        output = MAX_OUT;
    }
    return output;
}

QString PID::toString() const
{
    return "" + QString::number(kp) + "\t " + QString::number(ki)
            + "\t " + QString::number(kd);
}


double PID::getPreviousError() const
{
    return previousError;
}

void PID::setPreviousError(double value)
{
    previousError = value;
}


QDataStream &operator <<(QDataStream &out, const PID &pid)
{
    out << pid.getKp() << pid.getKi() << pid.getKd();
    return out;
}


QDataStream &operator >>(QDataStream &in, PID &pid)
{
    double kp;
    double ki;
    double kd;
    in >> kp;
    in >> ki;
    in >> kd;
    pid.setKp(kp);
    pid.setKi(ki);
    pid.setKd(kd);
    return in;
}
