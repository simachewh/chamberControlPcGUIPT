#include "communication.h"

QSerialPort *Communication::serial;


Communication::Communication(QObject *parent) : QObject(parent)
{
    serial = new QSerialPort(this);
    dataReceived = new QByteArray();

    //controlParams = new ControlPC();
    pidController = new Controller(parent);

    openPort();

    connect(serial, SIGNAL(readyRead()),
            this, SLOT(readData()));

    connect(this, SIGNAL(newDataArived(QByteArray, ControlCommands::CH_COMMAND)),
            this, SLOT(on_newDataArived(QByteArray, ControlCommands::CH_COMMAND)));

    connect(this, SIGNAL(replyReady(ControlCommands::CH_COMMAND)),
            this, SLOT(reply(ControlCommands::CH_COMMAND)));

    connect(pidController->controlCommands, SIGNAL(idleStateChanged()),
            this, SLOT(on_idelStateChanged()));

    connect(this, SIGNAL(requestControl()),
            pidController, SLOT(on_controlRequested()));

    connect(pidController, SIGNAL(controlready(ControlCommands::CH_COMMAND)),
            this, SLOT(reply(ControlCommands::CH_COMMAND)));

    pidController->controlCommands->setIdle(true);
}

Communication::~Communication(){
    delete serial;
    delete dataReceived;
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


QByteArray Communication::readData(){
    *dataReceived->append(serial->readAll());
    QByteArray *end = new QByteArray(1, 0x0D);
    if(*dataReceived->data() == ControlCommands::ACK ){
        emit newDataArived(*dataReceived, ControlCommands::ACK);
        dataReceived->clear();
    } else if(dataReceived->endsWith(*end)){
        char nameOfCommand = dataReceived->at(2);
        if(nameOfCommand == ControlCommands::A){
            emit newDataArived(*dataReceived, ControlCommands::A); //there might be no need to emit datarecieved
        }else if(nameOfCommand == ControlCommands::B){
            emit newDataArived(*dataReceived, ControlCommands::B);
        }else if(nameOfCommand == ControlCommands::I){
            emit newDataArived(*dataReceived, ControlCommands::I);
        }else{
            qDebug() << "Unknown data recieved" << *dataReceived;
            emit unusualDataArived(*dataReceived);
        }

        qDebug() << "before Clear" << *dataReceived;
        dataReceived->clear();
    }
    return *dataReceived;
}

void Communication::on_idelStateChanged(){
    bool isIdel = pidController->controlCommands->getIsIdle();
    if(isIdel){
        qDebug() << "state changed: " << isIdel;
        disconnect(pidController->chamberParams, SIGNAL(dryTemperatureChanged(int)),
                   pidController, SLOT(on_realvaluesChanged(int)));
        disconnect(pidController->chamberParams, SIGNAL(humidityChanged(int)),
                   pidController, SLOT(on_realvaluesChanged(int)));
        startIdelCommunication();
    }else{
        qDebug() << "state changed: " << isIdel;
        connect(pidController->chamberParams, SIGNAL(dryTemperatureChanged(int)),
                   pidController, SLOT(on_realvaluesChanged(int)));
        connect(pidController->chamberParams, SIGNAL(humidityChanged(int)),
                   pidController, SLOT(on_realvaluesChanged(int)));
    }
}

void Communication::on_newDataArived(QByteArray newDataArived, ControlCommands::CH_COMMAND chCommand){

if(chCommand == ControlCommands::A){
        QStringList list;
        double temp, humid;
        QString str(newDataArived);
        list = str.split(" ");

        temp = list[0].mid(4,9).toDouble();
        humid = list[2].left(4).toDouble();

        pidController->chamberParams->setDryTemprature(temp);
        pidController->chamberParams->setHumidity(humid);
        if(pidController->controlCommands->getIsIdle()){
            emit replyReady(chCommand);
        }else{
            emit requestControl();
        }
    }else {
        emit replyReady(chCommand);
    }
}

void Communication::on_chamberConnectionChanged(bool value)
{

}

void Communication::on_controlReady()
{
    //reply()
}

void Communication::reply(ControlCommands::CH_COMMAND chCommand)
{
    if(chCommand == ControlCommands::ACK)
    {
        sendData(pidController->controlCommands->
                 fullCommand(ControlCommands::BR));
    }
    else if(chCommand == ControlCommands::A)
    {
        sendData(pidController->controlCommands->
                 fullCommand(ControlCommands::IY));
    }
    else if(chCommand == ControlCommands::B)
    {
        sendData(pidController->controlCommands->
                 fullCommand(ControlCommands::AQ));
    }
    else if(chCommand == ControlCommands::I)
    {
        sendData(pidController->controlCommands->
                 fullCommand(ControlCommands::O));
    }
    else
    {
        //!if recived command doesn't make sense reply
        //! this. This is helps the communication to
        //! keep going but doesn't handel the issue
        //! better and wont make sure communication will
        //! continue normaly after. Better implementaion
        //! needed.
        sendData(pidController->controlCommands->
                 fullCommand(ControlCommands::BR));
    }
}


void Communication::startIdelCommunication(){
    qDebug() << "Communication::startIdelCommunication(): entered";

    sendData(pidController->controlCommands->idleCommand());
}

bool Communication::isChamberConnected()
{
    return chamberConnected;
}

void Communication::setChamberConnected(bool value)
{
    if(chamberConnected != value){
        chamberConnected = value;
        emit chamberConnectionChanged(value);
    }
}
