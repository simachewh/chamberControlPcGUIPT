#ifndef CONTROLLPC_H
#define CONTROLLPC_H

#include <QObject>
#include <QBitArray>
#include <QByteArray>

#include "program.h"
#include "chamber.h"


/**
 * @brief The ControlPC class.
 * This class holds properties needed to construct the commands
 * to be written by the control pc to the control box over serial
 * protocol.
 * @author Simachew Tibebu
 */
class ControlPC : public QObject
{
    Q_OBJECT

private:

    bool h1;

    bool h2;

    bool t1;

    bool t2;

    bool p1;

    bool p2;

    bool p3;

    bool lnu;

    bool c1;

    bool c2v2;

    bool v1;

    bool v3;

    bool v4;

    bool fan;

    QByteArray *cksum;

    /**
     * @brief isIdel holds value true when there is no test program runing.
     */
    static bool isIdle;

public:

    enum PC_COMMAND {BR, AQ, IY, O = 'O'};
    enum CH_COMMAND {ACK = 0x06, A = 'A', B = 'B', I = 'I'};
    enum CH_PART {H1, H2, T1, T2, P1, P2, P3, LNU, C1, C2V2, V1, V3, V4, FAN, Temp_Power, Hum_Power};

    Program *testProgram;



    /**
     * @brief The Anonymous:1 struct holds vlues of hex
     * I and Y.
     */
    struct iyComm{
        char i;
        char y;
    }iy;

    /**
     * @brief The Anonymous:1 struct, holds values of hex
     * A and Q.
     */
    struct aqComm{
        char a;
        char q;
    }aq;

    /**
     * @brief The Anonymous:1 struct, holds values of hex
     * B and R.
     */
    struct brComm{
        char b;
        char r;
    }br;

    /**
     * @brief commandBody Main part of the command that will be
     * interpreted by the control box.
     * This will hold data part of the command which latter, start
     * and end parts can be appended up on to construct full writable command.
     * Some functions in this class are responsible for the construction of this
     * command.
     */
    QByteArray * commandBody;

    /**
     * @brief comleteCommand Holds values of the complete
     * command ready to be written on serial device.
     */
    QByteArray *comleteCommand;

    /**
     * @brief start Represents the STX, start of text, in the serial
     * protocol - HEX 02.
     */
    char stx;

    /**
     * @brief etx Represents the ETX, end of text, in the serial
     * protocol - HEX 03.
     */
    char etx;

    /**
     * @brief zero holds hex zero
     */
    char zero;

    /**
     * @brief capO holds hex capital O
     */
    char capO;

    /**
     * @brief qMark holds hex Question mark (?)
     */
    char qMark;

    /**
     * @brief end Represents the end of text in the serial
     * protocol - HEX 0D.
     */
    char lineBreak;

    /**
     * @brief command1 represents the first byte in the command
     * block of the serial protocol which holds values for
     * H2, H1, T2, T1, P3, P2, P1, LNU each bit representing these
     * components respectively, H2 being HSB and LNU represented by LSB
     */
    QBitArray * commandBlock1;

    QByteArray * block1;
    /**
     * @brief command2 Represents the second byte of the command block
     * in the serial protocol, each bit representing ?, ?, C1, V4, V3, V2/C2, V1, FAN
     * respectively from HSB to LSB
     */
    QBitArray * commandBlock2;

    QByteArray * block2;

    /**
     * @brief humidityBar Represents the third byte of the command block in the serial
     * protocol representing the humidity bar. 8bit high being full and 8bits low being
     * no bars.
     */
    QByteArray * humidityBar;
    int humidityPower;

    /**
     * @brief temperatureBar Represents the fourth byte in the command block of the serial
     * protocol. Holds bits to represent the temprature progress bar. 8bits high means full bar
     * and 8bits low means empty bar.
     */
    QByteArray * temperatureBar;
    int temperaturePower;


    /**
     * @brief ControllPC This constructor initializes start to HEX 02,
     * end to HEX 0D and the rest of blocks of command to HEX 30
     * @param parent
     */
    explicit ControlPC(QObject *parent = 0);
    ~ControlPC();

    /**
     * @brief iyCommand constructs the command with 0iy
     * @return
     */
    QByteArray iyCommand();

    /**
     * @brief aqCommand Constructs the command with 0aq
     * @return
     */
    QByteArray aqCommand();

    /**
     * @brief brCommand constructs the command with 0br
     * @return
     */
    QByteArray brCommand();

    QByteArray idleCommand();

    QByteArray fullCommand(PC_COMMAND ctype);

    QByteArray calculatecksum();

    bool getIsIdle();

    bool getH1();

    void setH1(bool value);

    bool getH2();

    void setH2(bool value);

    bool getT1();

    void setT1(bool value);

    bool getT2();

    void setT2(bool value);

    bool getP1();

    void setP1(bool value);

    bool getP2();

    void setP2(bool value);

    bool getP3();

    void setP3(bool value);

    bool getLNU();

    void setLNU(bool value);

    bool getC1();

    void setC1(bool value);

    bool getC2V2();

    void setC2V2(bool value);

    bool getV1();

    void setV1(bool value);

    bool getV3();

    void setV3(bool value);

    bool getV4();

    void setV4(bool value);

    bool getFan();

    void setFan(bool value);

    QByteArray getTempratureBar();

    void setTemperatureBar(QByteArray value);

    QByteArray getHumidityBar();

    void setHumidityBar(QByteArray value);

    QByteArray getCksum();

    void setCksum(QByteArray value);



    void setTemperaturePower(int value);
    int getTempraturePower();
    void setHumidityPower(int value);
    int getHumidityPower();

    /**
     * @brief convertToBytes Converts the given QBitArray to a QByteArray.
     * @param bits QBitArray.
     * @return bytes QByteArray.
     */
    QByteArray convertToBytes(QBitArray bits);

    /**
     * @brief convertToBytes Converts the given integer to QByteArray.
     * For the use of this application the given integer will be between
     * 0 and 255, Hence the byte value representing this given integer is
     * found at the seventh index, meaning the result comming from QByteArray.at(7).
     * The resulting return value is this byte only wraped in the array. If given
     * an integer greater than 255 expect to have unpredictable results.
     * @param value
     * @return
     */
    QByteArray convertToBytes(int value);
    QByteArray calculatecksum(QByteArray value);
signals:
    void idleStateChanged();
    void pcCommandChanged(ControlPC::PC_COMMAND PC_COMMAND);
    void chCommandChanged(ControlPC::CH_COMMAND CH_COMMAND);

    void chPartChanged(bool value, ControlPC::CH_PART);
    void temperaturePowerChanged(int);
    void humidityPowerChanged(int);

    void block1Changed();
    void block2Changed();

    void h1Canged(bool h1);
    void h2Changed(bool h2);
    void t1Changed(bool t1);
    void t2Changed(bool t2);
    void p1Changed(bool p1);
    void p2Changed(bool p2);
    void p3Changed(bool p3);
    void lnuChaged(bool lnu);
    void c1Changed(bool c1);
    void c2v2Changed(bool c2v2);
    void v1Changed(bool v1);
    void v3Changed(bool v3);
    void v4Changed(bool v4);
    void fanChanged(bool fan);
    void cksumChanged(QByteArray value);

public slots:
    void setIdle(bool idelState);
    void on_chPartChanged(bool value, ControlPC::CH_PART part);
    void on_temperaturePowerChanged(int value);
    void on_humidityPowerChanged(int value);
};

#endif // CONTROLLPC_H
//correction is a must
