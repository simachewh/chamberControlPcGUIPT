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

namespace Ui {
class AddProgram;
}

class AddProgram : public QDialog
{
    Q_OBJECT

public:
    explicit AddProgram(QWidget *parent = 0);
    ~AddProgram();

private slots:
    void on_saveButton_clicked();

    void initStyle();

    void on_addStepButton_clicked();

private:
    Ui::AddProgram *ui;
};

#endif // ADDPROGRAM_H
