#include "communication.h"
#include "processtest.h"

QSerialPort *Communication::serial;

Communication::Communication(int a)
{
    qDebug() << "entered";
}

Communication::Communication(QObject *parent) : QObject(parent)
{
    serial = new QSerialPort(this);
    dataReceived = new QByteArray();

    controlParams = new ControlPC();
    chamberParams = new Chamber();
    //process = new ProcessTest();
    processThread = new QThread();

    //process->moveToThread(processThread);
    openPort();

    connect(serial, SIGNAL(readyRead()),
            this, SLOT(readData()));

    connect(this, SIGNAL(newDataArived(QByteArray, ControlPC::chCommand)),
            this, SLOT(on_newDataArived(QByteArray, ControlPC::chCommand)));

    connect(controlParams, SIGNAL(idleStateChanged()),
            this, SLOT(on_idelStateChanged()));

    controlParams->setIdle(true);

    //processThread->start();
}

void Communication::prepCommunication(){

}

Communication::~Communication(){
    processThread->quit();
    delete serial;
    delete dataReceived;
    delete controlParams;
    delete chamberParams;
    delete process;
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

        serial->flush();

        qDebug() << "port open: " << serial->isOpen() << endl
                 << "Baude: " << serial->baudRate() << endl
                 << "Data bits: " << serial->dataBits() << endl
                 << "Stop Bits: " << serial->stopBits() << endl
                 << "Parity: " << serial->parity();
    }else{
        open = serial->isOpen();
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
    if(*dataReceived->data() == ControlPC::ACK ){
        emit newDataArived(*dataReceived, ControlPC::ACK);
        dataReceived->clear();
    } else if(dataReceived->endsWith(*end)){
        char nameOfCommand = dataReceived->at(2);
        if(nameOfCommand == ControlPC::A){
            emit newDataArived(*dataReceived, ControlPC::A); //there might be no need to emit datarecieved
        }else if(nameOfCommand == ControlPC::B){
            emit newDataArived(*dataReceived, ControlPC::B);
        }else if(nameOfCommand == ControlPC::I){
            emit newDataArived(*dataReceived, ControlPC::I);
        }else{
            qDebug() << "Unknown data recieved" << *dataReceived;
        }

        qDebug() << "before Clear" << *dataReceived;
        dataReceived->clear();
    }
    return *dataReceived;
}

void Communication::on_idelStateChanged(){
    bool isIdel = controlParams->getIsIdle();
    if(isIdel){
        qDebug() << "state changed: " << isIdel;
        startIdelCommunication();
    }else{
        qDebug() << "state changed: " << isIdel;
    }
}

void Communication::on_newDataArived(QByteArray newDataArived, ControlPC::chCommand command){
    QStringList list;
    double temp, humid;

    if(command == ControlPC::ACK){
        sendData(controlParams->brCommand());
    }else if(command == ControlPC::A){
        QString str(newDataArived);
        list = str.split(" ");

        temp = list[0].mid(4,9).toDouble();
        humid = list[2].left(4).toDouble();

        chamberParams->setDryTemprature(temp);
        chamberParams->setHumidity(humid);

        sendData(controlParams->iyCommand());
    }else if (command == ControlPC::B){
        sendData(controlParams->aqCommand());
    }else if (command == ControlPC::I){
        sendData(controlParams->idleCommand());
    }else{

    }
}

//! end of SLOTS implementaion !//


void Communication::startIdelCommunication(){
    qDebug() << "startIdelCommunication(): entered";
    sendData(controlParams->idleCommand());
}




