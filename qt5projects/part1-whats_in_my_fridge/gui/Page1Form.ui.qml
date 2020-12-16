import QtQuick 2.4
import QtQuick.Controls 2.3

Item {
    id: item1
    width: 400
    height: 400

    ListView {
        id: listView
        anchors.fill: parent

        delegate: ItemDelegate {
            width: parent.width
            text: modelData.name || model.name
            font.bold: true
            Button {
                width: height
                height: parent.height
                text: "X"
                anchors.right: parent.right
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
            id: textField
            text: qsTr("enter item name")
        }

        Button {
            id: button
            text: qsTr("Add item")
            focusPolicy: Qt.NoFocus
        }
    }
}
