#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QTimer>
#include <QDebug>

#include "chamber.h"
#include "controlcommands.h"
#include "program.h"

/**
 * @brief The Controller class
 * @author Simachew Tibebu
 */
class Controller : public QObject
{
    Q_OBJECT

private:
    //! not quite sure if this is a good idea
    //! to have copies of these private params
    //! that are already accecible from the Program
    //! object of this class
//    int nowStep;
//    int totalStep;
//    int nowCycle;
//    int totalCycle;

//    double setTemp;
//    double setHum;

public:

    Program *testPgm;
    Chamber *chamberParams;
    ControlCommands *controlCommands;
    QTimer *timer;

    explicit Controller(QObject *parent = 0);
    ~Controller();    

    void controlTestRun();

    void setUpStart();

    void startTest();

    //    int getNowStep();
    //    int getNowCycle();
    //    int getTotalSteps();
    //    int getTotalCycle();
    //    double getSetTemp();
    //    double getSetHum();

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
