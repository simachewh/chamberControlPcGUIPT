#include "controlcommands.h"
#include "communication.h"
#include <QDebug>

bool ControlCommands::isIdle = false;


QBitArray *ControlCommands::getTemperaturePowerRate1() const
{
    return temperaturePowerRate1;
}

void ControlCommands::setTemperaturePowerRate1(QBitArray *value)
{
    (*temperaturePowerRate1)[4] = (*value)[0];
    (*temperaturePowerRate1)[5] = (*value)[1];
    (*temperaturePowerRate1)[6] = (*value)[2];
    (*temperaturePowerRate1)[7] = (*value)[3];
}

QBitArray *ControlCommands::getTemperaturePowerRate2() const
{
    return temperaturePowerRate2;
}

void ControlCommands::setTemperaturePowerRate2(QBitArray *value)
{
    (*temperaturePowerRate2)[4] = (*value)[4];
    (*temperaturePowerRate2)[5] = (*value)[5];
    (*temperaturePowerRate2)[6] = (*value)[6];
    (*temperaturePowerRate2)[7] = (*value)[7];
}

QBitArray *ControlCommands::getHumidityPowerRate2() const
{
    return humidityPowerRate2;
}

void ControlCommands::setHumidityPowerRate2(QBitArray *value)
{
    (*humidityPowerRate2)[4] = (*value)[4];
    (*humidityPowerRate2)[5] = (*value)[5];
    (*humidityPowerRate2)[6] = (*value)[6];
    (*humidityPowerRate2)[7] = (*value)[7];
}

QBitArray *ControlCommands::getHumidityPowerRate1() const
{
    return humidityPowerRate1;
}

void ControlCommands::setHumidityPowerRate1(QBitArray *value)
{
    (*humidityPowerRate1)[4] = (*value)[0];
    (*humidityPowerRate1)[5] = (*value)[1];
    (*humidityPowerRate1)[6] = (*value)[2];
    (*humidityPowerRate1)[7] = (*value)[3];
}
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

    humidityBar = new QByteArray();
    temperatureBar = new QByteArray();
    initBlocks();

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

void ControlCommands::initBlocks(){
    htBlock = new QBitArray(8);
    plBlock = new QBitArray(8);
    cvBlock = new QBitArray(8);
    vFBlock = new QBitArray(8);

    humidityPowerRate1 = new QBitArray(8);
    humidityPowerRate2 = new QBitArray(8);
    temperaturePowerRate1 = new QBitArray(8);
    temperaturePowerRate2 = new QBitArray(8);



    (*temperaturePowerRate1)[0] = (*temperaturePowerRate2)[0] =
            (*humidityPowerRate2)[0] = (*humidityPowerRate1)[0] =
            (*htBlock)[0] = (*plBlock)[0] = (*cvBlock)[0] = (*vFBlock)[0] = 0;
    (*temperaturePowerRate1)[1] = (*temperaturePowerRate2)[1] =
            (*humidityPowerRate2)[1] = (*humidityPowerRate1)[1] =
            (*htBlock)[1] = (*plBlock)[1] = (*cvBlock)[1] = (*vFBlock)[1] = 0;
    (*temperaturePowerRate1)[2] = (*temperaturePowerRate2)[2] =
            (*humidityPowerRate2)[2] = (*humidityPowerRate1)[2] =
            (*htBlock)[2] = (*plBlock)[2] = (*cvBlock)[2] = (*vFBlock)[2] = 1;
    (*temperaturePowerRate1)[3] = (*temperaturePowerRate2)[3] =
            (*humidityPowerRate2)[3] = (*humidityPowerRate1)[3] =
            (*htBlock)[3] = (*plBlock)[3] = (*cvBlock)[3] = (*vFBlock)[3] = 1;
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
//        *htBlock = convertToBytes(*commandBlock1);
//        *plBlock = convertToBytes(*commandBlock2);
        QString twelveZeros(12, zero);
        assembledCommand.append(capO);
        assembledCommand.append(twelveZeros);
        assembledCommand.append(convertToBytes(*htBlock));
        qDebug() << convertToBytes(*htBlock);
        assembledCommand.append(convertToBytes(*plBlock));
        qDebug() << convertToBytes(*plBlock);
        assembledCommand.append(convertToBytes(*cvBlock));
        qDebug() << convertToBytes(*cvBlock);
        assembledCommand.append(convertToBytes(*vFBlock));
        qDebug() << convertToBytes(*vFBlock);
        assembledCommand.append(convertToBytes(*humidityPowerRate1));
        qDebug() << convertToBytes(*humidityPowerRate1);
        assembledCommand.append(convertToBytes(*humidityPowerRate2));
        qDebug() << convertToBytes(*humidityPowerRate2);
        assembledCommand.append(convertToBytes(*temperaturePowerRate1));
        qDebug() << convertToBytes(*temperaturePowerRate1);
        assembledCommand.append(convertToBytes(*temperaturePowerRate2));
        qDebug() << convertToBytes(*temperaturePowerRate2);
        appendChecksum(&assembledCommand);
        assembledCommand.append(etx);
        //! checksum calculated from the assembled command
//        assembledCommand.append(calculateCksum(&assembledCommand));
        qDebug() << "ControlCommands::fullCommand(PC_COMMAND ctype)"
                 << assembledCommand;
        break;
    }
    default:
        break;
    }
    assembledCommand.append(lineBreak);
    return assembledCommand;
}

