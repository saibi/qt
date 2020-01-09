#ifndef ENTITY_H
#define ENTITY_H

#include <map>

#include <QObject>
#include <QScopedPointer>

#include <cm-lib_global.h>
#include <data/data-decorator.h>
#include <data/entity-collection.h>
#include <data/string-decorator.h>

namespace cm {
namespace data {


class CMLIBSHARED_EXPORT Entity : public QObject
{
	Q_OBJECT
public:
	Entity(QObject *parent = nullptr, const QString& key = "SomeEntityKey");
	Entity(QObject *parent, const QString &key, const QJsonObject& jsonObject);
	virtual ~Entity();

public:
	const QString& key() const;
	void update(const QJsonObject& jsonObject);
	QJsonObject toJson() const;
	const QString& id() const;

signals:
	void childEntitiesChanged();
	void dataDecoratorsChanged();
	void childCollectionsChanged(const QString& collectionKey);

protected:
	Entity* addChild(Entity* entity, const QString& key);
	DataDecorator* addDataItem(DataDecorator* dataDecorator);
	EntityCollectionBase* addChildCollection(EntityCollectionBase* entityCollection);
	void setPrimaryKey(StringDecorator *primaryKey);

protected:
	class Implementation;
	QScopedPointer<Implementation> implementation;

};

}}

#endif // ENTITY_H
