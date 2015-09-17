#ifndef PROGRAM_H
#define PROGRAM_H

#include <QObject>
#include <QMap>
#include "step.h"

class Program : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString programName READ getProgramName WRITE setProgramName NOTIFY programNameChanged)
    //Q_PROPERTY(int point READ getPoint WRITE setPoint NOTIFY pointChanged)
public:
    explicit Program(QObject *parent = 0);

    enum PGM_PARAM {Cycl, Curr_cycl, Stp, Curr_stp};

    //! ************* GETTERS AND SETTERS ************** !//
    QString getProgramName() const;

    void setProgramName(QString name);

    int getPoint() const;

    void setPoint(int value);

    int getCycle() const;

    void setCycle(int value);

    int getNoOfSteps() const;

    void setNoOfSteps(int value);

    int getCurrentCycle();

    int getCurrentStep();

    void setCurrentStep(int value);

    void setCurrentCycle(int value);

    QMap<int, Step*> getSteps() const;

    void setSteps(const QMap<int, Step *> value);

    int addStep(Step* s);

private:
    QString programName;
    int point;
    int cycle;
    int currentCycle;
    int noOfSteps;
    int currentStep;
    QMap<int, Step*> steps;
    Q_DISABLE_COPY(Program)

signals:
    void programNameChanged(QString);
    void pointChanged(int);
    void cycleChanged(int);
    void noOfStepsChanged(int);
    void stepsChanged();
    void stepsChanged(QString);
    void currentStepChanged(int);
    void currentCycleChanged(int);
    void programParamChanged(int, Program::PGM_PARAM);

public slots:

private slots:
    void on_stepsChanged();
};

#endif // PROGRAM_H