void ControlCommands::appendChecksum(QByteArray *value)
{
    quint8 cks = 0;
    for(int i = 0; i < value->size(); i++){
        cks += value->at(i);
    }
    QByteArray sum(1, cks);
    qDebug() << "ControlCommands::appendChecksum " << sum;
    value->append(sum);
}


QByteArray ControlCommands::calculateCksum(QByteArray *value)
{
    quint8 cks = 0;
    for(int i = 0; i < value->size(); i++){
        cks += value->at(i);
    }
    return QByteArray(1, cks);
}

QByteArray ControlCommands::getCalculatedCksum()
{
    QByteArray ba;
    QString zeros(12, zero);
    ba.append(zero);
    ba.append(capO);
    ba.append(zeros);
    ba.append(convertToBytes(*htBlock));
    ba.append(convertToBytes(*plBlock));
    ba.append(convertToBytes(*cvBlock));
    ba.append(convertToBytes(*vFBlock));
    ba.append(convertToBytes(*humidityPowerRate1));
    ba.append(convertToBytes(*humidityPowerRate2));
    ba.append(convertToBytes(*temperaturePowerRate1));
    ba.append(convertToBytes(*temperaturePowerRate2));

    return calculateCksum(&ba);
}

QByteArray ControlCommands::convertToBytes(QBitArray bits){
    QByteArray bytes;
//    bytes.resize(bits.count()/8+1);
    bytes.resize(bits.count()/8);
//       bytes.fill(0x30);
       // Convert from QBitArray to QByteArray
       for(int b=0; b<bits.count(); ++b)
       {
          qDebug()<< "ControlCommands::convertToBytes: " << b << " : " << bits[b] << " : " << bits.count();
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

QBitArray ControlCommands::toBitArray(int value){
    QBitArray bits(8);
    bits.fill(false);
    for(int i = 0; i < 8; i++){
        int remain = value % 2;
        if(remain > 0){
            bits[i] = true;
        }else{
            bits[i] = false;
        }
        value /= 2;
    }
    return bits;
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
    qDebug() << "ControlCommands::resetAll : finishing";
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
        qDebug() << "on_chPartChanged: " << "H1 = " << value;
        (*htBlock)[2] = value;
        break;
    case H2:
        (*htBlock)[3] = value;
        break;
    case T1:
        (*htBlock)[0] = value;
        break;
    case T2:
        (*htBlock)[1] = value;
        break;
    case P1:
        (*plBlock)[1] = value;
        break;
    case P2:
        (*plBlock)[2] = value;
        break;
    case P3:
        (*plBlock)[3] = value;
        break;
    case LNU:
        (*plBlock)[0] = value;
        break;
    case C1:
        (*cvBlock)[1] = value;
        break;
    case C2V2:
        (*vFBlock)[2] = value;
        break;
    case V1:
        (*vFBlock)[1] = value;
        break;
    case V3:
        (*vFBlock)[3] = value;
        break;
    case V4:
        (*cvBlock)[0] = value;
        break;
    case FAN:
        (*vFBlock)[0] = value;
        break;
    default:
        break;
    }
}

void ControlCommands::on_temperaturePowerChanged(int value){
    QBitArray b = toBitArray(value);
    setTemperaturePowerRate1(&b);
    setTemperaturePowerRate2(&b);
}

void ControlCommands::on_humidityPowerChanged(int value){
    QBitArray b = toBitArray(value);
    setHumidityPowerRate1(&b);
    setHumidityPowerRate2(&b);
}

