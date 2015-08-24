#include "stepsmodel.h"

StepsModel::StepsModel()
{
    pgmToShow = new Program();
}

//! ********************** PUBLIC FUNCTIONS ******************** !//

Program * StepsModel::getProgramToShow() const
{
    return pgmToShow;
}

void StepsModel::setProgramToShow(Program *pgm)
{
    pgmToShow = pgm;
}

//! ********************** END OF PUBLIC FUNCTIONS ************* !//

//! ********************** REIMPLEMENTED FUNCTIONS ************* !//

int StepsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return pgmToShow->getSteps().size();
}

int StepsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 10;
}

QVariant StepsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Vertical && role == Qt::DisplayRole)
    {
        return section + 1;
    }

    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch (section) {
        case 0:
            return "STP. NO.";
            break;
        case 1:
            return "Temp.";
            break;
        case 2:
            return "Humidity";
            break;
        case 3:
            return "Hrs.";
            break;
        case 4:
            return "Mins.";
            break;
        case 5:
            return "WT";
            break;
        case 6:
            return "hr";
            break;
        case 7:
            return "1";
            break;
        case 8:
            return "2";
            break;
        case 9:
            return "3";
            break;
        default:
            return "View ERR";
            break;
        }
    }
    return QVariant();
}

QVariant StepsModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
    {
        return QVariant();
    }
    if(role != Qt::DisplayRole)
    {
        return QVariant();
    }
    if(index.row() < 0 || index.row() > pgmToShow->getSteps().size())
    {
        return QVariant();
    }
    Step *s = pgmToShow->getSteps().value(index.row() +1);
    switch (index.column()) {
    case 0:
        return s->getStepNumber();
        break;
    case 1:
        return s->getTemperature();
        break;
    case 2:
        return s->getHumidity();
        break;
    case 3:
        return s->getHours();
        break;
    case 4:
        return s->getMinutes();
        break;
    case 5:
        return s->getWaiting();
        break;
    case 6:
        return s->getHR();
        break;
    case 7:
        return s->getOne();
        break;
    case 8:
        return s->getTwo();
        break;
    case 9:
        return s->getThree();
        break;
    default:
        return "View ERR";
        break;
    }
    return QVariant();
}

//QModelIndex StepsModel::index(int row, int column, const QModelIndex &parent) const
//{
//    return QModelIndex();
//}

//QModelIndex StepsModel::parent(const QModelIndex &child) const
//{
//    return QModelIndex();
//}

//! ********************** END OF REIMPLEMENTED FUNCTIONS ************* !//

