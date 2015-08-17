#ifndef ADDPROGRAM_H
#define ADDPROGRAM_H

#include <QWidget>
#include <QIntValidator>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QMessageBox>

#include "program.h"
#include "databackup.h"
#include "step.h"
#include "addstep.h"

namespace Ui {
class AddProgram;
}

class AddProgram : public QDialog
{
    Q_OBJECT

public:
    explicit AddProgram(QWidget *parent = 0);
    ~AddProgram();

signals:
    void stepAdded();

private slots:
    void on_saveButton_clicked();

    /**
     * @brief initStyle function to setup initial styles before while creating
     * the add program dialogue.
     */
    void initStyle();

    /**
     * @brief on_addStepButton_clicked slot handels a clicked() event of
     * the plus button for steps in the add program dialogue. This function
     * creates a program object, sets it's name and cycle and ties to save it
     * to a file. If a program by that name exists it will show an information
     * dialogue to request the user to change the name or replace the existing
     * program. Based on users response to the qustion the program name is ether
     * changed or the saved program is replaced.
     */
    void on_addStepButton_clicked();

    /**
     * @brief on_stepFormSubmited this slot handels the form submitted up on
     * adding a step to a program. It creates a step object, initializes
     * it with the given params, then calls the addStep(Step *step) function
     * that will add the step to the steps map of the program which inturn will make
     * a call to writeStepToFile function of the DataBackup class to append the step
     * to a file where the program is stored.
     * @param temp
     * @param humid
     * @param hrs
     * @param mins
     * @param wait
     * @param hr
     * @param one
     * @param two
     * @param three
     */
    void on_stepFormSubmited(QString temp, QString humid, QString hrs,
                   QString mins, QString wait, QString hr,
                   QString one, QString two, QString three);

//    bool addStep(QString name, Step * step);

//    bool addStep(Program * prgm, Step *step);
    /**
     * @brief addStep adds this step to the steps map of the current program
     * at the index of step->stepNumber, and makes a call to the writeStepToFile
     * function fo the DataBackup class to append the step to a file where the
     * program is being stored.
     * @param step
     * @return
     */
    bool addStep(Step *step);

private:
    Ui::AddProgram *ui;
    AddStep *as;
    Program *p;
    Step *step;

};

#endif // ADDPROGRAM_H
