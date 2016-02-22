#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QFileSystemModel>
#include <QSettings>

#include "controlcommands.h"
#include "communication.h"
#include "chamber.h"
#include "addprogram.h"
#include "databackup.h"
#include "stepsmodel.h"
#include "loadprogram.h"
#include "renamedialog.h"
#include "optionswidget.h"
#include "addpid.h"
#include "pidlistmodel.h"

#include "qcustomplot.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void monitorProgramInit();
    void helpProgramInit();
    void monitorHelpInit();
    void programTabInit();
    void monitorTabInit();
    void optionsTabInit();
    void quickStartTabInit();
    void auxTabInit();
    void populateTempPID();
    void populateHumidPID();
signals:
    void quickStepAboutToAdd();
    void testStoped(bool);
private slots:

    void initStyle();

    void on_newProgramButton_clicked();

    void on_programsListView_clicked(const QModelIndex &index);

    void on_loadProgramButton_clicked();

    void on_partsChanged(bool value, ControlCommands::CH_PART part);

    void on_tabWidget_currentChanged(int index);

    void on_addStepOnSelectedButton_clicked();

    void on_removeStepFromSelectedButton_clicked();

    void on_renameProgramButton_clicked();

    void on_deleteProgramButton_clicked();

    void on_testFinished();

    void on_stopButton_clicked();

    void on_startButton_clicked();

    void on_connectionLost(bool disconnected);

    void on_stepsTableView_clicked(const QModelIndex &index);

    void on_quickStartButton_clicked(bool checked);

    void on_idleStateChanged(bool state);

    void on_quickStopButton_clicked();

    void on_sysInfoToolButton_clicked(bool checked);

    void on_sysParamToolButton_clicked(bool checked);

    void on_controlParamToolButton_clicked(bool checked);

    void on_quickAddStepButton_clicked();

    void on_quickStepAboutToAdd();

    void on_makeDefaultButton_clicked();

    void on_plusHPButton_clicked();

    void on_pidTabWidget_currentChanged(int index);

    void on_minusHPButton_clicked();

    void on_tempPIDListView_clicked(const QModelIndex &index);

    void on_newPIDAdded(double p, double i, double d, int choice);

    void on_pointLineEdit_textEdited(const QString &arg1);

    void on_maxLowLineEdit_textEdited(const QString &arg1);

    void on_maxHighLineEdit_textEdited(const QString &arg1);

    void on_sysParamChangesButto_clicked();

    void on_updateIntervalButton_clicked();

    void on_viewButton_clicked();

    void on_intervalComboBox_currentIndexChanged(const QString &arg1);

    void on_serialPortComboBox_currentIndexChanged(int index);

    void on_serialPortNameUpdateButton_clicked();

public slots:
    void populateProgramsList();

    void populatePlotList();

    void on_humidPowerChange(int value);
    void on_tempPowerChange(int value);

    void on_testProgramNameChanged(QString value);

    void on_testProgramParamChanged(int value, Program::PGM_PARAM param);

private:
    Ui::MainWindow *ui;
    Communication *communication;

    //Program *quickPgm; // removed for resons explained in the constructor
    //check details at in the implementation file. (.cpp)

    enum MAIN_TAB_INDEX {MONITOR_INDEX = 0, PROGRAM_INDEX = 1,
                    AUX_INDEX = 2, OPTIONS_INDEX = 3, QUICK_START = 4};
    enum OPTIONS_PAGE_INDEX {SYS_INFO = 0, SYS_PARAM = 1, CONTROL_PARAM = 2};

    enum Pid_Tab_Index {Temperature_Index = 0, Humidity_Index = 1};

};

#endif // MAINWINDOW_H
