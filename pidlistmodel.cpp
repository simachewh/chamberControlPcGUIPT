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

    if(role == Qt::BackgroundColorRole){
        if(index.row() % 2 == 0 && !pidList.at(index.row()).getChoosen()){
            return qApp->palette().base();
        }else if(index.row() % 2 != 0 && !pidList.at(index.row()).getChoosen()){
            return qApp->palette().alternateBase();
        }
        if(pidList.at(index.row()).getChoosen()){
            return QColor(135, 206, 250);
        }
    }

    return QVariant();
}

bool PidListModel::removeRow(int row, const QModelIndex &parent)
{
    int size = pidList.size();
    beginRemoveRows(parent, row, row);
    pidList.removeAt(row);
    endRemoveRows();
    if (size > pidList.size()){
        return true;
    }else{
        return false;
    }
}

void PidListModel::chooseDefault(int index)
{
    //CONSL:
    qDebug() << "Index " << index << "val : " <<pidList.at(index).toString();;
    for(int i = 0; i < pidList.size(); i++){
        pidList[i].setChoosen(false);
        qDebug() << "Setting to false" << pidList.at(i).getChoosen();
    }
    pidList[index].setChoosen(true);
    qDebug() << "Setting to true" << pidList.at(index).getChoosen();
}

QList<PID> PidListModel::getPidList()
{
    return pidList;
}

void PidListModel::setPidList(QList<PID> value)
{
    pidList = value;
}
