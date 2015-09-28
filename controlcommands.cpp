#include "controlcommands.h"
#include "communication.h"
#include <QDebug>

bool ControlCommands::isIdle = false;

ControlCommands::ControlCommands(QObject *parent) : QObject(parent)
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
    block1 = new QByteArray();
    block2 = new QByteArray();
    humidityBar = new QByteArray();
    temperatureBar = new QByteArray();

//    testProgram = new Program(parent);
//    climateChamber = new Chamber(parent);

    connect(this, SIGNAL(chPartChanged(bool,ControlCommands::CH_PART)),
            this, SLOT(on_chPartChanged(bool,ControlCommands::CH_PART)));
    connect(this, SIGNAL(temperaturePowerChanged(int)),
            this,  SLOT(on_temperaturePowerChanged(int)));
    connect(this, SIGNAL(humidityPowerChanged(int)),
            this, SLOT(on_humidityPowerChanged(int)));
}

ControlCommands::~ControlCommands(){

}

QByteArray ControlCommands::iyCommand(){
    QByteArray * commandIy = new QByteArray();
    commandIy->append(stx);
    commandIy->append(zero);
    commandIy->append(iy.i);
    commandIy->append(etx);
    commandIy->append(iy.y);
    commandIy->append(lineBreak);

    return *commandIy;
}

QByteArray ControlCommands::aqCommand(){
    QByteArray * commandAq = new QByteArray();
    commandAq->append(stx);
    commandAq->append(zero);
    commandAq->append(aq.a);
    commandAq->append(etx);
    commandAq->append(aq.q);
    commandAq->append(lineBreak);

    return *commandAq;
}

QByteArray ControlCommands::brCommand(){
    QByteArray * commandBr = new QByteArray();
    commandBr->append(stx);
    commandBr->append(zero);
    commandBr->append(br.b);
    commandBr->append(etx);
    commandBr->append(br.r);
    commandBr->append(lineBreak);

    return *commandBr;
}

QByteArray ControlCommands::idleCommand(){
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

QByteArray ControlCommands::fullCommand(PC_COMMAND ctype)
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
        *block1 = convertToBytes(*commandBlock1);
        *block2 = convertToBytes(*commandBlock2);
        assembledCommand.append(capO);
        QString sixBytesOfZero(6, zero);
        assembledCommand.append(sixBytesOfZero);
        assembledCommand.append(*block1);
        assembledCommand.append(*block2);
        assembledCommand.append(getTempratureBar());
        assembledCommand.append(getHumidityBar());
        assembledCommand.append(etx);
        //! checksum calculated from the assembled command
        assembledCommand.append(calculateCksum(assembledCommand));
        //assembledCommand.append('?');
        break;
    }
    default:
        break;
    }
    assembledCommand.append(lineBreak);
    return assembledCommand;
}

QByteArray ControlCommands::calculateCksum(QByteArray value)
{
    char sum = 0;
    for(int i = 1; i < value.size() - 1; i++){
        sum += value.at(i);
    }
    return QByteArray(1, sum);
}

QByteArray ControlCommands::getCalculatedCksum()
{
    QByteArray ba;
    QString sixBytesOfZero(6, zero);
    ba.append(zero);
    ba.append(sixBytesOfZero);
    ba.append(convertToBytes(*commandBlock1));
    ba.append(convertToBytes(*commandBlock2));
    ba.append(getTempratureBar());
    ba.append(getHumidityBar());
    char sum = 0;
    for (int i = 0; i < ba.size(); i++) {
        sum += ba.at(i);
    }
    return QByteArray(1, sum);
}

QByteArray ControlCommands::convertToBytes(QBitArray bits){
    QByteArray bytes;
//    bytes.resize(bits.count()/8+1);
    bytes.resize(bits.count()/8);
       bytes.fill(0x30);
       // Convert from QBitArray to QByteArray
       for(int b=0; b<bits.count(); ++b)
       {
//           qDebug()<< "ControlCommands::convertToBytes: " << b << " : " << bits[b] << " : " << bits.count();
           bytes[b/8] = ( bytes.at(b/8) | ((bits[b]?1:0)<<(b%8)));
       }
       return bytes;
}

QByteArray ControlCommands::convertToBytes(int value){
    QByteArray bytes;
    QDataStream stream(&bytes, QIODevice::ReadWrite);
    stream << 1 << value;
    return bytes.mid(7);
}


bool ControlCommands::getIsIdle(){
    return ControlCommands::isIdle;
}

bool ControlCommands::getH1(){
    return h1;
}

bool ControlCommands::getH2(){
    return h2;
}

bool ControlCommands::getT1(){
    return t1;
}

bool ControlCommands::getT2(){
    return t2;
}

