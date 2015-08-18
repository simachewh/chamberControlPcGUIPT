#ifndef ADDSTEP_H
#define ADDSTEP_H

#include <QDialog>
#include <QIntValidator>
#include <QMessageBox>

namespace Ui {
class AddStep;
}

class AddStep : public QDialog
{
    Q_OBJECT

public:
    explicit AddStep(QWidget *parent = 0);
    ~AddStep();
signals:
    void stepFormSubmitted(QString temp, QString humid, QString hrs,
                           QString mins, QString wait, QString hr,
                           QString one, QString two, QString three);

private slots:
    /**
     * @brief on_addButton_clicked call back slot to handel click event of
     * add step button.
     */
    void on_addButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::AddStep *ui;
    /**
     * @brief initStyle funciton to make initial styles when the add
     * step dialogue is created.
     */
    void initStyle();

    /**
     * @brief clearFields clears the contents of the form fields.
     */
    void clearFields();
};

#endif // ADDSTEP_H
