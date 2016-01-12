#include "controlcommands.h"
#include "communication.h"
#include <QDebug>

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
    htBlock = 48;
    plBlock = 48;
    cvBlock = 48;
    vFBlock = 48;
    humidityPowerRate1 = 48;
    humidityPowerRate2 = 48;
    temperaturePowerRate1 = 48;
    temperaturePowerRate2 = 48;
    temperaturePower = 0;
    humidityPower = 0;
    cksum = new QByteArray();
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
        qDebug() << "capO " << capO;
        assembledCommand.append(twelveZeros);
        assembledCommand.append(htBlock);
        qDebug() << "htBlock" << htBlock;
        assembledCommand.append(plBlock);
        qDebug() << "plBlock" << plBlock;
        assembledCommand.append(cvBlock);
        qDebug() << "cvBlock" << cvBlock;
        assembledCommand.append(vFBlock);
        qDebug() <<"vfBlock " << vFBlock;
        assembledCommand.append(humidityPowerRate1);
        qDebug() << "humidrate1" << humidityPowerRate1;
        assembledCommand.append(humidityPowerRate2);
        qDebug() << "humidrate2" << humidityPowerRate2;
        assembledCommand.append(temperaturePowerRate1);
        qDebug() << "temprate1" << temperaturePowerRate1;
        assembledCommand.append(temperaturePowerRate2);
        qDebug() << "temprate2 " << temperaturePowerRate2;
        assembledCommand.append(etx);
        appendChecksum(&assembledCommand);
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
    for(int i = 1; i < value->size() - 1; i++){
        cks += value->at(i);
    }
    QByteArray sum(1, cks);
    qDebug() << "ControlCommands::appendChecksum " << sum;
    value->append(sum);
}

void ControlCommands::switchValves(bool value)
{
    setV1(value);
    setV3(value);
    setV4(value);
    setC2V2(value);
}

void ControlCommands::switchHeaters(bool value)
{
    setT1(value);
    setT2(value);
}

void ControlCommands::switchCooler(bool value)
{
    setC1(value);
    setC2V2(value);
}

void ControlCommands::switchHumidifiers(bool value)
{
    setH1(value);
    setH2(value);
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
    ba.append(toByte(htBlock));
    ba.append(toByte(plBlock));
    ba.append(toByte(cvBlock));
    ba.append(toByte(vFBlock));
    ba.append(toByte(humidityPowerRate1));
    ba.append(toByte(humidityPowerRate2));
    ba.append(toByte(temperaturePowerRate1));
    ba.append(toByte(temperaturePowerRate2));

    return calculateCksum(&ba);
}

QByteArray ControlCommands::toByte(std::bitset<8> bits){
    QByteArray byte;
    char c;
    unsigned long i = bits.to_ulong();
    c = static_cast<char>(i);
    byte.append(c);
    return byte;
}

QByteArray ControlCommands::toByte(int value){
    QByteArray bytes;
    if(value > 255){
        /// do nothing, return, log this error
    }
    char c = value;
    bytes.append(c);
    return bytes.mid(7);
}

std::bitset<8> ControlCommands::toBitArray(int value){
    std::bitset<8> bits(value);
    return bits;
}

void ControlCommands::resetAll()
{
    //! sitch of all devices & set power outputs to zero
    setIdle(true);
    setHumidityPower(0);
    setTemperaturePower(0);
    switchCooler(false);
    switchHeaters(false);
    switchHumidifiers(false);
    switchValves(false);

    qDebug() << "ControlCommands::resetAll : finishing";
}

void ControlCommands::on_chPartChanged(bool value, ControlCommands::CH_PART part){
    switch (part) {
    case H1:
    {
        qDebug() << "on_chPartChanged: " << "H1 = " << value;
        std::bitset<8> bits(htBlock);
        if(value){
            bits.set(2);
        }else{
            bits.reset(2);
        }
        htBlock = static_cast<char>(bits.to_ulong());
    }
        break;
    case H2:
    {
        std::bitset<8> bits(htBlock);
        if(value){
            bits.set(3);
        }else{
            bits.reset(3);
        }
        htBlock = static_cast<char>(bits.to_ulong());
    }
        break;
    case T1:
    {
        std::bitset<8> bits(htBlock);
        if(value){
           bits.set(0);
        }else{
            bits.reset(0);
        }
        htBlock = static_cast<char>(bits.to_ulong());
    }
        break;
    case T2:
    {
        std::bitset<8> bits(htBlock);
        if(value){
            bits.set(1);
        }else{
         bits.reset(1);
        }
        htBlock = static_cast<char>(bits.to_ulong());
    }
        break;
    case P1:
    {
        std::bitset<8> bits(plBlock);
        if(value){
         bits.set(1);
        }else{
            bits.reset(1);
        }
        plBlock = static_cast<char>(bits.to_ulong());
    }
        break;
    case P2:
    {
        std::bitset<8> bits(plBlock);
        if(value){
         bits.set(2)   ;
        }else{
            bits.reset(2);
        }
        plBlock = static_cast<char>(bits.to_ulong());
    }
        break;
    case P3:
    {
        std::bitset<8> bits(plBlock);
        if(value){
         bits.set(3);
        }else{
            bits.reset(3);
        }
        plBlock = static_cast<char>(bits.to_ulong());
    }
        break;
    case LNU:
    {
        std::bitset<8> bits(plBlock);
        if(value){
         bits.set(0);
        }else{
            bits.reset(0);
        }
        plBlock = static_cast<char>(bits.to_ulong());
    }
        break;
    case C1:
    {
        std::bitset<8> bits(cvBlock);
        if(value){
         bits.set(1)   ;
        }else{
            bits.reset(1);
        }
        cvBlock = static_cast<char>(bits.to_ulong());
    }
        break;
    case V4:
    {
        std::bitset<8> bits(cvBlock);
        if(value){
            bits.set(0);
        }else{
         bits.reset(0);
        }
        cvBlock = static_cast<char>(bits.to_ulong());
    }
        break;
    case C2V2:
    {
        std::bitset<8> bits(vFBlock);
        if(value){
            bits.set(2);
        }else{
         bits.reset(2);
        }
        vFBlock = static_cast<char>(bits.to_ulong());
    }
        break;
    case V1:
    {
        std::bitset<8> bits(vFBlock);
        if(value){
         bits.set(1)   ;
        }else{
            bits.reset(1);
        }
        vFBlock = static_cast<char>(bits.to_ulong());
    }
        break;
    case V3:
    {
        std::bitset<8> bits(vFBlock);
        if(value){
         bits.set(3);
        }else{
            bits.reset(3);
        }
        vFBlock = static_cast<char>(bits.to_ulong());
    }
        break;
    case FAN:
    {
        std::bitset<8> bits(vFBlock);
        if(value){
         bits.set(0);
        }else{
            bits.reset(0);
        }
        vFBlock = static_cast<char>(bits.to_ulong());
    }
        break;
    default:
        break;
    }
}