bool ControlCommands::getP1(){
    return p1;
}

bool ControlCommands::getP2(){
    return p2;
}

bool ControlCommands::getP3(){
    return p3;
}

bool ControlCommands::getLNU(){
    return lnu;
}

bool ControlCommands::getC1(){
    return c1;
}

bool ControlCommands::getC2V2(){
    return c2v2;
}

bool ControlCommands::getV1(){
    return v1;
}

bool ControlCommands::getV3(){
    return v3;
}

bool ControlCommands::getV4(){
    return v4;
}

bool ControlCommands::getFan(){
    return fan;
}

void ControlCommands::setH1(bool value){
    if(h1 != value){
        h1 = value;
        emit chPartChanged(value, H1);
    }
}

void ControlCommands::setH2(bool value){
    if(h2 != value){
        h2 = value;
        emit chPartChanged(value, H2);
    }
}

void ControlCommands::setT1(bool value){
    if(t1 != value){
        t1 = value;
        emit chPartChanged(value, T1);
    }
}

void ControlCommands::setT2(bool value){
    if(t2 != value){
        t2 = value;
        emit chPartChanged(value, T2);
    }
}

void ControlCommands::setP1(bool value){
    if(p1 != value){
        p1 = value;
        emit chPartChanged(value, P1);
    }
}

void ControlCommands::setP2(bool value){
    if(p2 != value){
        p2 = value;
        emit chPartChanged(value, P2);
    }
}

void ControlCommands::setP3(bool value){
    if(p3 != value){
        p3 = value;
        emit chPartChanged(value, P3);
    }
}

void ControlCommands::setLNU(bool value){
    if(lnu != value){
        lnu = value;
        emit chPartChanged(value, LNU);
    }
}

void ControlCommands::setC1(bool value){
    if(c1 != value){
        c1 = value;
        emit chPartChanged(value, C1);
    }
}

void ControlCommands::setC2V2(bool value){
    if(c2v2 != value){
        c2v2 = value;
        emit chPartChanged(value, C2V2);
    }
}

void ControlCommands::setV1(bool value){
    if(v1 != value){
        v1 = value;
        emit chPartChanged(value, V1);
    }
}

void ControlCommands::setV3(bool value){
    if(v3 != value){
        v3 = value;
        emit chPartChanged(value, V3);
    }
}

void ControlCommands::setV4(bool value){
    if(v4 != value){
        v4 = value;
        emit chPartChanged(value, V4);
    }
}

void ControlCommands::setFan(bool value){
    if(fan != value){
        fan = value;
        emit chPartChanged(value, FAN);
    }
}

QByteArray ControlCommands::getTempratureBar()
{
    return *temperatureBar;
}

void ControlCommands::setTemperatureBar(QByteArray value)
{
    *temperatureBar = value;
}

void ControlCommands::resetAll()
{
    //! sitch of all devices & set power outputs to zero
    bool on = true;
    bool off = false;

    setT1(off);
    setT2(off);
    setH1(off);
    setH2(off);
    setC1(off);
    setC2V2(off);
    setV1(off);
    setV3(off);
    setV4(off);
    setLNU(off);
    setP1(off);
    setP2(off);
    setP3(off);
    setHumidityPower(0);
    setTemperaturePower(0);
    setFan(off);
    setIdle(off);
}

QByteArray ControlCommands::getHumidityBar()
{
    return *humidityBar;
}

void ControlCommands::setHumidityBar(QByteArray value)
{
    * humidityBar = value;
}

QByteArray ControlCommands::getCksum()
{
    *cksum = getCalculatedCksum();
    return *cksum;
}

void ControlCommands::setCksum(QByteArray value)
{
    *cksum = value;
}

void ControlCommands::setTemperaturePower(int value){
    if(temperaturePower != value && value >= 0 && value <= 255){
        temperaturePower = value;
        emit temperaturePowerChanged(value);
    }
}

int ControlCommands::getTempraturePower(){
    return temperaturePower;
}

void ControlCommands::setHumidityPower(int value){
    if(humidityPower != value && value >= 0 && value <= 255){
        humidityPower = value;
        emit humidityPowerChanged(value);
    }
}

int ControlCommands::getHumidityPower(){
    return humidityPower;
}


void ControlCommands::setIdle(bool idelState){
    if(!(getIsIdle() == idelState)){
        isIdle = idelState;
        emit idleStateChanged();
    }
}

void ControlCommands::on_chPartChanged(bool value, ControlCommands::CH_PART part){
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

void ControlCommands::on_temperaturePowerChanged(int value){
    setTemperatureBar(convertToBytes(value));
}

void ControlCommands::on_humidityPowerChanged(int value){
    setHumidityBar(convertToBytes(value));
}

