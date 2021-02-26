#include "broadcasterbt.h"
#include <QDebug>

#include <QtBluetooth/QBluetoothLocalDevice>

namespace channels {
static const QLatin1String serviceUuid("e8e10f95-1a70-4b27-9ccf-02010264e9c8");
}

bool channels::BroadcasterBt::init()
{
	qDebug() << Q_FUNC_INFO;

	if ( !_listenToAdapter())
		return false;

	if ( !_provideServiceId() )
		return false;

	if ( !_provideServiceTraits() )
		return false;

	if ( !_provideServiceDiscoverability() )
		return false;

	if ( !_provideProtocolDescriptorList())
		return false;

	if ( !_registerService() )
		return false;

	return true;
}

void channels::BroadcasterBt::sendReadings(QList<QVariantMap> readings)
{
	qDebug() << Q_FUNC_INFO;
}

bool channels::BroadcasterBt::_listenToAdapter()
{
	qDebug() << Q_FUNC_INFO;
	_localAdapter = QBluetoothLocalDevice::allDevices().value(0).address();

	QBluetoothLocalDevice localDevice(_localAdapter);
	localDevice.setHostMode(QBluetoothLocalDevice::HostDiscoverable);

	_server = new QBluetoothServer(QBluetoothServiceInfo::RfcommProtocol, this);
	bool success = _server->listen(_localAdapter);

	qDebug() << (success ? "listening: " : "not listening: ") << localDevice.name();

	return success;
}

bool channels::BroadcasterBt::_provideServiceId()
{
	qDebug() << Q_FUNC_INFO;
	QBluetoothServiceInfo::Sequence classId;

	classId << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::SerialPort));

	_serviceInfo.setAttribute(QBluetoothServiceInfo::BluetoothProfileDescriptorList, classId);

	classId.prepend(QVariant::fromValue(QBluetoothUuid(serviceUuid)));
	_serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceClassIds, classId);
	_serviceInfo.setServiceUuid(QBluetoothUuid(serviceUuid));

	return _serviceInfo.isValid();
}

bool channels::BroadcasterBt::_provideServiceTraits()
{
	qDebug() << Q_FUNC_INFO;
	_serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceName, "CM BT Broadcaster Channel");
	_serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceDescription, "Cute Measures bluetooth broadcaster channel for sensor readings");
	_serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceProvider, "Cute Measures");
	return _serviceInfo.isValid();
}

bool channels::BroadcasterBt::_provideServiceDiscoverability()
{
	qDebug() << Q_FUNC_INFO;
	QBluetoothServiceInfo::Sequence publicBrowse;
	publicBrowse << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::PublicBrowseGroup));
	_serviceInfo.setAttribute(QBluetoothServiceInfo::BrowseGroupList, publicBrowse);

	return _serviceInfo.isValid();
}

bool channels::BroadcasterBt::_provideProtocolDescriptorList()
{
	qDebug() << Q_FUNC_INFO;
	QBluetoothServiceInfo::Sequence protocolDescriptorList;
	QBluetoothServiceInfo::Sequence protocol;
	protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::L2cap));
	protocolDescriptorList.append(QVariant::fromValue(protocol));
	protocol.clear();
	protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::Rfcomm)) << QVariant::fromValue(quint8(_server->serverPort()));
	protocolDescriptorList.append(QVariant::fromValue(protocol));
	_serviceInfo.setAttribute(QBluetoothServiceInfo::ProtocolDescriptorList, protocolDescriptorList);
	return _serviceInfo.isComplete();
}


bool channels::BroadcasterBt::_registerService()
{
	qDebug() << Q_FUNC_INFO;
	bool success = _serviceInfo.registerService(_localAdapter);
	if ( success)
		qDebug() << "registered service" << _serviceInfo.serviceName() << "on adapter" << _localAdapter.toString();

	return success;
}


