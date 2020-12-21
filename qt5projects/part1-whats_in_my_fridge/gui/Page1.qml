import QtQuick 2.7

Page1Form {

    addItemButton.onClicked: {
        console.log("add item", addItemField.text)
        addGroceryItem.run(addItemField.text)
    }

    groceriesListView.model: groceryItems.list

    //groceriesListView.onItemRemoved: console.log("item removed")
    // run-time error
    //qrc:/main.qml:12 Type Page1 unavailable
    //qrc:/Page1.qml:10 Cannot assign to non-existent property "onItemRemoved"

    Connections {
        target: groceriesListView
        onItemRemoved: {
            console.log("item removed", itemName)
            removeGroceryItem.run(itemName)
        }
    }

    Connections {
        target: addGroceryItem
        onSuccess: addItemField.text = ""
    }

    Component.onCompleted: {
        checkAvailableGroceries.run()
    }

}
