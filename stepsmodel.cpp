#include "stepsmodel.h"

StepsModel::StepsModel()
{
    pgmToShow = new Program();
    openStep = new Step();
}

Program * StepsModel::getProgramToShow() const
{
    return pgmToShow;
}

void StepsModel::setProgramToShow(Program *pgm)
{
    pgmToShow = pgm;
}

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
        return section;
    }

    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch (section) {
        case NUM:
            return "Num.";
            break;
        case TEMPR:
            return "Temp.°C";
            break;
        case HUM:
            return "HUM %RH";
            break;
        case HRS:
            return "Hrs.";
            break;
        case MINS:
            return "Mins.";
            break;
        case WAIT:
            return "WT";
            break;
        case HR:
            return "hr";
            break;
        case ONE:
            return "1";
            break;
        case TWO:
            return "2";
            break;
        case THREE:
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
    int row = index.row();
    int stepSize = pgmToShow->getSteps().size();
    bool containsNot = !pgmToShow->getSteps().contains(row);
    if(!index.isValid())
    {
        return QVariant();
    }
    if(role != Qt::DisplayRole)
    {
        return QVariant();
    }

    if(row < 0 || row > pgmToShow->getSteps().size()
            || !pgmToShow->getSteps().contains(row))
    {
        //TODO: check if this is the reason blocking update of GUI
        //on insert row
        return QVariant();
    }
    Step *s = pgmToShow->getSteps().value(index.row());
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
//    return QVariant();
}

bool StepsModel::insertRows(int row, int count, const QModelIndex &parent)
{
    //Q_UNUSED(row)
    //Q_UNUSED(count)
    int sizeBefore = pgmToShow->getNoOfSteps();
    DataBackup db;
    beginInsertRows(parent, row, row);

    pgmToShow->addStep(openStep);

    //db.writeStepToFile(openStep, pgmToShow);
    endInsertRows();
    db.writeStepToFile(openStep, pgmToShow);
    //TODO: GUI doesn't update inserted row, it loads from file when program
    //is selected again. Probably data method returns empty row.
    int sizeAfter = pgmToShow->getSteps().size();
    qDebug() << "insert row ended " << openStep->getTemperature()
             << " sizeBefore" << sizeBefore << " sizeAfter"
             << sizeAfter;
    if(sizeAfter > sizeBefore){
        return true;
    }else{
        return false;
    }
}

bool StepsModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(count)
    qDebug() << "removeRows: before" << pgmToShow->getNoOfSteps();
    int removed = 0;
    beginRemoveRows(parent, row, row);

    removed = pgmToShow->removeStep(row);

    endRemoveRows();


    if(removed == 1){
        qDebug() << "removeRows: true" << pgmToShow->getNoOfSteps();
        return true;
    }else{
        qDebug() << "removeRows: false" << pgmToShow->getNoOfSteps();
        return false;
    }
}


bool StepsModel::on_addStepFormSubmitted(QString temp, QString humid, QString hrs,
                                         QString mins, QString wait, QString hr,
                                         QString one, QString two, QString three)
{
    openStep = new Step();
    openStep->setTemperature(temp.toDouble());
    openStep->setHumidity(humid.toDouble());
    openStep->setHours(hrs.toDouble());
    openStep->setMinutes(mins.toDouble());
    openStep->setWaiting(wait.toInt());
    openStep->setHR(hr.toInt());
    openStep->setOne(one.toInt());
    openStep->setTwo(two.toInt());
    openStep->setThree(three.toInt());
    openStep->setStepNumber(pgmToShow->getSteps().size());
    //pgmToShow->addStep(openStep);
    qDebug() << "open step " << openStep->getTemperature();
    insertRows(pgmToShow->getSteps().size(), 1, QModelIndex());

    return true;
}
