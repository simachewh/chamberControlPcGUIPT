#include "communication.h"

Communication::Communication(QObject *parent) : QObject(parent)
{
    serial = new QSerialPort(this);
    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
}

Communication::~Communication(){

}

// Getters and setters

QSerialPort * Communication::SerialPort(){
    return this->serial;
}

void Communication::setSerialPort(QSerialPort port){
    //*(Communication::serial) = port;
}

QByteArray Communication::DataReceived(){
    return *this->dataReceived;
}

void Communication::setDataReceived(QByteArray bytes){
    *this->dataReceived = bytes;
}


void Communication::openPort(){
    serial->setPortName("ttyUSB1"); //the name of the serial port should not be hard coded
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setParity(QSerialPort::NoParity);
    if(!serial->isOpen()){
        serial->open(QIODevice::ReadWrite);
    }
}

void Communication::sendData(const QByteArray data){
    if(!serial->isOpen()){
        openPort();
    }

    int sentSixe = serial->write(data);

    qDebug() << "Sent: " << QString(data) << " size" << sentSixe;
}

/**
 * @brief Communication::readData
 * Reads data available in the serial device. It calles the readAll() method from
 * QSerialPort class.
 * @return the recieved data as QByteArray.
 */
QByteArray Communication::readData(){
    QByteArray readData = serial->readAll();
    emit newData(readData);
    return readData;
}

