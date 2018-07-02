#include "packet.h"

Packet::Packet()
{

}

Packet::Packet(const QString &text)
{
	setText(text);
}

void Packet::setText(const QString &text)
{
	m_text = text.trimmed();
	convertBin();
}


void Packet::convertBin()
{
	if ( m_text.isEmpty() )
		return;

	m_data = m_text.toLocal8Bit();

	m_data.replace("\n", "");
	m_data.replace("<FS>", QByteArray(1, RC_FS) );
	m_data.replace("<GS>", QByteArray(1, RC_GS) );
	m_data.replace("<RS>", QByteArray(1, RC_RS) );
	m_data.replace("<CR>", QByteArray(1, RC_CR) );
	m_data.replace("<LF>", QByteArray(1, RC_LF) );
	m_data.replace("<ACK>", QByteArray(1, RC_ACK) );
	m_data.replace("<NAK>", QByteArray(1, RC_NAK) );
	m_data.replace("<ESC>", QByteArray(1, RC_ESC) );
	m_data.replace("<SUB>", QByteArray(1, RC_SUB) );
	m_data.replace("<DC1>", QByteArray(1, RC_DC1) );
	m_data.replace("<DC3>", QByteArray(1, RC_DC3) );

	int idx;
	while ( (idx = m_data.indexOf("\\x")) >= 0 )
	{
		QString str = m_data.mid(idx + 2, 2);
		bool ok;
		char val = str.toInt(&ok, 16);
		m_data.remove(idx, 4);
		m_data.insert(idx, val);
	}
}

QString Packet::toFormatString(const QByteArray & data)
{
	QString formatStr;
	unsigned char c;

	for (int i = 0; i < data.size(); ++i)
	{
		c = (unsigned char) data.at(i);

		switch (c)
		{
		case RC_FS:		formatStr += "<FS>"; formatStr += '\n'; break;
		case RC_GS:		formatStr += "<GS>"; formatStr += '\n'; break;
		case RC_DC1:	formatStr += "<DC1>"; break;
		case RC_DC3:	formatStr += "<DC3>"; break;
		case RC_ACK:	formatStr += "<ACK>"; formatStr += '\n'; break;
		case RC_NAK:	formatStr += "<NAK>"; formatStr += '\n'; break;
		case RC_ESC:	formatStr += "<ESC>"; break;
		case RC_RS:		formatStr += "<RS>"; formatStr += '\n'; break;
		case RC_SUB:	formatStr += "<SUB>"; break;
		case RC_CR:		formatStr += "<CR>"; formatStr += '\n'; break;
		case RC_LF:		formatStr += "<LF>"; break;

		default:
		{
			if (isprint(c))
				formatStr += QChar(c);
			else
				formatStr += QString("\\x%1").arg(c, 2, 16, QChar('0'));
		}
		}
	}

	return formatStr;
}

QString Packet::getAsFormatString()
{
	return toFormatString(m_data);
}

bool Packet::calcCrc(bool autoErrorCorrection)
{
	m_crcVal = 0;
	m_crcStr.clear();

	int idx = m_data.indexOf(RC_FS);
	if ( idx < 0 )
	{
		qDebug("[Packet::calcCrc] <FS> not found");
		return false;
	}
	++idx;

	int rsIdx = m_data.indexOf(RC_RS);
	if ( rsIdx < 0 )
	{
		qDebug("[Packet::calcCrc] <RS> not found");
		return false;
	}

	if ( m_data.indexOf(RC_GS) < 0 )
	{
		qDebug("[Packet::calcCrc] <GS> not found");

		if ( !autoErrorCorrection )
			return false;
	}

	// calc crc value
	m_crcVal = crc16ccitt(m_data.constData() + idx, rsIdx - idx + 1 );

	qDebug("[Packet::calcCrc] crcval = %d", m_crcVal);


	// new
	int firstCrcTextPos = rsIdx + 1;
	QByteArray crcPart = m_data.mid(firstCrcTextPos);
	crcPart.replace(char(RC_CR), "");
	crcPart.replace(char(RC_LF), "");
	crcPart.replace(char(RC_GS), "");
	if ( autoErrorCorrection )
		crcPart.replace(char(0), ""); // remove invalid 0

	int sepIdx = crcPart.indexOf(RC_LABEL_SEP);
	if ( sepIdx < 0 )
	{
		qDebug("[Packet::calcCrc] no crc label separator");
		return false;
	}

	QString crcLabel = crcPart.left(sepIdx).trimmed().toUpper();
	if ( autoErrorCorrection )
	{
		crcLabel.replace(" ", ""); // remove invalid space
		crcLabel.replace("\t ", ""); // remove invalid tab
	}

	if ( crcLabel != "CRC" )
	{
		qDebug("[Packet::calcCrc] no crc label");
		return false;
	}

	m_crcStr = crcPart.mid(sepIdx + 1).trimmed();
	qDebug("[Packet::calcCrc] m_crcStr = [%s]", qPrintable(m_crcStr));

	return true;
}

int Packet::crc16ccitt(const char* pData, int size)
{
	quint16 crc16 = 0;

	if (pData && size > 0)
	{
		for (int i = 0; i < size; i++)
		{
			crc16 = (crc16 >> 8) | (crc16 << 8);
			crc16 ^= (quint8) pData[i];
			crc16 ^= (crc16 & 0xFF) >> 4;
			crc16 ^= (crc16 << 8) << 4;
			crc16 ^= ((crc16 & 0xFF) << 4) << 1;
		}
	}
	else
	{
		return -1;
	}

	return (int) crc16;
}
