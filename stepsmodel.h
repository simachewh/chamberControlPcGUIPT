#ifndef STEPSMODEL_H
#define STEPSMODEL_H

#include <QObject>
#include <QMap>
#include <QAbstractTableModel>

#include "step.h"
#include "program.h"
#include "addstep.h"
#include "databackup.h"

/**
 * @brief The StepsModel class A class to create a table model of
 * steps in a Program. An instance of this model can be given to
 * a table view to show each step within a program.
 */
class StepsModel : public QAbstractTableModel
{
private:
    Q_OBJECT
    Program *pgmToShow;
    Step *openStep;


public:
    AddStep *ads;
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

    bool insertRows(int row, int count, const QModelIndex &parent);

    bool removeRow(int row, const QModelIndex &parent);

public slots:
    bool on_addStepFormSubmitted(QString temp, QString humid, QString hrs,
                                 QString mins, QString wait, QString hr,
                                 QString one, QString two, QString three);

};

#endif // STEPSMODEL_H
