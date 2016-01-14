#ifndef PID_H
#define PID_H

#include <QObject>
#include <QDataStream>

class PID : public QObject
{
    Q_OBJECT
//TODO: NO reason to subclass QObject so far.
//NOTE: Maybe a bool value(true for default) then can change the background
    //color of the default PID in the view

private:
    enum {MAX_OUT = 255, MIN_OUT = 0};
    bool choosen;
    double error;
    double previousError;

    double integral;
    double proportional;
    double derivative;

    double ki;
    double kp;
    double kd;

    double dt;

    double setValue;
    double measuredValue;

public:
    explicit PID(QObject *parent = 0);
         PID(const PID &other);

         PID &operator=(const PID &other);

    int control();

    QString toString() const;

    double getError() const;
    void setError(double value);

    double getPreviousError() const;
    void setPreviousError(double value);

    double getIntegral() const;
    void setIntegral(double value);

    double getProportional() const;
    void setProportional(double value);

    double getDerivative() const;
    void setDerivative(double value);

    double getKi() const;
    void setKi(double value);

    double getKp() const;
    void setKp(double value);

    double getKd() const;
    void setKd(double value);

    double getDt() const;
    void setDt(double value);

    double getSetValue() const;    

    double getMeasuredValue() const;

    bool getChoosen() const;
    void setChoosen(bool value);

signals:

public slots:
    void setMeasuredValue(double value);

    void setSetValue(double value);

};

QDataStream &operator <<(QDataStream &out, const PID &pid);
QDataStream &operator >>(QDataStream &in, PID &pid);

#endif // PID_H
