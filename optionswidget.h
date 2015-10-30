#ifndef OPTIONSWIDGET_H
#define OPTIONSWIDGET_H

#include <QWidget>

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

public slots:
    void on_sysButtonClicked();
};

#endif // OPTIONSWIDGET_H
