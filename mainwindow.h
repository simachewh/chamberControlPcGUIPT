#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QFileSystemModel>

#include "controlpc.h"
#include "communication.h"
#include "chamber.h"
#include "process.h"
#include "addprogram.h"


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
    void on_monitorButton_clicked();

    void on_auxButton_clicked();

    void on_helpButton_clicked();

    void on_programButton_clicked();

    void initStyle();

    void on_newProgramButton_clicked();

public slots:
    void populateProgramsList();

private:
    Ui::MainWindow *ui;
    Communication *communication;
    //AddProgram *ap;

    enum {MONITOR_INDEX = 0, PROGRAM_INDEX = 1, AUX_INDEX = 2, HELP_INDEX = 3};

};

#endif // MAINWINDOW_H
