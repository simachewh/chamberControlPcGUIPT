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
     * @brief on_stepFormSubmited slot handels the form submitted up on
     * adding a step to a program. It creates a step object, initializes
     * it with the given params, and add it to the steps map of the
     * program adding this step.
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

private:
    Ui::AddProgram *ui;
    AddStep *as;
    Program *p;
    Step *step;

};

#endif // ADDPROGRAM_H
