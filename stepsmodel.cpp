#include "stepsmodel.h"

StepsModel::StepsModel()
{
    pgmToShow = new Program();
    openStep = new Step();
    ads = new AddStep();
    connect(ads, SIGNAL(stepFormSubmitted(QString,QString,QString,
                                          QString,QString,QString,
                                          QString,QString,QString)),
            this, SLOT(on_addStepFormSubmitted(QString,QString,QString,
                                               QString,QString,QString,
                                               QString,QString,QString)));
}

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
            return "NO.";
            break;
        case 1:
            return "Temp.°C";
            break;
        case 2:
            return "HUM %RH";
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

bool StepsModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(row)
    Q_UNUSED(count)
    int sizeBefore = pgmToShow->getSteps().size();
    DataBackup db;
    beginInsertRows(parent, pgmToShow->getSteps().size(),
                    pgmToShow->getSteps().size());
    pgmToShow->addStep(openStep);
    db.writeStepToFile(openStep, pgmToShow);
    endInsertRows();
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

bool StepsModel::removeRow(int row, const QModelIndex &parent)
{
    Q_UNUSED(row) Q_UNUSED(parent)
    return false;
}

bool StepsModel::on_addStepFormSubmitted(QString temp, QString humid, QString hrs,
                                         QString mins, QString wait, QString hr,
                                         QString one, QString two, QString three)
{
    openStep->setTemperature(temp.toDouble());
    openStep->setHumidity(humid.toDouble());
    openStep->setHours(hrs.toDouble());
    openStep->setMinutes(mins.toDouble());
    openStep->setWaiting(wait.toInt());
    openStep->setHR(hr.toInt());
    openStep->setOne(one.toInt());
    openStep->setTwo(two.toInt());
    openStep->setThree(three.toInt());
    openStep->setStepNumber(pgmToShow->getSteps().size() + 1);
    qDebug() << "open step " << openStep->getTemperature();
    insertRows(pgmToShow->getSteps().size(), 1, QModelIndex());

    return true;
}

//! ********************** END OF REIMPLEMENTED FUNCTIONS ************* !//

