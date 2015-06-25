#include "communication.h"
#include "controlpc.h"


Communication::Communication(QObject *parent) : QObject(parent)
{
    serial = new QSerialPort(this);
    dataReceived = new QByteArray();

    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
}

Communication::~Communication(){
    delete serial;
    delete dataReceived;
}

// Getters and setters

QSerialPort * Communication::SerialPort(){
    return this->serial;
}


QByteArray Communication::DataReceived(){
    return *this->dataReceived;
}

void Communication::setDataReceived(QByteArray bytes){
    *this->dataReceived = bytes;
}


void Communication::openPort(){
    serial->setPortName("ttyUSB1"); //the name of the serial port should not be hard coded
    if(!serial->isOpen()){
        serial->open(QIODevice::ReadWrite);
    }
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setParity(QSerialPort::NoParity);

}

void Communication::sendData(const QByteArray data){
    if(!serial->isOpen()){
        openPort();
    }

    int sentSize= 0;
    sentSize = serial->write(data);
    qDebug() << "Sent: " << QString(data) << " size" << sentSize;
}

/**
 * @brief Communication::readData functopn.
 * Reads data available in the serial device. It calles the readAll() method from
 * QSerialPort class.
 * @return the recieved data as QByteArray.
 */
QByteArray Communication::readData(){

    //QByteArray recievedData;
    *dataReceived->append(serial->readAll());
    QByteArray *end = new QByteArray(1, 0x0D);

    if(dataReceived->endsWith(*end)){
        emit newData(*dataReceived);
        qDebug() << "before Clear" << *dataReceived;
        dataReceived->clear();
    }
    return *dataReceived;
}



