#ifndef PIDLISTMODEL_H
#define PIDLISTMODEL_H

#include <QObject>
#include <QAbstractListModel>

#include "pid.h"

class PidListModel : public QAbstractListModel
{
public:
    PidListModel();
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;


    QList<PID> getPidList();
    void setPidList(QList<PID> value);

private:
    QList<PID> pidList;
};

#endif // PIDLISTMODEL_H
