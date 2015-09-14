#ifndef RENAMEDIALOG_H
#define RENAMEDIALOG_H

#include <QDialog>
#include <QMessageBox>

#include "databackup.h"

namespace Ui {
class RenameDialog;
}

class RenameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RenameDialog(QWidget *parent = 0);
    ~RenameDialog();

    /**
     * @brief setMessage Sets the message to be displayed informing the user what
     * Program is about to be renamed. Include the name of the program in the string.
     * @param msg the string to be displayed when
     */
    void setMessage(QString msg);

    /**
     * @brief getOldName gets the name to be changed. It is an empty string if it is not
     * changed upon selection of the item to rename.
     * @return
     */
    QString getOldName() const;

    /**
     * @brief setOldName sets the oldName field. set the old name field to the name that needs
     * to be renamed upon selection of the item.
     * @param value
     */
    void setOldName(QString value);
private slots:
    void on_okButton_clicked();

private:
    Ui::RenameDialog *ui;

    /**
     * @brief oldName holds value of the name of program to rename.
     */
    QString oldName;
};

#endif // RENAMEDIALOG_H
