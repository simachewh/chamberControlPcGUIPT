#ifndef CONTROLLPC_H
#define CONTROLLPC_H

#include <QObject>
#include <QBitArray>
#include <QByteArray>
#include <bitset>

#include "program.h"
#include "chamber.h"


/**
 * @brief The ControlPC class.
 * This class holds properties needed to construct the commands
 * to be written by the control pc to the control box over serial
 * protocol.
 * @author Simachew Tibebu
 */
class ControlCommands : public QObject
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

//    Program *testProgram;
//    Chamber *climateChamber;


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
     * @brief htBlock this array represents the command block that holds information
     * about H1, H2, T1, and T2 devices activation.
     */
    unsigned char htBlock;


    /**
     * @brief plBlock represents the command block that holds information about P1, P2,
     * P3 and LNV devices activation.
     */
    unsigned char plBlock;

    /**
     * @brief cvBlock represents the command block that holds information about C1 and V4
     * device activation.
     */
    unsigned char cvBlock;

    /**
     * @brief vFBlock represents the command block that holds information about V1, V2/C2,
     * V3 and FAN devices activation.
     */
    unsigned char vFBlock;

    /**
     * @brief humidityBar1 represents the first section of the humidity power block.
     */
    unsigned char humidityPowerRate1;

    /**
     * @brief humidityBar2 represents the second section of the humidity power block.
     */
    unsigned char humidityPowerRate2;

    /**
     * @brief temperatureBar1 represents the first section of the temprature power block.
     */
    unsigned char temperaturePowerRate1;

    /**
     * @brief temperaturePowerRate2 represents the second section of the tempreature power block.
     */
    unsigned char temperaturePowerRate2;

    int humidityPower;

    int temperaturePower;


    /**
     * @brief ControllPC This constructor initializes start to HEX 02,
     * end to HEX 0D and the rest of blocks of command to HEX 30
     * @param parent
     */
    explicit ControlCommands(QObject *parent = 0);
    ~ControlCommands();


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

    /**
     * @brief fullCommand Function constracts the type of command
     * described by the given enum type, PC_COMMAND, in to a QByteArray
     * and returns it.
     * @param ctype
     * @return
     */
    QByteArray fullCommand(PC_COMMAND ctype);

    QByteArray getCalculatedCksum();

    /**
     * @brief convertToBytes Converts the given QBitArray to a QByteArray.
     * @param bits QBitArray.
     * @return bytes QByteArray.
     */
    QByteArray toByte(std::bitset<8> bits);

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
    QByteArray toByte(int value);
    QByteArray calculateCksum(QByteArray *value);
    void initBlocks();
    std::bitset<8> toBitArray(int value);


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

    QByteArray getHumidityBar();

    QByteArray getCksum();

    void setCksum(QByteArray value);

    void setTemperaturePower(int value);

    int getTempraturePower();

    void setHumidityPower(int value);

    int getHumidityPower();

    char getTemperaturePowerRate1() const;

    void setTemperaturePowerRate1(std::bitset<8> *value);

    char getTemperaturePowerRate2() const;

    void setTemperaturePowerRate2(std::bitset<8> *value);

    char getHumidityPowerRate2() const;

    void setHumidityPowerRate2(std::bitset<8> *value);

    char getHumidityPowerRate1() const;

    void setHumidityPowerRate1(std::bitset<8> *value);

    void appendChecksum(QByteArray *value);
signals:
    void idleStateChanged();
    void pcCommandChanged(ControlCommands::PC_COMMAND PC_COMMAND);
    void chCommandChanged(ControlCommands::CH_COMMAND CH_COMMAND);

    void chPartChanged(bool value, ControlCommands::CH_PART);
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
    void on_chPartChanged(bool value,
                          ControlCommands::CH_PART part);

    /**
     * @brief on_temperaturePowerChanged This function
     * takes the given integer convert it to byte and
     * sets the temperatureBar progperty. It will make
     * a call to setTemperatureBar().
     * @param value the integer value of tempertature power.
     * this value shoul be [0, 255]
     */
    void on_temperaturePowerChanged(int value);
    void on_humidityPowerChanged(int value);

    void resetAll();
};

#endif // CONTROLLPC_H
//correction is a must
