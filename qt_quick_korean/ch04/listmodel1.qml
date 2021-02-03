import QtQuick 2.9
import QtQuick.Window 2.3

Window {
    width: 200
    height: 200
    visible: true

    ListModel {
        id: fruitModel
        ListElement {
            name: "Apple"
            cost: 2.45
            attributes: [
                ListElement { description: "Delicious" },
                ListElement { description: "Expensive" },
                ListElement { description: "hello" }
            ]
        }
        ListElement {
            name: "Orange"
            cost: 3.25
            attributes: [
                ListElement { description: "Expensive" }
            ]
        }
        ListElement {
            name: "Banana"
            cost: 1.95
            attributes: [
                ListElement { description: "Delicious" },
                ListElement { description: "Expensive" }
            ]
        }

    }

    Component {
        id: fruitDelegate

        Item {
            width: 200
            height: 50
            Text {
                id: nameField
                text: name
            }
            Text {
                text: '$' + cost
                anchors.left: nameField.right
            }

            Row {
                anchors.top: nameField.bottom
                spacing: 5

                Text { text: "Attributes:" }
                Repeater {
                    model: attributes
                    Text { text: description }
                }

            }

        }

    }

    ListView {
        anchors.fill: parent
        model: fruitModel
        delegate: fruitDelegate
    }
}