void ControlCommands::on_temperaturePowerChanged(int value){
    std::bitset<8> b = toBitArray(value);
    setTemperaturePowerRate1(&b);
    setTemperaturePowerRate2(&b);
}

void ControlCommands::on_humidityPowerChanged(int value){
    std::bitset<8> b = toBitArray(value);
    setHumidityPowerRate1(&b);
    setHumidityPowerRate2(&b);
}

char ControlCommands::getTemperaturePowerRate1() const
{
    return temperaturePowerRate1;
}

void ControlCommands::setTemperaturePowerRate1(std::bitset<8> *value)
{
    std::bitset<8> bits(temperaturePowerRate1);
    bits[4] = (*value)[0];
    bits[5] = (*value)[1];
    bits[6] = (*value)[2];
    bits[7] = (*value)[3];
}

char ControlCommands::getTemperaturePowerRate2() const
{
    return temperaturePowerRate2;
}

void ControlCommands::setTemperaturePowerRate2(std::bitset<8> *value)
{
    std::bitset<8> bits(temperaturePowerRate2);
    bits[4] = (*value)[4];
    bits[5] = (*value)[5];
    bits[6] = (*value)[6];
    bits[7] = (*value)[7];
}

char ControlCommands::getHumidityPowerRate2() const
{
    return humidityPowerRate2;
}

void ControlCommands::setHumidityPowerRate2(std::bitset<8> *value)
{
    std::bitset<8> bits(humidityPowerRate2);
    bits[4] = (*value)[4];
    bits[5] = (*value)[5];
    bits[6] = (*value)[6];
    bits[7] = (*value)[7];
}

char ControlCommands::getHumidityPowerRate1() const
{
    return humidityPowerRate1;
}

void ControlCommands::setHumidityPowerRate1(std::bitset<8> *value)
{
    std::bitset<8> bits(humidityPowerRate1);
    bits[4] = (*value)[0];
    bits[5] = (*value)[1];
    bits[6] = (*value)[2];
    bits[7] = (*value)[3];
}

bool ControlCommands::isIdle(){
    return ControlCommands::idle;
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
        emit chPartChanged(value, ControlCommands::H1);
    }
}

void ControlCommands::setH2(bool value){
    if(h2 != value){
        h2 = value;
        emit chPartChanged(value, ControlCommands::H2);
    }
}

void ControlCommands::setT1(bool value){
    if(t1 != value){
        t1 = value;
        emit chPartChanged(value, ControlCommands::T1);
    }
}

void ControlCommands::setT2(bool value){
    if(t2 != value){
        t2 = value;
        emit chPartChanged(value, ControlCommands::T2);
    }
}

void ControlCommands::setP1(bool value){
    if(p1 != value){
        p1 = value;
        emit chPartChanged(value, ControlCommands::P1);
    }
}

void ControlCommands::setP2(bool value){
    if(p2 != value){
        p2 = value;
        emit chPartChanged(value, ControlCommands::P2);
    }
}

void ControlCommands::setP3(bool value){
    if(p3 != value){
        p3 = value;
        emit chPartChanged(value, ControlCommands::P3);
    }
}

void ControlCommands::setLNU(bool value){
    if(lnu != value){
        lnu = value;
        emit chPartChanged(value, ControlCommands::LNU);
    }
}

void ControlCommands::setC1(bool value){
    if(c1 != value){
        c1 = value;
        emit chPartChanged(value, ControlCommands::C1);
    }
}

void ControlCommands::setC2V2(bool value){
    if(c2v2 != value){
        c2v2 = value;
        emit chPartChanged(value, ControlCommands::C2V2);
    }
}

void ControlCommands::setV1(bool value){
    if(v1 != value){
        v1 = value;
        emit chPartChanged(value, ControlCommands::V1);
    }
}

void ControlCommands::setV3(bool value){
    if(v3 != value){
        v3 = value;
        emit chPartChanged(value, ControlCommands::V3);
    }
}

void ControlCommands::setV4(bool value){
    if(v4 != value){
        v4 = value;
        emit chPartChanged(value, ControlCommands::V4);
    }
}

void ControlCommands::setFan(bool value){
    if(fan != value){
        fan = value;
        emit chPartChanged(value, ControlCommands::FAN);
    }
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
    if(!(isIdle() == idelState)){
        idle = idelState;
        emit idleStateChanged(idelState);
    }
}
