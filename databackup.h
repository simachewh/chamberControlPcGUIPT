#ifndef DATABACKUP_H
#define DATABACKUP_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QFileInfo>

#include "program.h"
#include "step.h"

class DataBackup : public QObject
{
    Q_OBJECT
public:
    static const QString PROGRAMS_DIR_PATH;

    void checkDir(QString name);

    explicit DataBackup(QObject *parent = 0);

    bool saveProgram(Program *p);

    Program loadProgram(QString programName);

    QFileInfoList listPrograms();

private:
    static const QString PROGRAMS_DIR_NAME;

signals:
    void dirNotFound(QString dirName);

public slots:
    bool createDir(QString dirName);
};

#endif // DATABACKUP_H
