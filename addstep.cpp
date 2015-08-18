#include "addstep.h"
#include "ui_addstep.h"

AddStep::AddStep(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddStep)
{
    ui->setupUi(this);
    initStyle();
    //connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(close()));
}

AddStep::~AddStep()
{
    delete ui;
}

//! ********************** PRIVATE FUNCTIONS ************ !//

void AddStep::initStyle(){
    ui->hoursEdit->setValidator(new QIntValidator(0, 100, this));
    ui->minutesEdit->setValidator(new QIntValidator(0, 59, this));
    ui->tempEdit->setValidator(new QIntValidator(-40, 150, this));
    ui->humidityEdit->setValidator(new QIntValidator(0, 100, this));
    ui->waitEdit->setValidator(new QIntValidator(0, 1, this));
    ui->hrEdit->setValidator(new QIntValidator(0, 1, this));
    ui->oneEdit->setValidator(new QIntValidator(0, 1, this));
    ui->twoEdit->setValidator(new QIntValidator(0, 1, this));
    ui->threeEdit->setValidator(new QIntValidator(0, 1, this));
}

void AddStep::clearFields()
{
    ui->tempEdit->clear();
    ui->humidityEdit->clear();
    ui->hoursEdit->clear();
    ui->minutesEdit->clear();
    ui->waitEdit->clear();
    ui->hrEdit->clear();
    ui->oneEdit->clear();
    ui->twoEdit->clear();
    ui->threeEdit->clear();
}

//! ********************** END OF PRIVATE FUNCTIONS *********** !//


//! ********************** PRIVATE SLOTS ********************** !//

void AddStep::on_addButton_clicked()
{
    QString temp = ui->tempEdit->text();
    QString humid = ui->humidityEdit->text();
    QString hrs = ui->hoursEdit->text();
    QString mins = ui->minutesEdit->text();
    QString wait = ui->waitEdit->text();

    if(temp.isEmpty() || humid.isEmpty()
            || hrs.isEmpty() || mins.isEmpty()
            || wait.isEmpty() ){
        QMessageBox::information(this, "Note", "Please enter all necessary fields",
                                 QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }

    QString hr = ui->hrEdit->text();
    if(hr.isEmpty()) hr = "0";
    QString one = ui->oneEdit->text();
    if(one.isEmpty()) one = "0";
    QString two = ui->twoEdit->text();
    if(two.isEmpty()) two = "0";
    QString three = ui->threeEdit->text();
    if(three.isEmpty()) three = "0";
    emit stepFormSubmitted(temp, humid, hrs, mins, wait, hr, one, two, three);
    clearFields();
}




void AddStep::on_cancelButton_clicked()
{
    clearFields();
    this->close();
}
