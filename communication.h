#ifndef COMMUN_H
#define COMMUN_H

#include <QObject>
#include <QtSerialPort/QtSerialPort>

#include "controlpc.h"
#include "chamber.h"

class ProcessTest;

class Communication : public QObject
{
    Q_OBJECT
private:
    QByteArray *dataReceived;
//    QThread *processThread;
    ProcessTest *process;

public:

    ControlPC *controlParams;
    Chamber *chamberParams;

    static QSerialPort *serial;

    //! ******************* public functions ****************** !//

    /**
     * @brief openPort Opens the serial port for communication
     * after setting the port settings
     */
    bool openPort();

    /**
     * @brief sendData tries to send data given to it over the serial port
     */
    void sendData(const QByteArray);

    /**
     * @brief start function. Starts the communication between pc and
     * control box. it checks if communication is in idel mood or
     * program rinning using isIdel property of ControlPC class.
     */
    void startIdelCommunication(); //! TO DO: NOT USED CURRENTLY !//

    void prepCommunication();

     //! ******************* end of public functions ****************** !//


     //! *************** constructors and destructor **************** !//
    /**
     * @brief Communication This constructor initializes the QSerialPort object and
     * @param parent
     */
    explicit Communication(QObject *parent = 0);
    explicit Communication(int a);
    ~Communication();

signals:
    void newDataArived(QByteArray newDataArived, ControlPC::CH_COMMAND);
    void unusualDataArived(QByteArray unkownData);
    void dataArived(QByteArray);
    void idelState(bool);
    void idelStateChanged(bool);
public slots:
    QByteArray readData();
    void on_idelStateChanged();
    void on_newDataArived(QByteArray newDataArived, ControlPC::CH_COMMAND);
};

#endif // COMMUN_H
