#ifndef SERIALHANDLER_H
#define SERIALHANDLER_H

#include <QThread>
#include <QWaitCondition>
#include <QQueue>

#include "serialport.h"

class SerialHandler : public QThread
{
    Q_OBJECT
public:
    explicit SerialHandler(int id = -1, QObject* parent = 0);
    ~SerialHandler();

    void setBaudRate(BaudRateType baudRate) {_serialSettings.BaudRate = baudRate;}
    void setDataBits(DataBitsType dataBits) {_serialSettings.DataBits = dataBits;}
    void setParity(ParityType parity) {_serialSettings.Parity = parity;}
    void setStopBits(StopBitsType stopBits) {_serialSettings.StopBits = stopBits;}
    void setFlowControl(FlowType flow) {_serialSettings.FlowControl = flow;}

    void setPortName(const QString& name) {_serialPortName = name;}
    const QString& getPortName() {return _serialPortName;}

    bool startService();
    bool stopService();

    int getId();

    int getRecvPacketCount();

    bool recvPacket(Packet& packet, int sec = 0);

    bool sendPacket(const Packet& packet);

    bool sendAck();

    bool sendNak();

signals:
    void packetReceived(int id);

protected:
    virtual void run();

private:
    SerialPort _serialPort;

    PortSettings _serialSettings;

    QString _serialPortName;

    int _id;

    bool _stopFlag;

    QMutex _recvMutex;

    QWaitCondition _recvWaitCond;

    QQueue<Packet> _recvPacketQueue;
};

#endif // SERIALHANDLER_H
