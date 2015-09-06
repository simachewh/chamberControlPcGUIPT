#include "controlpc.h"
#include "communication.h"
#include <QDebug>

bool ControlPC::isIdle = false;

ControlPC::ControlPC(QObject *parent) : QObject(parent)
{
    stx = 0x02;
    etx = 0x03;
    zero = 0x30;
    capO = 0x4F;
    qMark = 0x3F;
    lineBreak = 0x0D;
    //commandBlock1 = commandBlock2 = commandBlock3 = commandBlock4 = 0x30;
    iy.i = 0X49;
    iy.y = 0x79;
    aq.a = 0x41;
    aq.q = 0x71;
    br.b = 0x42;
    br.r = 0x72;

    commandBlock1 = new QBitArray(8);
    commandBlock2 = new QBitArray(8);
    humidityBar = new QByteArray();
    temperatureBar = new QByteArray();
    testProgram = new Program(parent);

    connect(this, SIGNAL(chPartChanged(bool,ControlPC::CH_PART)),
            this, SLOT(on_chPartChanged(bool,ControlPC::CH_PART)));
    connect(this, SIGNAL(temperaturePowerChanged(int)),
            this,  SLOT(on_temperaturePowerChanged(int)));
    connect(this, SIGNAL(humidityPowerChanged(int)),
            this, SLOT(on_humidityPowerChanged(int)));
}

ControlPC::~ControlPC(){

}

QByteArray ControlPC::iyCommand(){
    QByteArray * commandIy = new QByteArray();
    commandIy->append(stx);
    commandIy->append(zero);
    commandIy->append(iy.i);
    commandIy->append(etx);
    commandIy->append(iy.y);
    commandIy->append(lineBreak);

    return *commandIy;
}

QByteArray ControlPC::aqCommand(){
    QByteArray * commandAq = new QByteArray();
    commandAq->append(stx);
    commandAq->append(zero);
    commandAq->append(aq.a);
    commandAq->append(etx);
    commandAq->append(aq.q);
    commandAq->append(lineBreak);

    return *commandAq;
}

QByteArray ControlPC::brCommand(){
    QByteArray * commandBr = new QByteArray();
    commandBr->append(stx);
    commandBr->append(zero);
    commandBr->append(br.b);
    commandBr->append(etx);
    commandBr->append(br.r);
    commandBr->append(lineBreak);

    return *commandBr;
}

QByteArray ControlPC::idleCommand(){
    QByteArray * commandIdeal = new QByteArray();
    QString zeros(20, zero);
    commandIdeal->append(stx);
    commandIdeal->append(zero);
    commandIdeal->append(capO);
    commandIdeal->append(zeros);
    commandIdeal->append(qMark);
    commandIdeal->append(lineBreak);

    return *commandIdeal;
}
//!maybe good for performance, consider implimenting!//
QByteArray ControlPC::fullCommand(){
    completeCommand->append(stx);
    completeCommand->append(zero);
    completeCommand->append(*commandBody);
    completeCommand->append(lineBreak);

    return *completeCommand;
}

QByteArray ControlPC::assembleFullCommand(PC_COMMAND ctype)
{
    QByteArray assembledCommand;
    assembledCommand.append(stx);
    assembledCommand.append(zero);
    switch (ctype) {
    case BR:
        assembledCommand.append(br.b);
        assembledCommand.append(etx);
        assembledCommand.append(br.r);
        break;
    case AQ:
        assembledCommand.append(aq.a);
        assembledCommand.append(etx);
        assembledCommand.append(aq.q);
        break;
    case IY:
        assembledCommand.append(iy.i);
        assembledCommand.append(etx);
        assembledCommand.append(iy.y);
        break;
    case O:
    {
        assembledCommand.append(capO);
        QString sixBytesOfZero(6, zero);
        assembledCommand.append(sixBytesOfZero);
        assembledCommand.append(*block1);
        assembledCommand.append(*block2);
        assembledCommand.append(getTempratureBar());
        assembledCommand.append(getHumidityBar());
        assembledCommand.append(etx);
        assembledCommand.append('?'); //! this here is still not known how it is made and appended
        break;
    }
    default:
        break;
    }
    assembledCommand.append(lineBreak);
    return assembledCommand;
}

QByteArray ControlPC::convertToBytes(QBitArray bits){
    QByteArray bytes;
       bytes.resize(bits.count()/8+1);
       bytes.fill(0);
       // Convert from QBitArray to QByteArray
       for(int b=0; b<bits.count(); ++b)
       {
           qDebug()<< b << " : " << bits[b] << " : " << bits.count();
           bytes[b/8] = ( bytes.at(b/8) | ((bits[b]?1:0)<<(b%8)));
       }
       return bytes;
}

QByteArray ControlPC::convertToBytes(int value){
    QByteArray bytes;
    QDataStream stream(&bytes, QIODevice::ReadWrite);
    stream << 1 << value;
    return bytes.mid(7);
}

//! ******* Getters and Setters ********** !//

bool ControlPC::getIsIdle(){
    return ControlPC::isIdle;
}

