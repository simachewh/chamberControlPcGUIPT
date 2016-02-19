#ifndef DATABACKUP_H
#define DATABACKUP_H

#include <QDate>
#include <QObject>
#include <QFile>
#include <QDir>
#include <QFileSystemModel>
#include <QTextStream>
#include <QDataStream>

#include <QDebug>

#include "program.h"
#include "step.h"
#include "pid.h"

class DataBackup : public QObject
{
    Q_OBJECT
    Q_ENUMS(File_Type)
public:
    enum File_Type {PRGM, TST_DATA, SYS_WARN, SYS_BOOT, PID_DATA};
    static const QString PROGRAMS_DIR_PATH;
    static const QString PID_DIR_PATH;
    static const QString TST_DIR_PATH;

    /**
     * @brief DataBackup constructor.
     * @param parent
     */
    explicit DataBackup(QObject *parent = 0);

    /**
     * @brief checkDir checks if a dirctory by the given name exists in
     * the working directory, and emits appropriate signals based on
     * the result.
     * @param name
     */
    void checkDir(QString name);

    /**
     * @brief saveProgram A function to save program to a file by the name
     * of the program and writes the cycle of the program in the file.
     * @param p
     * @return
     */
    bool saveProgram(Program *p);

    /**
     * @brief loadProgram A function to to load the selected program
     * from the list of programs. []
     * @param programName
     * @return
     */
    void loadTestProgram(QString pgmFileName, Program *prgm);

    /**
     * @brief listPrograms A function that reads the directory storing
     * the program files and returning the list with metadata as
     * QFileSystemModel.
     * @return
     */
    QFileSystemModel* listPrograms();

    /**
     * @brief programExists checks if the file by the name exists. [unused] This
     * can be rewriten to check files other than program files, pass an argument
     * to create a switch case and see.
     * @param name
     * @return true if file by the given name exists, false otherwise.
     */
    bool programExists(QString name);

    /**
     * @brief writeStepToFile saves the given Step to the file that stores
     * the given Program. The file storing the program has same name as
     * the programName attribute of the given program and an extension
     * declared in this class.
     * @param step
     * @param prgm
     * @return
     */
    bool writeStepToFile(Step *step, Program *prgm);

    /**
     * @brief fileLives
     * @param type
     * @param name
     * @return
     */
    QString fileLives(File_Type type, QString name);

    /**
     * @brief renameProgram renames a program file of name oldName to a name given by newName.
     * @param newName
     * @param oldName
     * @return
     */
    bool renameProgram(QString newName, QString oldName);

    /**
     * @brief removeProgram removes the file where the program by the given name is stored from the
     * file system. The name of the program and the file name are the same.
     * @param name
     * @return
     */
    bool removeProgram(QString name);

    void insertPID(QDataStream &stream, PID *pid);

    /**
     * @brief loadPIDList Loads the list of PIDs from the file given by choice.
     * If choice is 0 Temperature PID list is loaded and if choice is 1 Humidity
     * PID list is loaded.
     * @param choice
     * @return if choice is 0 a QList of Temperature PIDs, if choice is 1 a QList
     * of Humidity PIDs.
     */
    QList<PID> loadPIDList(int choice);

    bool appendPlot(QString prgmName, double temp, double humid, int elapsedMinutes);

    void loadPlot(QString name, QVector<double> *temp, QVector<double> *humid,
                  QVector<double> *time);

    void replacePIDList(QList<PID> pidList, int choice);
private:
    static const QString PROGRAMS_DIR_NAME;
    static const QString PID_DIR_NAME;
    static const QString PRGM_FILE_EXT;
    static const QString TXT_FILE_EXT;
    static const QString TST_DIR_NAME;

//! TODO: A possible aproach to saving system wide setting
//! using a struct of setting and perssistin it to a file using the QDataStream
//! NOTE: A more convinent and better approch would be to use the QSetting frame
//! work for a reliable persistance.

signals:
    /**
     * @brief dirNotFound Signal. Use this to broadcast a missing
     * directory by the given name. It is connectd to
     * createDir(QString dirName) slot of this class.
     * @param dirName
     */
    void dirNotFound(QString dirName);

public slots:
    /**
     * @brief createDir A slot function that creates a directory
     * by the given name under the working directory.
     * @param dirName
     * @return
     */
    bool createDir(QString dirName);

    /**
     * @brief on_testStarted Creates the file to write the plot data on.
     * the file name is created by appending the name of the program that is
     * running, the date it is running on and a string "-pl".
     * @param prgmName
     */
    void on_testStarted(QString prgmName);

    void on_testFinished(QString prgmName);

    void on_pidFormSubmited(double p, double i, double d, int choice);
};

#endif // DATABACKUP_H
