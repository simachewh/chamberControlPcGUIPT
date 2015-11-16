#ifndef STEP_H
#define STEP_H

#include <QObject>
#include <QDebug>

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

    int getWaiting() const;

    void setWaiting(int value);

    int getHR() const;

    void setHR(int value);

    int getOne() const;

    int getTwo() const;

    int getThree() const;

    void setOne(int value);

    void setTwo(int value);

    void setThree(int value);
    //!*************** END OF GETTERS AND SETTERS **************!//
    /**
     * @brief operator == . Overloaded operator
     * @param step
     * @return
     */
    bool operator ==(const Step &step);

private:

    int stepNumber;
    double temperature;
    double humidity;
    double hours;
    double minutes;
    //this is changed from int to bool, follow possible issues.
    bool waiting;

    int hR;
    int one;
    int two;
    int three;

signals:
    void stepNumberChanged(int);
    void temperatureChanged(double);
    void humidityChanged(double);
    void hoursChanged(int);
    void minutesChanged(int);
    void waitingChanged(int);

    void hRChanged(int);
    void oneChanged(int);
    void twoChanged(int);
    void threeChanged(int);

public slots:
};

Q_DECLARE_METATYPE(Step*)
#endif // STEP_H
