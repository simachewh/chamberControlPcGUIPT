#ifndef STEPSMODEL_H
#define STEPSMODEL_H

#include <QObject>
#include <QMap>
#include <QAbstractTableModel>

#include "step.h"
#include "program.h"

/**
 * @brief The StepsModel class A class to create a table model of
 * steps in a Program. An instance of this model can be given to
 * a table view to show each step within a program.
 */
class StepsModel : public QAbstractTableModel
{
private:
    Program *pgmToShow;

public:
    /**
     * @brief StepsModel Constructor to create instance of this class.
     * Initializes the private member pgmToShow with a new empty Program
     * inistance.
     */
    StepsModel();

    /**
     * @brief setProgramToShow Sets the private member pgmToShow to the
     * given Program pointer pgm.
     * @param pgm
     */
    void setProgramToShow(Program *pgm);

    /**
     * @brief getProgramToShow gets the pointer to where the proivate member pgmToShow
     * points to.
     * @return
     */
    Program * getProgramToShow() const;

    int rowCount(const QModelIndex &parent) const;

    int columnCount(const QModelIndex &parent) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    QVariant data(const QModelIndex &index, int role) const;
};

#endif // STEPSMODEL_H
