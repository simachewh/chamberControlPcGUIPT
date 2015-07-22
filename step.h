#ifndef STEP_H
#define STEP_H

#include <QObject>

class Step : public QObject
{
    Q_OBJECT
    Q_ENUMS(WaitState)
public:
    enum WaitState {WAIT = 1, NO_WAIT = 0};
    explicit Step(QObject *parent = 0);

    //! ***************** GETTERS AND SETTERS ***************** !//
    int getStepNumber() const;

    void setStepNumber(int value);

    double getTemperature() const;

    void setTemperature(double value);

    double getHumidity() const;

    void setHumidity(double value);

    int getHours() const;

    void setHours(int value);

    int getMinutes() const;

    void setMinutes(int value);

    Step::WaitState getWaiting() const;

    void setWaiting(WaitState value);

    bool operator ==(const Step &step);

private:

    int stepNumber;
    double temperature;
    double humidity;
    double hours;
    double minutes;
    WaitState waiting;

signals:
    void stepNumberChanged(int);
    void temperatureChanged(double);
    void humidityChanged(double);
    void hoursChanged(int);
    void minutesChanged(int);
    void waitingChanged(WaitState);

public slots:
};

#endif // STEP_H
