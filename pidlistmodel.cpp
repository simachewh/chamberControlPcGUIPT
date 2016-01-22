#include "pidlistmodel.h"

PidListModel::PidListModel()
{
    openPid = new PID();
}

PidListModel::~PidListModel()
{
    delete openPid;
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

bool PidListModel::insertRow(int row, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + 1);
    pidList.insert(pidList.size(), *openPid);
    endInsertRows();
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

void PidListModel::on_pidFormSubmited(double p, double i, double d, int choice)
{
    QString name = "temperature";
    if(choice == 1){
        name = "humidity";
    }

    PID pid;
    pid.setKp(p);
    pid.setKi(i);
    pid.setKd(d);
    qDebug() << "PidListModel::on_pidFormSubmited"
             << pid.getKp();
    openPid = &pid;
    insertRow(pidList.size(), QModelIndex());
    DataBackup db;
    db.on_pidFormSubmited(p, i, d, choice);

}