bool ControlPC::getH1(){
    return h1;
}

bool ControlPC::getH2(){
    return h2;
}

bool ControlPC::getT1(){
    return t1;
}

bool ControlPC::getT2(){
    return t2;
}

bool ControlPC::getP1(){
    return p1;
}

bool ControlPC::getP2(){
    return p2;
}

bool ControlPC::getP3(){
    return p3;
}

bool ControlPC::getLNU(){
    return lnu;
}

bool ControlPC::getC1(){
    return c1;
}

bool ControlPC::getC2V2(){
    return c2v2;
}

bool ControlPC::getV1(){
    return v1;
}

bool ControlPC::getV3(){
    return v3;
}

bool ControlPC::getV4(){
    return v4;
}

bool ControlPC::getFan(){
    return fan;
}

void ControlPC::setH1(bool value){
    if(h1 != value){
        h1 = value;
        emit chPartChanged(value, H1);
    }
}

void ControlPC::setH2(bool value){
    if(h2 != value){
        h2 = value;
        emit chPartChanged(value, H2);
    }
}

void ControlPC::setT1(bool value){
    if(t1 != value){
        t1 = value;
        emit chPartChanged(value, T1);
    }
}

void ControlPC::setT2(bool value){
    if(t2 != value){
        t2 = value;
        emit chPartChanged(value, T2);
    }
}

void ControlPC::setP1(bool value){
    if(p1 != value){
        p1 = value;
        emit chPartChanged(value, P1);
    }
}

void ControlPC::setP2(bool value){
    if(p2 != value){
        p2 = value;
        emit chPartChanged(value, P2);
    }
}

void ControlPC::setP3(bool value){
    if(p3 != value){
        p3 = value;
        emit chPartChanged(value, P3);
    }
}

void ControlPC::setLNU(bool value){
    if(lnu != value){
        lnu = value;
        emit chPartChanged(value, LNU);
    }
}

void ControlPC::setC1(bool value){
    if(c1 != value){
        c1 = value;
        emit chPartChanged(value, C1);
    }
}

void ControlPC::setC2V2(bool value){
    if(c2v2 != value){
        c2v2 = value;
        emit chPartChanged(value, C2V2);
    }
}

void ControlPC::setV1(bool value){
    if(v1 != value){
        v1 = value;
        emit chPartChanged(value, V1);
    }
}

void ControlPC::setV3(bool value){
    if(v3 != value){
        v3 = value;
        emit chPartChanged(value, V3);
    }
}

void ControlPC::setV4(bool value){
    if(v4 != value){
        v4 = value;
        emit chPartChanged(value, V4);
    }
}

void ControlPC::setFan(bool value){
    if(fan != value){
        fan = value;
        emit chPartChanged(value, FAN);
    }
}

QByteArray ControlPC::getTempratureBar()
{
    return *temperatureBar;
}

void ControlPC::setTemperatureBar(QByteArray value)
{
    *temperatureBar = value;
}

QByteArray ControlPC::getHumidityBar()
{
    return *humidityBar;
}

void ControlPC::setHumidityBar(QByteArray value)
{
   * humidityBar = value;
}

void ControlPC::setTemperaturePower(int value){
    if(temperaturePower != value){
        temperaturePower = value;
        emit temperaturePowerChanged(value);
    }
}

int ControlPC::getTempraturePower(){
    return temperaturePower;
}

void ControlPC::setHumidityPower(int value){
    if(humidityPower != value){
        humidityPower = value;
        emit humidityPowerChanged(value);
    }
}

int ControlPC::getHumidityPower(){
    return humidityPower;
}

//! ******* SLOTS ********** !//

void ControlPC::setIdle(bool idelState){
    if(!(getIsIdle() == idelState)){
        isIdle = idelState;
        emit idleStateChanged();
    }
}

void ControlPC::on_chPartChanged(bool value, ControlPC::CH_PART part){
    switch (part) {
    case H1:
        (*commandBlock1)[6] = value;
        break;
    case H2:
        (*commandBlock1)[7] = value;
        break;
    case T1:
        (*commandBlock1)[4] = value;
        break;
    case T2:
        (*commandBlock1)[5] = value;
        break;
    case P1:
        (*commandBlock1)[1] = value;
        break;
    case P2:
        (*commandBlock1)[2] = value;
        break;
    case P3:
        (*commandBlock1)[3] = value;
        break;
    case LNU:
        (*commandBlock1)[0] = value;
        break;
    case C1:
        (*commandBlock2)[5] = value;
        break;
    case C2V2:
        (*commandBlock2)[2] = value;
        break;
    case V1:
        (*commandBlock2)[1] = value;
        break;
    case V3:
        (*commandBlock2)[3] = value;
        break;
    case V4:
        (*commandBlock2)[4] = value;
        break;
    case FAN:
        (*commandBlock2)[0] = value;
        break;
    default:
        break;
    }
}

void ControlPC::on_temperaturePowerChanged(int value){
    setTemperatureBar(convertToBytes(value));
}

void ControlPC::on_humidityPowerChanged(int value){
    setHumidityBar(convertToBytes(value));
}

