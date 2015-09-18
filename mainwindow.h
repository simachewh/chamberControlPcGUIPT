#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QFileSystemModel>

#include "controlcommands.h"
#include "communication.h"
#include "chamber.h"
#include "addprogram.h"
#include "databackup.h"
#include "stepsmodel.h"
#include "loadprogram.h"
#include "renamedialog.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

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

    void on_startStopButton_clicked();

public slots:
    void populateProgramsList();

    void on_humidPowerChange(int value);
    void on_tempPowerChange(int value);

    void on_testProgramNameChanged(QString value);

    void on_testProgramParamChanged(int value, Program::PGM_PARAM param);

private:
    Ui::MainWindow *ui;
    Communication *communication;

    enum VIEW_INDEX {MONITOR_INDEX = 0, PROGRAM_INDEX = 1, AUX_INDEX = 2, HELP_INDEX = 3};

};

#endif // MAINWINDOW_H
