#ifndef DATABACKUP_H
#define DATABACKUP_H

#include <QObject>

#include "program.h"
#include "step.h"

class DataBackup : public QObject
{
    Q_OBJECT
public:
    explicit DataBackup(QObject *parent = 0);

    static bool saveProgram(Program p);

    static Program loadProgram(QString programName);

    static QStringList listPrograms();

signals:

public slots:
};

#endif // DATABACKUP_H
