#include "addprogram.h"
#include "ui_addprogram.h"
#include <QDebug>

AddProgram::AddProgram(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddProgram)
{
    ui->setupUi(this);
}

AddProgram::~AddProgram()
{
    delete ui;
}

void AddProgram::on_saveButton_clicked()
{
    QString programName = ui->programNameEdit->text();
    Program *p = new Program();
    p->setProgramName(programName);
    DataBackup *db = new DataBackup();
    db->saveProgram(p);
    qDebug() << "saved Program " << p->getProgramName();

    this->close();
}
