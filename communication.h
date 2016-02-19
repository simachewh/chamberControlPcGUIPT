#ifndef COMMUN_H
#define COMMUN_H

#include <QObject>
#include <QtSerialPort/QtSerialPort>

#include "controlcommands.h"
#include "chamber.h"
#include "controller.h"

class ProcessTest;

class Communication : public QObject
{
    Q_OBJECT
private:
    QByteArray *dataReceived;

    /**
     * @brief chamberConnected Property to hold the connection state
     * of the control Pc to the control box. Proper use would be to set
     * it true for when the connection is established and false when it
     * is lost.
     */
    bool chamberConnected;

    /**
     * @brief connectionTimer This timer is used
     * to check connnection of the pc to controll
     * box
     */
    QTimer *connectionTimer;

    int stateCounter;

public:

   // ControlPC *controlParams;
    Controller *pidController;

    static QSerialPort *serial;

    /**
     * @brief Communication This constructor initializes the QSerialPort object and
     * @param parent
     */
    explicit Communication(QObject *parent = 0);

    ~Communication();

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
    void startIdelCommunication(); //! TO DO: !//

    /**
     * @brief isChamberConnected gets the chamberconnection state value.
     * Chamber connection value is represented by the chamberConnected
     * property.
     * @see Communication::chamberConnected
     * @return Returns the value of Communication::chamberConnected
     */
    bool isChamberConnected();

    /**
     * @brief setChamberConnected A functio to set the value of
     * Communication::chamberConnected. If the given value is
     * different from Communication::chamberConnected its value will
     * be changed and notified.
     * @param value A bool value to set Communication::chamberConnected
     */
    void setChamberConnected(bool value);

    int getStateCounter();

    void setStateCounter(int value);

    void stateCounterIncrement();




    bool getChamberConnected() const;

signals:
    void newDataArived(QByteArray newDataArived, ControlCommands::CH_COMMAND);
    void replyReady(ControlCommands::CH_COMMAND);
    void requestControl();
    void unusualDataArived(QByteArray unkownData);
    void dataArived(QByteArray);
    void idelState(bool);
    void idelStateChanged(bool);
    void chamberConnectionChanged(bool);
    void connectionLost(bool);
    void stateCounterChanged(int);

public slots:

    /**
     * @brief Communication::readData functopn.
     * Reads data available in the serial device. It calles the readAll() method from
     * QSerialPort class.
     * @return the recieved data as QByteArray.
     */
    QByteArray readData();
    void on_idelStateChanged();
    void on_newDataArived(QByteArray newDataArived, ControlCommands::CH_COMMAND chCommand);
    void on_chamberConnectionChanged(bool value);
    void on_controlReady();
    void reply(ControlCommands::CH_COMMAND chCommand);

    /**
     * @brief on_connectionTimerOut Starts communication by sending
     * BR command to the control box and tracks the connection status
     * of this application to the control box.
     * this slot is convinently made to be connected
     * with the connectionTimer->timeout() signal.
     */
    void on_connectionTimerOut();

    void on_stateCounterChanged(int value);
};

#endif // COMMUN_H
