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

    PID *temperaturePID;

    PID *humidityPID;

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

    void controlTestRun();

    void setUpStart();

    void startTest(QString programName);


    Step *getCurrentStep() const;
    void setCurrentStep(Step *value);

    Step *getPreviousStep() const;
    void setPreviousStep(Step *value);

    Step *getNextStep() const;
    void setNextStep(Step *value);

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

public slots:

    void on_TemperatureRealChanged(double value);

    void changeStep();

    void on_stepChange();

    void on_controlRequested();

    void runDeviceControll();

    void on_stepsDone(bool);
};

#endif // CONTROLLER_H
