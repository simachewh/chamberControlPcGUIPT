#include "addprogram.h"
#include "ui_addprogram.h"
#include <QDebug>

AddProgram::AddProgram(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddProgram)
{
    ui->setupUi(this);
    initStyle();

    as = new AddStep(parent);
    p = new Program(parent);
    step = new Step(parent);

    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(as, SIGNAL(stepFormSubmitted(QString,QString,QString,QString,QString,QString,QString,QString,QString)),
            this, SLOT(on_stepFormSubmited(QString,QString,QString,QString,QString,QString,QString,QString,QString)));
}

AddProgram::~AddProgram()
{
    delete ui;
    delete step;
    delete p;
    delete as;
}

void AddProgram::on_saveButton_clicked()
{
//    Program *p = new Program();


//    QString programName = ui->programNameEdit->text();
//    p->setProgramName(programName);
//    DataBackup *db = new DataBackup();
//    db->saveProgram(p);
//    qDebug() << "saved Program " << p->getProgramName();

    this->close();
}

void AddProgram::initStyle(){
    ui->cycleEdit->setValidator(new QIntValidator(1, 100, this));
}

void AddProgram::on_addStepButton_clicked()
{
    if(ui->programNameEdit->text().isEmpty() || ui->cycleEdit->text().isEmpty()){
        QMessageBox::information(this, "Note", "Program Name and Cycle must be "
                                               "added\nbefore adding a Step",
                                 QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }


    QString pName = ui->programNameEdit->text();
    pName = pName.trimmed();
    QString cycle = ui->cycleEdit->text();
    cycle = cycle.trimmed();
    int cy = cycle.toInt();
    QMessageBox::StandardButton reply;
    DataBackup *dB = new DataBackup();

//! check if a program by that name exists and deal with it accordingly !//

    bool pLives = dB->programExists(pName);
    //!if this program is saved in file
    if(pLives)
    {
        //! and if the program to be added is not the current program
        if(p->getProgramName() != pName && p->getCycle() != cy)
        {
            //! ask the user to overwrite the file or not
            QString text = "Program by the name " + pName + " already exists.\n"
                                              "Do you want to replace it?";
            reply = QMessageBox::question(this, "Warning", text,
                                  QMessageBox::Yes | QMessageBox::No,
                                          QMessageBox::No);

            if(reply == QMessageBox::Yes)
            {
                ///save the program
                p->setProgramName(pName);
                p->setCycle(cy);
                dB->saveProgram(p);
            }else{
                return;
            }
        }
        else //! if other wise it is the same program
        {
            //! continue adding steps
            p->setProgramName(pName);
            p->setCycle(cy);
        }
    }
    else //! if other wise the program is not in file
    {
        //! initialize and save it to file
        p->setProgramName(pName);
        p->setCycle(cy);
        dB->saveProgram(p);
    }

    as->setModal(true);
    as->show();
}

//! ********************** PRIVATE SLOTS ********************** !//

void AddProgram::on_stepFormSubmited(QString temp, QString humid, QString hrs,
                           QString mins, QString wait, QString hr,
                           QString one, QString two, QString three)
{
    Step *s = new Step();
    s->setTemperature(temp.toDouble());
    s->setHumidity(humid.toDouble());
    s->setHours(hrs.toDouble());
    s->setMinutes(mins.toDouble());
    s->setWaiting(wait.toInt());
    s->setHR(hr.toInt());
    s->setOne(one.toInt());
    s->setTwo(two.toInt());
    s->setThree(three.toInt());
    s->setStepNumber(p->getSteps().size() + 1);

    addStep(s);
    qDebug() <<"on_stepFormSubmited: STEPS SIZE" <<  p->getSteps().size();
    QMap<int, Step*> st = p->getSteps();
    foreach (Step *s, st) {
        qDebug() << "on_stepFormSubmited: " << s->getTemperature();
    }

}

bool AddProgram::addStep(Step *step)
{
    bool isAdded = false;
    DataBackup *db = new DataBackup();

    QString prgmName = p->getProgramName();
    p->getSteps().insert(p->getSteps().size() + 1, step);

    QString path = db->fileLives(DataBackup::PRGM, prgmName);
    QFile prgmFile(path);

    if(path.isEmpty()){
        isAdded = false;
        return isAdded;
    }

    p->addStep(step);
    db->writeStepToFile(step, p);

    return isAdded;
}
