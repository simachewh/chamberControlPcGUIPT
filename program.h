#ifndef PROGRAM_H
#define PROGRAM_H

#include <QObject>
#include <QMap>
#include "step.h"

#include <QDebug>

class Program : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString programName READ getProgramName WRITE setProgramName NOTIFY programNameChanged)
    //Q_PROPERTY(int point READ getPoint WRITE setPoint NOTIFY pointChanged)
public:
    explicit Program(QObject *parent = 0);
    ~Program();

    enum PGM_PARAM {Cycl, Curr_cycl, Stp, Curr_stp};

    /**
     * @brief getCurrentStep Gets the Current step being excuted by this
     * program.
     * @return If found it return a pointer to the current step, if not
     * returns 0 (NULL).
     */
    Step *getCurrentStep();

    /**
     * @brief getNextStep Gets the next step to be excuted in this program.
     * @return If found it return the a pointer to the next step, if not
     * return 0 (NULL)
     */
    Step *getNextStep();

    /**
     * @brief getPreviousStep Gets the previous step in this program.
     * It tries to find the step executed before the currennt one and
     * returns a pointer to it. If incase the current step is the
     * first one then it returns zero (NULL).
     * @return if found it returns a pointer to the previous step, if
     * not returns 0 (NULL)
     */
    Step *getPreviousStep();

    /**
     * @brief goToNextStep Tries to progress the step being excuted
     * by incrementing the currentStepNum. If current step has reached
     * the last step it will increment the currentCycle and set currentStepNum
     * to the begining, there by continuing excution of steps for the
     * next cycle. If the current Step was one of the middle steps it will
     * increment the currentStepNum. If the steps are all done
     * and all cylcles are done it will do nothing and return false showing
     * that excution of program has finished. In all other cases it returs true.
     * @return Returns true if it was able to move to the next step
     * and false if it has reached last step.
     */
    bool goToNextStep();

    /**
     * @brief getProgramName Gets the name of the program.
     * @return name of this program.
     */
    QString getProgramName() const;

    void setProgramName(QString name);

    int getPoint() const;

    void setPoint(int value);

    int getCycle() const;

    void setCycle(int value);

    int getNoOfSteps() const;

    void setNoOfSteps(int value);

    int getCurrentCycleNum();

    int getCurrentStepNum();

    void setCurrentStepNum(int value);

    void setCurrentCycle(int value);

    QMap<int, Step*> getSteps() const;

    void setSteps(const QMap<int, Step *> value);

    int addStep(Step* s);

    int removeStep(int key);
private:
    QString programName;
    int point;
    int cycle;
    int currentCycle;

    /**
     * @brief noOfSteps Represents the total number of steps
     * in this program. It is equal to the value that would
     * be found by steps.size()
     */
    int noOfSteps;

    /**
     * @brief currentStepNum Represents the step number that
     * is being excuted now. Step numbers span from zero to
     * the total number of steps minus one.
     */
    int currentStepNum;
    /**
     * @brief steps a property to hold the steps in this program.
     */
    QMap<int, Step*> *steps; //NOTE: consider changing this to QList, might be more efficeint
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
