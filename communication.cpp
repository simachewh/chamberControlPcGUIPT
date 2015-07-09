#include "communication.h"
#include "controlpc.h"


Communication::Communication(QObject *parent) : QObject(parent)
{
    serial = new QSerialPort(this);
    dataReceived = new QByteArray();

    controlParams = new ControlPC();
    chamberParams = new Chamber();

    openPort();

    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));

    connect(this, SIGNAL(newDataArived(QByteArray)),
            this, SLOT(on_newDataArived(QByteArray)));

    //connect(controlParams, SIGNAL(idelStateChanged()), this, SLOT(on_idelStateChanged()), Qt::QueuedConnection );

    controlParams->setIdel(true);

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


bool Communication::openPort(){
    bool open = false;
    serial->setPortName("ttyUSB1"); //the name of the serial port should not be hard coded
    if(!serial->isOpen()){
        open = serial->open(QIODevice::ReadWrite);
        serial->setBaudRate(QSerialPort::Baud9600);
        serial->setDataBits(QSerialPort::Data8);
        serial->setStopBits(QSerialPort::OneStop);
        serial->setParity(QSerialPort::NoParity);

        emit idelStateChanged(true);
        qDebug() << "port open: " << serial->isOpen() << endl
                 << "Baude: " << serial->baudRate() << endl
                 << "Data bits: " << serial->dataBits() << endl
                 << "Stop Bits: " << serial->stopBits() << endl
                 << "Parity: " << serial->parity();
    }else{
        open = false;
    }
return open;

}

void Communication::sendData(const QByteArray data){
    if(!serial->isOpen()){
        openPort();
    }

    int sentSize= 0;
    sentSize = serial->write(data);
    qDebug() << "Sent: " << QString(data) << " size" << sentSize;
}

//!SLOTS implementation !//
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
        emit newDataArived(*dataReceived);
        qDebug() << "before Clear" << *dataReceived;
        dataReceived->clear();
    }
    return *dataReceived;
}

void Communication::on_idelStateChanged(){
    bool isIdel = controlParams->getIsIdel();
    if(isIdel){
        qDebug() << "state changed: " << isIdel;
        startIdelCommunication();
    }else{
        qDebug() << "state changed: " << isIdel;
    }
}

void Communication::on_newDataArived(QByteArray newDataArived){
    QStringList list;
    double temp, humid;

    if(newDataArived.at(2) == 'A'){
        QString str(newDataArived);
        list = str.split(" ");

        temp = list[0].mid(4,9).toDouble();
        humid = list[2].left(4).toDouble();

        chamberParams->setDryTemprature(temp);
        chamberParams->setHumidity(humid);
    }
}

//! end of SLOTS implementaion !//

void Communication::startIdelCommunication(){
    //QEventLoop loop;
    qDebug() << "startIdelCommunication(): entered";
    while(controlParams->getIsIdel()){
        QCoreApplication::processEvents();
               sendData(controlParams->iyCommand());
               serial->waitForReadyRead(1000);
               sendData(controlParams->aqCommand());
               sendData(controlParams->brCommand());
               sendData(controlParams->idelCommand());

       }
    //loop.exec();
}




