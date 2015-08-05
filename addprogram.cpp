#include "addprogram.h"
#include "ui_addprogram.h"
#include <QDebug>

AddProgram::AddProgram(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddProgram)
{
    ui->setupUi(this);
    initStyle();
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(close()));
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

void AddProgram::initStyle(){
    ui->cycleEdit->setValidator(new QIntValidator(1, 100, this));
    //ui->programNameEdit->setValidator();

    Step *s1 = new Step();
    s1->setTemperature(25); s1->setHumidity(50); s1->setHours(1); s1->setMinutes(20);
    s1->setStepNumber(1); s1->setWaiting(Step::WAIT);
    QVariant variant;
    variant.setValue(s1);

    QStandardItemModel *model = new QStandardItemModel();
    QStandardItem *parentItem = model->invisibleRootItem();
    QStandardItem *item = new QStandardItem();
    item->setData(QVariant::fromValue(s1));
    parentItem->appendRow(item);



    ui->tableView->setModel(model);
}

void AddProgram::on_addStepButton_clicked()
{
    if(ui->programNameEdit->text().isEmpty() || ui->cycleEdit->text().isEmpty()){
        QMessageBox::information(this, "Note", "Program Name and Cycle must be added\nbefore adding a Step",
                                 QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
}
