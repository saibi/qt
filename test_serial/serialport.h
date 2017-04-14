#ifndef SERIALPORT_H
#define SERIALPORT_H

#include "qextserialport/qextserialport.h"

#include <QtGlobal>
#include <QByteArray>
#include <QMutex>

#include "packet.h"

#ifndef E_NO_ERROR // defined in 'qextserialbase.h'
#define E_NO_ERROR                   0
#define E_INVALID_FD                 1
#define E_NO_MEMORY                  2
#define E_CAUGHT_NON_BLOCKED_SIGNAL  3
#define E_PORT_TIMEOUT               4
#define E_INVALID_DEVICE             5
#define E_BREAK_CONDITION            6
#define E_FRAMING_ERROR              7
#define E_IO_ERROR                   8
#define E_BUFFER_OVERRUN             9
#define E_RECEIVE_OVERFLOW          10
#define E_RECEIVE_PARITY_ERROR      11
#define E_TRANSMIT_OVERFLOW         12
#define E_READ_FAILED               13
#define E_WRITE_FAILED              14
#endif

#define E_UNKNOWN                   100
#define E_WRONG_FORMAT              101
#define E_WRONG_CRC                 102
#define E_NEGATIVE_ACKNOWLEDGE      103
#define E_WRONG_ACKNOWLEDGE         104


// timeout
#define DEFAULT_CONFIRMATION_TIMEOUT	6000 // 6 seconds
#define DEFAULT_PACKET_TIMEOUT		12000 // 12 seconds
#define DEFAULT_INTERCHARACTER_TIMEOUT	5000 // 5 seconds


class SerialPort : public QextSerialPort
{
public:
    SerialPort();
    SerialPort(const QString & name);
    SerialPort(PortSettings const& s);
    SerialPort(const QString & name, PortSettings const& s);
    SerialPort(const SerialPort& s);
    SerialPort& operator=(const SerialPort&);
    virtual ~SerialPort();

    // wait for recv
    bool waitForRecv(int msecs);

#if 0
    bool readLinePacket(int msecs, Packet& packet);
    bool readLinePacket(Packet& packet) {return readLinePacket(-1, packet);}
#endif

    // recv
    bool recvPacket(int msecs, Packet& packet, bool autoAck = true);
    bool recvPacket(Packet& packet, bool autoAck = true) {return recvPacket(-1, packet, autoAck);}

    bool recvAck(int msecs = DEFAULT_CONFIRMATION_TIMEOUT);

    // send
    bool sendPacketNoWait(const Packet& packet);

    bool sendPacket(const Packet& packet, int msecs = DEFAULT_CONFIRMATION_TIMEOUT);

    bool sendAck(bool ackFlag = true);
    bool sendNak() {return sendAck(false);}

    //
    void resetRecvBuffer();

private:
    QByteArray _recvBuffer;

    QMutex _mutex;
};

#endif // SERIALPORT_H
