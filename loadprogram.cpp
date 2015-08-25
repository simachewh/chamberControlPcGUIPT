#include "loadprogram.h"
#include "ui_loadprogram.h"

LoadProgram::LoadProgram(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoadProgram)
{
    ui->setupUi(this);
}

LoadProgram::~LoadProgram()
{
    delete ui;
}

void LoadProgram::setLoaded(Program *pgm)
{
    StepsModel *stm = new StepsModel();
    stm->setProgramToShow(pgm);
    ui->tableView->setModel(stm);
    ui->pgmNameLabel->setText(pgm->getProgramName());
}
