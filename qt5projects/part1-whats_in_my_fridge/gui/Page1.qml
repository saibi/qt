import QtQuick 2.7

Page1Form {

    addItemButton.onClicked: {
        console.log("add item")
        addGroceryItem.run(addItemField.text)
    }

    //groceriesListView.onItemRemoved: console.log("item removed")

    Component.onCompleted: checkAvailableGroceries.run()
}
