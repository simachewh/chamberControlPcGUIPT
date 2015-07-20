#include "processtest.h"

ProcessTest::ProcessTest() : Communication(1)
{
    qDebug() << "ProcessTest() entered";
}

ProcessTest::~ProcessTest(){

}

void ProcessTest::idleCommunication(){
    qDebug() << "startIdelCommunication(): entered";
    int i = 0;
    while(controlParams->getIsIdle()){
               //sendData(controlParams->iyCommand());
               //sendData(controlParams->aqCommand());
               //sendData(controlParams->brCommand());
               //sendData(controlParams->idelCommand());
            qDebug() << "run loop" << i++;
       }
}

