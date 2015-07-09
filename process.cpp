#include "process.h"

Process::Process(QObject *parent) : QObject(parent)
{
    controlerParams = new ControlPC();
    communication = new Communication();
    climateChamber = new Chamber();

    //! no programs are runing at start up !//
    //ControlPC::isIdel = true;
    //!connect the newData(QByteArray) signal of the the communiction
    //! object to on_newValues(QByteArray) slot of this class to forward
    //! new data arival to the gui.
    connect(this->communication, SIGNAL(newData(QByteArray)),
            this, SLOT(on_newValues(QByteArray)));
    communication->openPort();
}

void Process::on_newValues(QByteArray data){
    emit newValues(data);
}

