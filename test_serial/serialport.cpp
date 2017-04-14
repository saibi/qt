#include "serialport.h"

#include <QMutexLocker>

#include "parser.h"
#include "record.h"

#define WAIT_FOR_RECV_MSEC    50

#define MAX_PACKET_SIZE    (1024 * 1024)

SerialPort::SerialPort()
 : QextSerialPort(), _mutex(QMutex::Recursive)
{
}

SerialPort::SerialPort(const QString& name)
 : QextSerialPort(name), _mutex(QMutex::Recursive)
{
}

SerialPort::SerialPort(PortSettings const& settings)
 : QextSerialPort(settings), _mutex(QMutex::Recursive)
{
}

SerialPort::SerialPort(const QString& name, PortSettings const& settings)
 : QextSerialPort(name, settings), _mutex(QMutex::Recursive)
{
}

SerialPort::SerialPort(const SerialPort& s)
 : QextSerialPort(s), _mutex(QMutex::Recursive)
{
}

SerialPort& SerialPort::operator=(const SerialPort& s)
{
    resetRecvBuffer();

    return (SerialPort&)QextSerialPort::operator=(s);
}

SerialPort::~SerialPort()
{
}

bool SerialPort::waitForRecv(int msecs)
{
    QMutexLocker locker(&_mutex);

    if (!isOpen() || !isReadable())
    {
        lastErr = E_IO_ERROR;
        return false;
    }

    qint64 bytes = this->size() + QIODevice::bytesAvailable();
    //qint64 bytes = this->bytesAvailable();

    if (bytes > 0)
    {
        lastErr = E_NO_ERROR;
        return true;
    }
    else if (msecs == 0)
    {
        lastErr = E_PORT_TIMEOUT;
        return false;
    }

    int idx = 0;

    while (1)
    {
        bytes = this->bytesAvailable();

        if (bytes == -1)
        {
            // lastErr was set by bytesAvailable()
            if (lastErr != E_PORT_TIMEOUT)
            {
                return false;
            }
        }
        else if (bytes > 0)
        {
            lastErr = E_NO_ERROR;
            return true;
        }

#if defined(WIN32)
        ::Sleep(WAIT_FOR_RECV_MSEC);
#else
        {
            struct timespec delay;
            delay.tv_sec = 0;
            delay.tv_nsec = WAIT_FOR_RECV_MSEC * 1000000;
            nanosleep(&delay, NULL);
        }
        //usleep(WAIT_FOR_RECV_MSEC*1000);
#endif

        // infinite wait
        if (msecs < 0)
            continue;

        // timed wait
        idx++;

        if ((idx * WAIT_FOR_RECV_MSEC) >= msecs)
        {
            lastErr = E_PORT_TIMEOUT;
            return false;
        }

        if (!isOpen() || !isReadable())
        {
            lastErr = E_IO_ERROR;
            return false;
        }
    }

    return false;
}

#if 0
bool SerialPort::readLinePacket(int msecs, Packet& packet)
{
    QMutexLocker locker(&_mutex);

    packet.reset();

    QByteArray buffer;
    int i;
    bool isEnded;

    while (1)
    {
        buffer.clear();
        isEnded = false;

        if (_recvBuffer.size() > 0)
        {
            buffer = _recvBuffer;
            _recvBuffer.clear();
        }
        else
        {
            if (waitForRecv(msecs) == false)
            {
                // lastErr was set by waitForRecv()
                return false;
            }

            buffer = this->readAll();

            if (buffer.size() == 0)
            {
                // maybe, lastErr was set by readAll()
                return false;
            }
        }

        for (i = 0; i < buffer.size(); i++)
        {
            if (buffer.at(i) == RC_LF || buffer.at(i) == RC_CR)
            {
                isEnded = true;
                break;
            }
        }

        if (i > 0)
            packet.append(buffer.constData(), i);

        if (isEnded)
        {
            if (buffer.size() > (i+1))
            {
                _recvBuffer.clear();
                _recvBuffer.append(buffer.constData() + (i+1), buffer.size() - (i+1));
            }

            if (packet.getSize() > 0)
            {
                lastErr = E_NO_ERROR;
                return true;
            }
        }

        // '\n' 또는 '\r' 의 입력이 없으면서 계속 다른 문자가 들어올 경우, 무한 루프에 빠지는 것을 방지하기 위해 넣은 코드
        if (packet.getSize() > MAX_PACKET_SIZE)
        {
            _recvBuffer.clear();
            lastErr = E_BUFFER_OVERRUN;
            return false;
        }
    }

    // not reach
    _recvBuffer.clear();
    lastErr = E_UNKNOWN;
    return false;
}
#endif

