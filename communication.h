#ifndef COMMUN_H
#define COMMUN_H

#include <QObject>
#include <QtSerialPort/QtSerialPort>

#include "controlpc.h"
#include "chamber.h"

class Communication : public QObject
{
    Q_OBJECT
private:
    QByteArray *dataReceived;

public:

    ControlPC *controlParams;
    Chamber *chamberParams;

    QSerialPort *serial;

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

    //! ***************** Getters and setters ***************** !//
    /** Returns the pointer to the property of type QSerialPort object.
     * @brief Communication::serialPort
     * @return
     */
    QSerialPort * SerialPort();

    /**
     * @brief setSerialPort sets the value pointed by the property serial to the
     * given QSerialPort
     * @param port
     */
    void setSerialPort(QSerialPort port);

    /** Returns the value pointed to by the dataRecieved property of Communication class.
     * @brief Communication::DataReceived
     * @return
     */
    QByteArray DataReceived();

    /**
     * @brief setDataReceived Sets the value pointed by the property dataReceived to the
     * given QByteArray.
     * @param bytes
     */
    void setDataReceived(QByteArray bytes);



     //constructors and destructor
    /**
     * @brief Communication This constructor initializes the QSerialPort object and
     * @param parent
     */
    explicit Communication(QObject *parent = 0);
    ~Communication();

signals:
    void newDataArived(QByteArray newDataArived);
    void dataArived();
    void idelState(bool);
    void idelStateChanged(bool);
public slots:
    QByteArray readData();
    void on_idelStateChanged();
    void on_newDataArived(QByteArray newDataArived);
};

#endif // COMMUN_H
