#include "optionswidget.h"
#include "ui_optionswidget.h"

OptionsWidget::OptionsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OptionsWidget)
{
    ui->setupUi(this);
    ui->optionsStackedWidget->setCurrentIndex(2);
    connect(ui->sysInfoToolButton, SIGNAL(clicked()),
            this, SLOT(on_sysButtonClicked()));
}

OptionsWidget::~OptionsWidget()
{
    delete ui;
}

void OptionsWidget::on_sysInfoToolButton_clicked()
{
    ui->optionsStackedWidget->setCurrentIndex(SYS_INFO);
}

void OptionsWidget::on_sysParamToolButton_clicked()
{
    ui->optionsStackedWidget->setCurrentIndex(SYS_PARAM);
    QSettings setting;
    ui->pointLineEdit->setText(setting.value("point", 25).toString());
    ui->maxHighLineEdit->setText(setting.value("max", 140).toString());
    ui->maxLowLineEdit->setText(setting.value("min", -40).toString());
}

void OptionsWidget::on_controlParamToolButton_clicked()
{
    ui->optionsStackedWidget->setCurrentIndex(CONTROL_PARAM);
    ui->minusHPButton->setEnabled(false);
    ui->tempPIDListView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tempPIDListView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tempPIDListView->setAlternatingRowColors(true);
    DataBackup db;
    PidListModel *tempPidModel = new PidListModel();
    tempPidModel->setPidList(db.loadPIDList(0));
    ui->tempPIDListView->setModel(tempPidModel);

    ui->humidPIDListView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->humidPIDListView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->humidPIDListView->setAlternatingRowColors(true);
    PidListModel *humidPidModel = new PidListModel();
    humidPidModel->setPidList(db.loadPIDList(1));
    ui->humidPIDListView->setModel(humidPidModel);

//    if(!communication->pidController->isDefaultSet()){
//        QMessageBox::information(this, "Info", "PID params default has not been set yet."
//                                               "Remember to set defaults before runing tests",
//                                 QMessageBox::Ok);
//    }
    populateHumidPID();
    populateTempPID();
}

void OptionsWidget::populateTempPID()
{
    DataBackup db;
    PidListModel *tempPidModel = new PidListModel();
    tempPidModel->setPidList(db.loadPIDList(Temperature_Index));
    ui->tempPIDListView->setModel(tempPidModel);
    ui->tempPIDListView->repaint();
}

void OptionsWidget::populateHumidPID()
{
    DataBackup db;
    PidListModel *humidPidModel = new PidListModel();
    humidPidModel->setPidList(db.loadPIDList(Humidity_Index));
    ui->humidPIDListView->setModel(humidPidModel);
    ui->humidPIDListView->repaint();
}
