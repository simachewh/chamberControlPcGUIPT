#include "addpid.h"
#include "ui_addpid.h"

#include <QMessageBox>

AddPid::AddPid(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddPid)
{
    ui->setupUi(this);
    connect(this, SIGNAL(formSubmited(double,double,double,int)),
            &db, SLOT(on_pidFormSubmited(double, double, double, int)));
    choice = 0;
}

AddPid::~AddPid()
{
    delete ui;
}

void AddPid::on_buttonBox_accepted()
{
    double p = ui->pDoubleSpinBox->text().toDouble();
    double i = ui->iDoubleSpinBox->text().toDouble();
    double d = ui->dDoubleSpinBox->text().toDouble();
    if(p == 0 || i == 0 || d == 0){
        QMessageBox::warning(this, "Empty fields", "Fields can not be empty!");
        return;
    }
    emit formSubmited(p, i, d, choice);
}

void AddPid::on_tempRadioButton_clicked(bool checked)
{
    if(checked){
        choice = 0;
    }
}

void AddPid::on_humidRadioButton_clicked(bool checked)
{
    if(checked){
        choice = 1;
    }
}
