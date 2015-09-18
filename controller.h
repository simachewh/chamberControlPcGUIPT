#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QTimer>
#include <QDebug>

#include "chamber.h"
#include "controlcommands.h"
#include "program.h"

class Controller : public QObject
{
    Q_OBJECT
public:

    Program *testPgm;
    Chamber *chamberParams;
    ControlCommands *controlCommands;


    explicit Controller(QObject *parent = 0);
    ~Controller();

    bool h1On(double setValue, double feedback);

    bool h2On(double setValue, double feedback);

    bool t1On(double tsetVal, double tfeedback);

    bool t2On(double tsetVal, double tfeedback);

//    bool c2v2On(double tsetValue, double tfeedback);

//    bool c1On(double hsetVal, double hfedback,
//              double tsetVal, double tfeedback);

//    bool v1On(double tsetVal, double tfeedback);

//    bool v3On(double tsetVal, double tfeedback);

//    bool v4On(double tsetval, double tfeedback);

//    bool p1On(double setVal, double feedback);

//    bool p2On(double setVal, double feedback);

//    bool p3On(double setVal, double feedback);

//    int temperaturePowerOut(double tsetVal, double tfeedback);

    QTimer *timer;

    void controlTestRun();

    void setUpStart();

    void startTest();






signals:
    void stepsFinished(int totalStepsDone);
    void stepsDone(bool);
    void controlready(ControlCommands::CH_COMMAND);

public slots:    

    void changeStep();

    void on_stepChange();

    void on_controlRequested();
};

#endif // CONTROLLER_H
