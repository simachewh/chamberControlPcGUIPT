#ifndef PROCESSTEST_H
#define PROCESSTEST_H

#include <QObject>
#include "communication.h"

class ProcessTest : public Communication
{
    Q_OBJECT
public:
    ProcessTest();
    ~ProcessTest();
public slots:
    void idleCommunication();
};

#endif // PROCESSTEST_H
