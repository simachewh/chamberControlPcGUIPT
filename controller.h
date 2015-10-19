#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QTimer>
#include <QDebug>

#include "chamber.h"
#include "controlcommands.h"
#include "program.h"
#include "step.h"
#include "databackup.h"

/**
 * @brief The Controller class is where control of a test program
 * happens using a PID control algorithm.
 * @author Simachew Tibebu
 */
class Controller : public QObject
{
    Q_OBJECT

private:

    enum {MAX_OUT = 255, MIN_OUT = 0};

    /**
     * @brief currentStep Representaion of the current step
     * in the runing program.
     */
    Step *currentStep;

    /**
     * @brief previousStep Representaion of the previous step
     * in the runing program.
     */
    Step *previousStep;

    /**
     * @brief nextStep Represents the next step in the
     * runing program.
     */
    Step *nextStep;

    /**
     * @brief previousTempError The previous error of temperature.
     */
    double previousTempError;

    /**
     * @brief tempError The error between the set and measured values
     * of temperature
     */
    double tempError;

    /**
     * @brief previousHumidError The previous error of Humidity.
     */
    double previousHumidError;

    /**
     * @brief humidError The error between the set and measured values
     * of humidity.
     */
    double humidError;

    /**
     * @brief temperatureSetValue Temperature set value of current
     * step.
     */
    double temperatureSetValue;

    /**
     * @brief HumiditySetValue Humidity set value of current step.
     */
    double HumiditySetValue;

    /**
     * @brief measuredTemp Temperature measured value;
     */
    double measuredTemp;

    /**
     * @brief measuredHumid Humidity measured value.
     */
    double measuredHumid;

    /**
     * @brief kp The proportional gain.
     */
    double kpTemp;

    /**
     * @brief ki The integral gain.
     */
    double kiTemp;

    /**
     * @brief kd The derivative gain.
     */
    double kdTemp;

    double integralTemp;

    double proportionalTemp;

    double derivativeTemp;

    double dtTemp;

    double kpHumd;

    double kiHumid;

    double kdHumid;

    double integralHumid;

    double proportionalHumid;

    double derivativeHumid;

    double dtHumid;



public:

    /**
     * @brief testPgm Represents the program runing in the
     * system. This is the program that is being controlled
     * by this controller. Once a program is started this
     * pointer is set to the runing program by the system.
     */
    Program *testPgm;

    /**
     * @brief chamberParams
     */
    Chamber *chamberParams;
    ControlCommands *controlCommands;
    QTimer *timer;

    explicit Controller(QObject *parent = 0);
    ~Controller();

    int pidTempControl();

    int pidHumidControl();

    void controlTestRun();

    void setUpStart();

    void startTest(QString programName);


    Step *getCurrentStep() const;
    void setCurrentStep(Step *value);

    Step *getPreviousStep() const;
    void setPreviousStep(Step *value);

    Step *getNextStep() const;
    void setNextStep(Step *value);

    double getPreviousTempError() const;
    void setPreviousTempError(double value);

    double getTempError() const;
    void setTempError(double value);

    double getPreviousHumidError() const;
    void setPreviousHumidError(double value);

    double getHumidError() const;
    void setHumidError(double value);

    double getTemperatureSetValue() const;
    void setTemperatureSetValue(double value);

    double getHumiditySetValue() const;
    void setHumiditySetValue(double value);

    double getMeasuredTemp() const;
    void setMeasuredTemp(double value);

    double getMeasuredHumid() const;
    void setMeasuredHumid(double value);

    double getKp() const;
    void setKp(double value);

    double getKi() const;
    void setKi(double value);

    double getKd() const;
    void setKd(double value);

    double getIntegral() const;
    void setIntegral(double value);

    double getProportional() const;
    void setProportional(double value);

    double getDerivative() const;
    void setDerivative(double value);

    double getDt() const;
    void setDt(double value);

    double getKpTemp() const;
    void setKpTemp(double value);

    double getKiTemp() const;
    void setKiTemp(double value);

    double getKdTemp() const;
    void setKdTemp(double value);

    double getIntegralTemp() const;
    void setIntegralTemp(double value);

    double getProportionalTemp() const;
    void setProportionalTemp(double value);

    double getDerivativeTemp() const;
    void setDerivativeTemp(double value);

    double getDtTemp() const;
    void setDtTemp(double value);

    double getKpHumd() const;
    void setKpHumd(double value);

    double getKiHumid() const;
    void setKiHumid(double value);

    double getKdHumid() const;
    void setKdHumid(double value);

    double getIntegralHumid() const;
    void setIntegralHumid(double value);

    double getProportionalHumid() const;
    void setProportionalHumid(double value);

    double getDerivativeHumid() const;
    void setDerivativeHumid(double value);

    double getDtHumid() const;
    void setDtHumid(double value);

signals:
    void stepsFinished(int totalStepsDone);
    void stepsDone(bool);
    void controlready(ControlCommands::CH_COMMAND);

public slots:    

    void changeStep();

    void on_stepChange();

    void on_controlRequested();

    void runDeviceControll();

    void on_stepsDone(bool);
};

#endif // CONTROLLER_H
