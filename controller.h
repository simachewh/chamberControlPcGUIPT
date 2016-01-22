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

    static const QString Temp_P;
    static const QString Temp_I;
    static const QString Temp_D;
    static const QString Humid_P;
    static const QString Humid_I;
    static const QString Humid_D;

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

public slots:
    void setNextStep(Step *value);

    void setPreviousStep(Step *value);

    void setCurrentStep(Step *value);

    void on_TemperatureRealChanged(double value);

    void changeStep();

    void on_stepChange();

    void on_controlRequested();

    void runDeviceControll();

    void on_stepsDone(bool);

    void on_stepsChanged();
};

#endif // CONTROLLER_H
