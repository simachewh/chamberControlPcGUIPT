#ifndef ADDPROGRAM_H
#define ADDPROGRAM_H

#include <QWidget>
#include "program.h"
#include "databackup.h"

namespace Ui {
class AddProgram;
}

class AddProgram : public QWidget
{
    Q_OBJECT

public:
    explicit AddProgram(QWidget *parent = 0);
    ~AddProgram();

private slots:
    void on_saveButton_clicked();

private:
    Ui::AddProgram *ui;
};

#endif // ADDPROGRAM_H
