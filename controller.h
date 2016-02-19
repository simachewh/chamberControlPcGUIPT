#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QSettings>

#include "chamber.h"
#include "controlcommands.h"
#include "program.h"
#include "step.h"
#include "databackup.h"
#include "pid.h"

/**
 * @brief The Controller class is where control of a test program
 * happens using a PID control algorithm.
 * @author Simachew Tibebu
 */
class Controller : public QObject
{
    Q_OBJECT

private:

    DataBackup *db;

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
     * @brief temperaturePID A PID object that represents PID parameters and operations
     *  designated to control the temperature of the chamber.
     */
    PID *temperaturePID;

    /**
     * @brief humidityPID A PID object that represents PID parameters and operations
     *  designated to control the humidity of the chamber.
     */
    PID *humidityPID;

    QTimer *plotTimer;

public:

    static const QString Temp_P;
    static const QString Temp_I;
    static const QString Temp_D;
    static const QString Humid_P;
    static const QString Humid_I;
    static const QString Humid_D;
    static const QString PlotInterval;

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
    QTimer *stepTimer;

    explicit Controller(QObject *parent = 0);
    ~Controller();

    void controlTestRun();

    void setUpStart();

    void startTest(QString programName);

    void saveTempDefault(QString data);

    void saveHumidDefault(QString data);

    void removePIDDefaults(int choice);

    bool isDefaultSet();


    Step *getCurrentStep() const;

    Step *getPreviousStep() const;

    Step *getNextStep() const;

    Program *getTestPgm() const;
    void setTestPgm(Program *value);

    void startQuickTest(Program *pgm);
    PID *getTemperaturePID() const;
    void setTemperaturePID(PID *value);

    PID *getHumidityPID() const;
    void setHumidityPID(PID *value);

signals:
    void stepsFinished(int totalStepsDone);
    void stepsDone(bool);
    void controlready(ControlCommands::CH_COMMAND);
    void currentStepChanged();
    void nextStepChanged();
    void previousStepChanged();
    void targetReached();

public slots:
    void setNextStep(Step *value);

    void setPreviousStep(Step *value);

    void setCurrentStep(Step *value);

    void on_temperatureRealChanged(double value);

    void on_humidityRealChanged(double value);

    /**
     * @brief on_realValuesCollected This slot checks the collected real
     * values if they have reached the goal to be achived, in the error
     * ranges of 0.01 for temperature and 0.1 for humidity. If goal
     * is reached it sets the current step's completed property to
     * true, and emits targetReached() signal of this object.
     */
    void on_realValuesCollected();

    /**
     * @brief changeStep Moves the test one step forward based on
     * the wait value and the real values reaching the set value.
     * If the goal is reached befoer the given time of the step and
     * wait value is zero, the test moves a step forward. If goal was
     * reached in time but wait is on then the test waits till the time
     * is over while maintaining the set values. If goal was not achived
     * but there was no wait anyway, then the test just moves a step
     * forward.
     */
    void changeStep();

    /**
     * @brief on_stepChange This slot is to be called up on a progression of
     * step in test. It prepares the pid controllers to be
     * aware of the new step. Set values are set to the new one. This function
     * also makes a call to setUpStart(); and controlTestRun();
     */
    void on_stepChange();

    void on_controlRequested();

    /**
     * @brief runDeviceControll Controls the state of devices in the
     * chamber. Meaning devices are turned on and off here. The approach
     * is to use the temperature and humidity power outputs to determine
     * what device is needed for the job and change their state
     * accordingly.
     */
    void runDeviceControll();

    void on_stepsDone(bool);

    void on_stepsChanged();

    /**
     * @brief on_plotTimerOut A slot to record real values to file for later use in ploting.
     *  when the plot timer times out this function is appropriate to call.
     */
    void on_plotTimerOut();
};

#endif // CONTROLLER_H
