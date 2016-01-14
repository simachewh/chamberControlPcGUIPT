#ifndef PIDLISTMODEL_H
#define PIDLISTMODEL_H

#include <QApplication>
#include <QBrush>
#include <QPalette>
#include <QObject>
#include <QAbstractListModel>
#include <QDebug>

#include "pid.h"

class PidListModel : public QAbstractListModel
{
public:
    PidListModel();
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    bool removeRow(int row, const QModelIndex &parent) Q_DECL_OVERRIDE;
    void chooseDefault(int index);

    QList<PID> getPidList();
    void setPidList(QList<PID> value);

private:
    QList<PID> pidList;
};

#endif // PIDLISTMODEL_H
