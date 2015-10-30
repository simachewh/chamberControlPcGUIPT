#include "optionswidget.h"
#include "ui_optionswidget.h"

OptionsWidget::OptionsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OptionsWidget)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(2);
    connect(ui->sysInfoPushButton, SIGNAL(clicked()),
            this, SLOT(on_sysButtonClicked()));
}

OptionsWidget::~OptionsWidget()
{
    delete ui;
}

void OptionsWidget::on_sysButtonClicked()
{
    ui->testlabel->setText("System");
}
