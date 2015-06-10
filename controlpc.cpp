#include "controlpc.h"
#include <QDebug>

ControlPC::ControlPC(QObject *parent) : QObject(parent)
{
    stx = 0x02;
    etx = 0x03;
    zero = 0x30;
    capO = 0x4F;
    qMark = 0x3F;
    lineBreak = 0x0D;
    command1 = command2 = command3 = command4 = 0x30;
    iy.i = 0X49;
    iy.y = 0x79;
    aq.a = 0x41;
    aq.q = 0x71;
    br.b = 0x42;
    br.r = 0x72;
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
    commandBr->append(br.r);
    commandBr->append(lineBreak);

    return *commandBr;
}

QByteArray ControlPC::idelCommand(){
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
