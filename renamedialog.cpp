#include "renamedialog.h"
#include "ui_renamedialog.h"

RenameDialog::RenameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RenameDialog)
{
    ui->setupUi(this);
    oldName = "";
}

RenameDialog::~RenameDialog()
{
    delete ui;
}

void RenameDialog::setMessage(QString msg)
{
    ui->label->setText(msg);
}

QString RenameDialog::getOldName() const
{
    return oldName;
}

void RenameDialog::setOldName(QString value)
{
    oldName = value;
}

void RenameDialog::on_okButton_clicked()
{
    QString newName(ui->lineEdit->text());
    newName = newName.trimmed();
    if(newName.isEmpty()){
        QMessageBox::information(this, "Note", "Please fill in the new name",
                                 QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }

    DataBackup db;
    bool renamed = db.renameProgram(newName, getOldName());

    if(renamed){
        QMessageBox::information(this, "Success", "Program has been renamed to " + newName,
                                 QMessageBox::Ok, QMessageBox::NoButton);
        this->close();
    }else{
        QMessageBox::information(this, "Failure", "Renaming failed",
                                 QMessageBox::Ok, QMessageBox::NoButton);
        this->close();
        return;
    }

}
