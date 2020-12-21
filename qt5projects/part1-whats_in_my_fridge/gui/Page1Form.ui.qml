import QtQuick 2.7
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

Item {
    id: item1
    width: 400
    height: 400
    property alias groceriesListView: groceriesListView
    property alias addItemField: addItemField
    property alias addItemButton: addItemButton

    ListView {
        id: groceriesListView

        signal itemRemoved(string itemName)

        anchors.fill: parent

        delegate: ItemDelegate {
            width: parent.width
            text: modelData.name || model.name
            font.bold: true
            Button {
                id: removeItemButton
                width: height
                height: parent.height
                text: "X"
                anchors.right: parent.right
                Connections {
                    target: removeItemButton
                    onClicked: groceriesListView.itemRemoved(modelData.name
                                                             || model.name)
                }
            }
        }

        model: ListModel {
            ListElement {
                name: "Bananas"
            }

            ListElement {
                name: "Orange Juice"
            }

            ListElement {
                name: "Grapes"
            }

            ListElement {
                name: "Eggs"
            }
        }

        /*
        onItemRemoved: {
            // design mode message : JavaScript blocks are not supported in a Qt Quick UI form. (M223) (Line: 56)
            console.log("remove item", itemName)
            removeGroceryItem.run(itemName)
        }
        */
    }

    Row {
        id: row
        y: 301
        height: 64
        spacing: 8
        anchors.leftMargin: 8
        anchors.rightMargin: 8
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        TextField {
            id: addItemField
            text: qsTr("enter item name")
        }

        Button {
            id: addItemButton
            text: qsTr("Add item")
            focusPolicy: Qt.NoFocus
        }
    }
}
