#ifndef BROADCASTERBT_H
#define BROADCASTERBT_H

#include "broadcasterchannel.h"

#include <QtBluetooth/QBluetoothServer>
#include <QtBluetooth/QBluetoothAddress>
#include <QtBluetooth/QBluetoothServiceInfo>

namespace channels {

class BroadcasterBt : public BroadcasterChannel
{
	Q_OBJECT

public:
	explicit BroadcasterBt(QObject * parent = nullptr);
	bool init() override;

public slots:
	void sendReadings(QList<QVariantMap> readings);

private:
	bool _listenToAdapter();
	bool _provideProtocolDescriptorList();
	bool _provideServiceId();
	bool _provideServiceDiscoverability();
	bool _provideServiceTraits();
	bool _registerService();

	QBluetoothServer *_server;
	QBluetoothAddress _localAdapter;
	QBluetoothServiceInfo _serviceInfo;

};

}

#endif // BROADCASTERBT_H
