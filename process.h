#ifndef PROCESS_H
#define PROCESS_H

#include <QObject>

#include "communication.h"
#include "controlpc.h"
#include "chamber.h"

class Process : public QObject
{
    Q_OBJECT
public:
    explicit Process(QObject *parent = 0);

private:
    ControlPC *controlerParams;
    Communication *communication;
    Chamber *climateChamber;

signals:
    void newValues(QByteArray);
    void idelProcess(bool isIdel);
public slots:
    void on_newValues(QByteArray);
};

#endif // PROCESS_H
