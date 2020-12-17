#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "../entities/grocery_items.h"
#include "../repositories/grocery_items_repo_dummy.h"
#include "../usecases/check_available_groceries/check_available_groceries.h"
#include "../usecases/add_grocery_item/add_grocery_item.h"
#include "../usecases/remove_grocery_item/remove_grocery_item.h"

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

	QGuiApplication app(argc, argv);

	auto groceryItems = new entities::GroceryItems(&app);
	auto groceryItemRepoDummy = new repositories::GroceryItemsRepoDummy(&app);
	groceryItems->setRepository(groceryItemRepoDummy);
	auto checkAvailableGroceries = new usecases::CheckAvailableGroceries(groceryItems, &app);
	auto addGroceryItem = new usecases::AddGroceryItem(groceryItems, &app);
	auto removeGroceryItem = new usecases::RemoveGroceryItem(groceryItems, &app);

	QQmlApplicationEngine engine;

	engine.rootContext()->setContextProperty("groceryItems", groceryItems);
	engine.rootContext()->setContextProperty("checkAvailableGroceries", checkAvailableGroceries);
	engine.rootContext()->setContextProperty("addGroceryItem", addGroceryItem);
	engine.rootContext()->setContextProperty("removeGroceryItem", removeGroceryItem);

	engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
	if (engine.rootObjects().isEmpty())
		return -1;

	return app.exec();
}
