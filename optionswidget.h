#ifndef OPTIONSWIDGET_H
#define OPTIONSWIDGET_H

#include <QWidget>
#include <QSettings>
#include <QMessageBox>

#include "databackup.h"
#include "pidlistmodel.h"
#include "communication.h"


namespace Ui {
class OptionsWidget;
}

class OptionsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OptionsWidget(QWidget *parent = 0);
    ~OptionsWidget();

private:
    Ui::OptionsWidget *ui;
    enum Pid_Tab_Index {Temperature_Index = 0, Humidity_Index = 1};
    enum OPTIONS_PAGE_INDEX {SYS_INFO = 0, SYS_PARAM = 1, CONTROL_PARAM = 2};


    void populateTempPID();
    void populateHumidPID();

public slots:
private slots:
    void on_sysInfoToolButton_clicked();
    void on_sysParamToolButton_clicked();
    void on_controlParamToolButton_clicked();
};

#endif // OPTIONSWIDGET_H
