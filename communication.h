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
    bool chamberConnected;
    /**
     * @brief connectionTimer This timer is used
     * to check connnection of the pc to controll
     * box
     */
    QTimer *connectionTimer;

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




signals:
    void newDataArived(QByteArray newDataArived, ControlCommands::CH_COMMAND);
    void replyReady(ControlCommands::CH_COMMAND);
    void requestControl();
    void unusualDataArived(QByteArray unkownData);
    void dataArived(QByteArray);
    void idelState(bool);
    void idelStateChanged(bool);
    void chamberConnectionChanged(bool);

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
};

#endif // COMMUN_H
