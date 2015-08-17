#ifndef DATABACKUP_H
#define DATABACKUP_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QFileSystemModel>
#include <QTextStream>
#include <QDataStream>

#include "program.h"
#include "step.h"

class DataBackup : public QObject
{
    Q_OBJECT
public:
    enum File_Type {PRGM, TST_DATA, SYS_WARN, SYS_BOOT};
    static const QString PROGRAMS_DIR_PATH;

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
    Program loadProgram(QString programName);

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

private:
    static const QString PROGRAMS_DIR_NAME;
    static const QString PRGM_FILE_EXT;

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
};

#endif // DATABACKUP_H
