#include "serialhandler.h"

#include <QMutexLocker>

#define WAIT_FOR_READ_MSEC    50

static QMutex g_mutex;

SerialHandler::SerialHandler(int id, QObject* parent)
 : QThread(parent)
{
    _id = id;

    _stopFlag = true;

    _serialSettings.BaudRate = BAUD115200;
    _serialSettings.DataBits = DATA_8;
    _serialSettings.Parity = PAR_NONE;
    _serialSettings.StopBits = STOP_1;
    _serialSettings.FlowControl = FLOW_OFF;
}

SerialHandler::~SerialHandler()
{
}

int SerialHandler::getId()
{
    return _id;
}

bool SerialHandler::startService()
{
    QMutexLocker locker(&g_mutex);

    if (isRunning())
        return false;

    _stopFlag = false;

    if (_serialPort.isOpen())
        _serialPort.close();

    _serialPort.setBaudRate(_serialSettings.BaudRate);
    _serialPort.setDataBits(_serialSettings.DataBits);
    _serialPort.setParity(_serialSettings.Parity);
    _serialPort.setStopBits(_serialSettings.StopBits);
    _serialPort.setFlowControl(_serialSettings.FlowControl);

    _serialPort.setPortName(_serialPortName);

    if (_serialPort.open(QIODevice::ReadWrite) == false)
        return false;

    start();

    return true;
}

bool SerialHandler::stopService()
{
    QMutexLocker locker(&g_mutex);

    _stopFlag = true;

    wait();

    if (_serialPort.isOpen())
        _serialPort.close();

    return true;
}

void SerialHandler::run()
{
    while(_stopFlag != true)
    {
        Packet packet;

        if (_serialPort.recvPacket(WAIT_FOR_READ_MSEC, packet, false))
        {
            // received
            {
                QMutexLocker lockerRecv(&_recvMutex);

                packet.setCreatedTime();

                _recvPacketQueue.enqueue(packet);
            }

            emit packetReceived(_id);

            _recvWaitCond.wakeOne();
        }
        else
        {
            if (_serialPort.lastError() != E_PORT_TIMEOUT)
            {
                // error
                qWarning("[SerialHandler::run] Error Code: %lu\n", _serialPort.lastError());
            }
        }
    }

    _recvWaitCond.wakeAll();
}

int SerialHandler::getRecvPacketCount()
{
    QMutexLocker lockerRecv(&_recvMutex);

    return (int) _recvPacketQueue.size();
}

bool SerialHandler::recvPacket(Packet &packet, int sec)
{
    QMutexLocker lockerRecv(&_recvMutex);

    if (_recvPacketQueue.size() > 0)
    {
        packet = _recvPacketQueue.dequeue();
        return true;
    }

    if (sec == 0)
    {
        // no wait
        return false;
    }
    else if (sec < 0)
    {
        // infinite wait
        _recvWaitCond.wait(lockerRecv.mutex());
    }
    else
    {
        // timed wait
        _recvWaitCond.wait(lockerRecv.mutex(), sec * 1000);
    }

    if (_recvPacketQueue.size() > 0)
    {
        packet = _recvPacketQueue.dequeue();
        return true;
    }

    return false;
}

bool SerialHandler::sendPacket(const Packet &packet)
{
    return _serialPort.sendPacketNoWait(packet);
}

bool SerialHandler::sendAck()
{
    return _serialPort.sendAck();
}

bool SerialHandler::sendNak()
{
    return _serialPort.sendNak();
}
