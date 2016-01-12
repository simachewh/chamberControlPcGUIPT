#include "pidlistmodel.h"

PidListModel::PidListModel()
{
//    pidList = new QList<PID>;
}

int PidListModel::rowCount(const QModelIndex &parent) const
{
    return pidList.size();
}

QVariant PidListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {return QVariant();}

    if (index.row() >= pidList.size() || index.row() < 0)
    {return QVariant();}

    if (role == Qt::DisplayRole) {
        return pidList.at(index.row()).toString();
    }
    return QVariant();
}

QList<PID> PidListModel::getPidList()
{
    return pidList;
}

void PidListModel::setPidList(QList<PID> value)
{
    pidList = value;
}
