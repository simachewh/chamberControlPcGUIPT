#ifndef COMMUN_H
#define COMMUN_H

#include <QObject>
#include <QtSerialPort/QtSerialPort>


class Communication : public QObject
{
    Q_OBJECT
private:
    QByteArray *dataReceived;
    QSerialPort *serial;
public:
    /**
     * @brief openPort Opens the serial port for communication
     * after setting the port settings
     */
    void openPort();

    /**
     * @brief sendData tries to send data given to it over the serial port
     */
    void sendData(const QByteArray);

    // Getters and setters
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

    // Constructors and destructor
    explicit Communication(QObject *parent = 0);
    ~Communication();

signals:
    void newData(QByteArray);
public slots:
    QByteArray readData();
};

#endif // COMMUN_H
