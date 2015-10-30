#include "optionswidget.h"
#include "ui_optionswidget.h"

OptionsWidget::OptionsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OptionsWidget)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(2);
}

OptionsWidget::~OptionsWidget()
{
    delete ui;
}
