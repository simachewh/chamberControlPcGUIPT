#ifndef PIDLISTMODEL_H
#define PIDLISTMODEL_H

#include <QApplication>
#include <QBrush>
#include <QPalette>
#include <QObject>
#include <QAbstractListModel>
#include <QDebug>

#include "pid.h"
#include "databackup.h"

class PidListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    PidListModel();
    ~PidListModel();
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    bool removeRow(int row, const QModelIndex &parent) Q_DECL_OVERRIDE;
    bool insertRow(int row, const QModelIndex &parent) Q_DECL_OVERRIDE;
    void chooseDefault(int index);

    QList<PID> getPidList();
    void setPidList(QList<PID> value);

private:
    QList<PID> pidList;
    PID *openPid;

public slots:
    void on_pidFormSubmited(double p, double i, double d, int choice);
};

#endif // PIDLISTMODEL_H