bool SerialPort::recvPacket(int msecs, Packet& packet, bool autoAck)
{
    QMutexLocker locker(&_mutex);

    packet.reset();

    QByteArray buffer;
    int i;
    bool firstFlag = true;
    bool isEnded;

    while (1)
    {
        buffer.clear();
        isEnded = false;

        if (_recvBuffer.size() > 0)
        {
            buffer = _recvBuffer;
            _recvBuffer.clear();
        }
        else
        {
            if (waitForRecv(msecs) == false)
            {
                // lastErr was set by waitForRecv()
                return false;
            }

            buffer = this->readAll();

            if (buffer.size() == 0)
            {
                // maybe, lastErr was set by readAll()
                return false;
            }
        }

        if (firstFlag)
        {
            // 전송받은 데이터의 첫 캐릭터 검사 (3가지 중에 하나여야 함: FS, ACK, NAK)
            switch (buffer.at(0))
            {
                case RC_START_MSG: // FS
                    packet.setPacketType(Packet::PT_DATA);
                    break;

                case RC_ACK: // ACK
                    packet.setPacketType(Packet::PT_ACK);
                    isEnded = true;
                    break;

                case RC_NAK: // NAK
                    packet.setPacketType(Packet::PT_NAK);
                    isEnded = true;
                    break;

                default:
                    lastErr = E_WRONG_FORMAT;
                    return false;
            }

            packet.append(buffer.at(0));

            firstFlag = false;
            i = 1;
        }
        else
        {
            i = 0;
        }

        for (; i < buffer.size() && isEnded == false; i++)
        {
            switch (buffer.at(i))
            {
                case RC_END_MSG: // GS
                    isEnded = true;
                    break;

                case RC_START_MSG: // FS
                case RC_ACK: // ACK
                case RC_NAK: // NAK
                    packet.setPacketType(Packet::PT_UNKNOWN);
                    lastErr = E_WRONG_FORMAT;
                    return false;

                case RC_CRC_SEP: // RS
                    // CRC separator 위치 표시
                    packet.markCrcPostion();
                    break;

                default:
                    ;
            }

            packet.append(buffer.at(i));

            if (isEnded)
                break;
        }

        if (isEnded)
        {
            if (buffer.size() > (i+1))
            {
                _recvBuffer.clear();
                _recvBuffer.append(buffer.constData() + (i+1), buffer.size() - (i+1));
            }

            if (packet.getSize() > 0)
            {
                break;
            }
        }

        // RC_END_MSG 의 입력이 없으면서 계속 다른 문자가 들어올 경우, 무한 루프에 빠지는 것을 방지하기 위해 넣은 코드
        if (packet.getSize() > MAX_PACKET_SIZE)
        {
            _recvBuffer.clear();
            lastErr = E_BUFFER_OVERRUN;
            return false;
        }
    }

    if (packet.getPacketType() == Packet::PT_DATA)
    {
        Parser::verifyPacketCrc(packet);

        if (autoAck)
        {
            switch (packet.getCrcStatus())
            {
                case Packet::CS_NONE:
                case Packet::CS_OK:
                    // ACK
                    lastErr = E_NO_ERROR;
                    locker.unlock();
                    sendAck();
                    return true;

                case Packet::CS_ERROR:
                default:
                    // NAK
                    _recvBuffer.clear();
                    lastErr = E_WRONG_CRC;
                    locker.unlock();
                    sendNak();
                    return false;
            }
        }
        else
        {
            if (packet.getCrcStatus() == Packet::CS_ERROR)
            {
                _recvBuffer.clear();
                lastErr = E_WRONG_CRC;
                return false;
            }
        }
    }

    lastErr = E_NO_ERROR;
    return true;
}

bool SerialPort::recvAck(int msecs)
{
    QMutexLocker locker(&_mutex);

    char oneBuffer;

    if (waitForRecv(msecs) == false)
    {
        // lastErr was set by waitForRecv()
        return false;
    }

    if (this->read(&oneBuffer, 1) != 1)
    {
        // lastErr was set by read() - readData()
        return false;
    }

    if (oneBuffer == RC_NAK)
    {
        lastErr = E_NEGATIVE_ACKNOWLEDGE;
        return false;
    }
    else if (oneBuffer != RC_ACK)
    {
        lastErr = E_WRONG_ACKNOWLEDGE;
        return false;
    }

    lastErr = E_NO_ERROR;
    return true;
}

bool SerialPort::sendPacketNoWait(const Packet& packet)
{
    QMutexLocker locker(&_mutex);

    if (!isOpen() || !isWritable())
    {
        lastErr = E_IO_ERROR;
        return false;
    }

    char* pData = (char*) packet.getData();
    qint64 dataSize = packet.getSize();

    qint64 bytes;

    while (pData && dataSize > 0)
    {
        bytes = this->write(pData, dataSize);

        if (bytes == -1)
        {
            // lastErr was set by write() - writeData()
            return false;
        }

        pData += bytes;
        dataSize -= bytes;
    }

    lastErr = E_NO_ERROR;
    return true;
}

bool SerialPort::sendPacket(const Packet& packet, int msecs)
{
    QMutexLocker locker(&_mutex);

    if (sendPacketNoWait(packet) == false)
    {
        // lastErr was set by sendPacket()
        return false;
    }

    return recvAck(msecs);
}

bool SerialPort::sendAck(bool ackFlag)
{
    QMutexLocker locker(&_mutex);

    if (!isOpen() || !isWritable())
    {
        lastErr = E_IO_ERROR;
        return false;
    }

    char oneBuffer;

    if (ackFlag)
        oneBuffer = RC_ACK;
    else
        oneBuffer = RC_NAK;

    qint64 bytes = 0;

    while (bytes == 0)
    {
        bytes = this->write(&oneBuffer, 1);

        if (bytes == -1)
        {
            // lastErr was set by write()
            return false;
        }
    }

    lastErr = E_NO_ERROR;
    return true;
}

void SerialPort::resetRecvBuffer()
{
    QMutexLocker locker(&_mutex);

    _recvBuffer.clear();
}
