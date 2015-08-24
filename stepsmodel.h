#ifndef STEPSMODEL_H
#define STEPSMODEL_H

#include <QObject>
#include <QMap>
#include <QAbstractTableModel>

#include "step.h"
#include "program.h"

class StepsModel : public QAbstractTableModel
{
private:
//    QMap<int, Step*> stepsToModel;
    Program *pgmToShow;
public:
    StepsModel();

    void setProgramToShow(Program *pgm);
    Program * getProgramToShow() const;

//    QMap<int, Step*> getStepsToModel() const;
//    void setStepsToModel(QMap<int, Step*>);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;

//    QModelIndex index(int row, int column, const QModelIndex &parent) const;
//    QModelIndex parent(const QModelIndex &child) const;

};

#endif // STEPSMODEL_H
