#ifndef SERIALMANAGER_H
#define SERIALMANAGER_H

#include <QObject>
#include <QMap>
#include <QMutex>
#include "serialhandler.h"

class SerialManager : public QObject
{
    Q_OBJECT
public:
    explicit SerialManager(QObject *parent = 0);
    ~SerialManager();

    bool start(int& id, // out
               const QString& portName,
               BaudRateType baudRate = BAUD115200,
               DataBitsType dataBits = DATA_8,
               ParityType parity = PAR_NONE,
               StopBitsType stopBits = STOP_1,
               FlowType flow = FLOW_OFF);

    bool stop(int id);

    void stopAll();

    QString getPortName(int id);

    bool recvPacket(int id, Packet& packet, int sec = 0);

    bool sendPacket(int id, const Packet& packet);

    bool sendAck(int id);

    bool sendNak(int id);

signals:
    void packetReceived(int id);

public slots:
    void handlePacketReceived(int id);

private:
    QMap<int, SerialHandler*> _serialHandlerMap;

    int _nextSerialHandlerId;

    QMutex _mutex;
};

#endif // SERIALMANAGER_H
