#include "broadcasterchannel.h"
#include "../../entities/entity_broadcaster/broadcaster.h"

#include <QDebug>

bool channels::BroadcasterChannel::connectToBroadcaster(entities::Broadcaster *broadcaster)
{
	qDebug() << Q_FUNC_INFO;

	return connect(broadcaster, &entities::Broadcaster::readingsPublished, this, &BroadcasterChannel::sendReadings);
}
