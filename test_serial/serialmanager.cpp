#include "serialmanager.h"

#include <QMutexLocker>

SerialManager::SerialManager(QObject *parent)
    : QObject(parent)
{
    _nextSerialHandlerId = 0;
}

SerialManager::~SerialManager()
{
    stopAll();
}

void SerialManager::handlePacketReceived(int id)
{
    emit packetReceived(id);
}

bool SerialManager::start(int &id, const QString &portName, BaudRateType baudRate, DataBitsType dataBits, ParityType parity, StopBitsType stopBits, FlowType flow)
{
    QMutexLocker lock(&_mutex);

    SerialHandler* pSerialHandler = new SerialHandler(_nextSerialHandlerId, this);

    if (pSerialHandler == 0)
    {
        // error
        return false;
    }

    pSerialHandler->setPortName(portName);

    pSerialHandler->setBaudRate(baudRate);
    pSerialHandler->setDataBits(dataBits);
    pSerialHandler->setParity(parity);
    pSerialHandler->setStopBits(stopBits);
    pSerialHandler->setFlowControl(flow);

    if (pSerialHandler->startService() == false)
    {
        // error
        delete pSerialHandler;
        return false;
    }

    _serialHandlerMap.insert(_nextSerialHandlerId, pSerialHandler);

    id = _nextSerialHandlerId;
    _nextSerialHandlerId++;

    connect(pSerialHandler, SIGNAL(packetReceived(int)), this, SLOT(handlePacketReceived(int)));

    return true;
}

bool SerialManager::stop(int id)
{
    QMutexLocker lock(&_mutex);

    if (_serialHandlerMap.contains(id) == false)
    {
        return false;
    }

    SerialHandler* pSerialHandler = _serialHandlerMap.value(id);

    if (pSerialHandler == 0)
    {
        return false;
    }

    pSerialHandler->stopService();

    disconnect(pSerialHandler, SIGNAL(packetReceived(int)), this, SLOT(handlePacketReceived(int)));

    _serialHandlerMap.remove(id);
    
    delete pSerialHandler;

    return true;
}

void SerialManager::stopAll()
{
    QMutexLocker lock(&_mutex);

    QMap<int, SerialHandler*>::	iterator i;

    for (i = _serialHandlerMap.begin(); i != _serialHandlerMap.end(); ++i)
    {
        SerialHandler* pSerialHandler = i.value();

        if (pSerialHandler)
        {
            pSerialHandler->stopService();

            disconnect(pSerialHandler, SIGNAL(packetReceived(int)), this, SLOT(handlePacketReceived(int)));

            delete pSerialHandler;
        }
    }
}

QString SerialManager::getPortName(int id)
{
    QMutexLocker lock(&_mutex);

    if (_serialHandlerMap.contains(id) == false)
    {
        return QString();
    }

    SerialHandler* pSerialHandler = _serialHandlerMap.value(id);

    if (pSerialHandler == 0)
    {
        return QString();
    }

    return pSerialHandler->getPortName();
}

bool SerialManager::recvPacket(int id, Packet &packet, int sec)
{
    QMutexLocker lock(&_mutex);

    if (_serialHandlerMap.contains(id) == false)
    {
        return false;
    }

    SerialHandler* pSerialHandler = _serialHandlerMap.value(id);

    if (pSerialHandler == 0)
    {
        return false;
    }

    return pSerialHandler->recvPacket(packet, sec);
}

bool SerialManager::sendPacket(int id, const Packet &packet)
{
    QMutexLocker lock(&_mutex);

    if (_serialHandlerMap.contains(id) == false)
    {
        return false;
    }

    SerialHandler* pSerialHandler = _serialHandlerMap.value(id);

    if (pSerialHandler ==  0)
    {
        return false;
    }

    return pSerialHandler->sendPacket(packet);
}

bool SerialManager::sendAck(int id)
{
    QMutexLocker lock(&_mutex);

    if (_serialHandlerMap.contains(id) == false)
    {
        return false;
    }

    SerialHandler* pSerialHandler = _serialHandlerMap.value(id);

    if (pSerialHandler == 0)
    {
        return false;
    }

    return pSerialHandler->sendAck();
}

bool SerialManager::sendNak(int id)
{
    QMutexLocker lock(&_mutex);

    if (_serialHandlerMap.contains(id) == false)
    {
        return false;
    }

    SerialHandler* pSerialHandler = _serialHandlerMap.value(id);

    if (pSerialHandler == 0)
    {
        return false;
    }

    return pSerialHandler->sendNak();
}
