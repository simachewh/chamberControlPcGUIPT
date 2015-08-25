#ifndef LOADPROGRAM_H
#define LOADPROGRAM_H

#include <QDialog>
#include "program.h"
#include "stepsmodel.h"

namespace Ui {
class LoadProgram;
}

class LoadProgram : public QDialog
{
    Q_OBJECT

public:
    explicit LoadProgram(QWidget *parent = 0);
    ~LoadProgram();

    void setLoaded(Program *pgm);

private:
    Ui::LoadProgram *ui;
};

#endif // LOADPROGRAM_H
