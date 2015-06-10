#ifndef CONTROLLPC_H
#define CONTROLLPC_H

#include <QObject>

class ControlPC : public QObject
{
    Q_OBJECT
public:
//private:
    /**
     * @brief The Anonymous:1 struct holds vlues of hex
     * I and Y
     */
    struct{
        char i;
        char y;
    }iy;

    /**
     * @brief The Anonymous:1 struct, holds values of hex
     * A and Q
     */
    struct{
        char a;
        char q;
    }aq;

    /**
     * @brief The Anonymous:1 struct, holds values of hex
     * B and R
     */
    struct{
        char b;
        char r;
    }br;

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
    char command1;
    /**
     * @brief command2 Represents the second byte of the command block
     * in the serial protocol, each bit representing ?, ?, C1, V4, V3, V2/C2, V1, FAN
     * respectively from HSB to LSB
     */
    char command2;
    /**
     * @brief command3 Represents the third byte of the command block in the serial
     * protocol representing the humidity bar, 8bit high being full and 8bits low being
     * no bars.
     */
    char command3;
    /**
     * @brief command4 Represents the fourth byte in the command block of the serial
     * protocol. Holds bits to represent the temprature bar. 8bits high means full bar
     * and 8bits low means empty bar.
     */
    char command4;


//public:

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

    QByteArray idelCommand();

signals:

public slots:
};

#endif // CONTROLLPC_H
