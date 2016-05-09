#ifndef STEPSMODEL_H
#define STEPSMODEL_H

#include <QObject>
#include <QMap>
#include <QAbstractTableModel>

#include "step.h"
#include "program.h"
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

    /**
     * @brief pgmToShow The test program that has steps from which the
     *step model is made from.
     */
    Program *pgmToShow;

    /**
     * @brief openStep Reserved space for a Step that can be added to the model.
     */
    Step *openStep;

    /**
     * @brief The HEADER_TYPE enum Enumertaion for the header data.
     */
    enum HEADER_TYPE {NUM = 0, TEMPR = 1, HUM = 2,
                     HRS = 3, MINS = 4, WAIT =5,
                     HR = 6, ONE = 7, TWO = 8, THREE = 9};


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

    /**
     * @brief rowCount Counts the number of rows in the step model.
     * This count is equal to the number of steps in a test program.
     * @param parent
     * @return the number of steps in a test program.
     */
    int rowCount(const QModelIndex &parent) const;

    /**
     * @brief columnCount Counts and returns the number of columns in the step model.
     * @param parent
     * @return the number of column count.
     */
    int columnCount(const QModelIndex &parent) const;

    /**
     * @brief headerData Creates the headers of the table view. Column title
     * and row numbers.
     * @param section
     * @param orientation
     * @param role
     * @return
     */
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    /**
     * @brief data This function fills the table with the data of steps in the
     * test program. Each row represents a step. Each column in a row represents
     * the parameters of a step.
     * @param index
     * @param role
     * @return
     */
    QVariant data(const QModelIndex &index, int role) const;

    /**
     * @brief insertRows Inserts a new step to the list of steps in a program.
     * The newly added step is added to the view, on the table view and added
     * the program file.
     * @param row
     * @param count
     * @param parent
     * @return
     */
    bool insertRows(int row, int count, const QModelIndex &parent);

    /**
     * @brief removeRows Removes the selected step from the test program.
     * @param row the selected step's row
     * @param count
     * @param parent
     * @return
     */
    bool removeRows(int row, int count, const QModelIndex &parent);

public slots:

    /**
     * @brief on_addStepFormSubmitted Initializes the openStep Step object
     * with the given parameters and makes a call to insertRows to add a step
     * to the program.
     * @param temp
     * @param humid
     * @param hrs
     * @param mins
     * @param wait
     * @param hr
     * @param one
     * @param two
     * @param three
     * @return
     */
    bool on_addStepFormSubmitted(QString temp, QString humid, QString hrs,
                                 QString mins, QString wait, QString hr,
                                 QString one, QString two, QString three);

};

#endif // STEPSMODEL_H
