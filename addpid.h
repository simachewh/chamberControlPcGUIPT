#ifndef ADDPID_H
#define ADDPID_H

#include <QDialog>
#include "databackup.h"

namespace Ui {
class AddPid;
}
/**
 * @brief The AddPid class
 */
class AddPid : public QDialog
{
    Q_OBJECT

public:
    explicit AddPid(QWidget *parent = 0);
    ~AddPid();
signals:
    void formSubmited(double p, double i, double d, int choice);

private slots:
    void on_buttonBox_accepted();

    void on_tempRadioButton_clicked(bool checked);

    void on_humidRadioButton_clicked(bool checked);

private:
    Ui::AddPid *ui;
    /**
     * @brief choice An integer to represent the choice of pid to add.
     * Value is set 1 for temperature PID and 0 for humidity PID.
     */
    int choice;
    DataBackup db;
};

#endif // ADDPID_H
