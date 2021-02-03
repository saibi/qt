import QtQuick 2.9
import QtQuick.Window 2.3
import QtQuick.Layouts 1.3

Window {
    width: 200
    height: 200
    visible: true
    id: root

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

                font.pixelSize: 20
                anchors.left: parent.left
                anchors.leftMargin: 2
            }
            Text {
                text: '$' + cost
                anchors.left: nameField.right
                anchors.leftMargin: 10
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

    Component {
        id: bannercomponent
        Rectangle {
            id: banner
            width: root.width
            height: 50

            gradient: clubcolors
            border {
                color: "#9EDDF2"
                width: 2
            }

            RowLayout {
                anchors.fill: parent
                Text {

                    text: "Name"
                    font.pixelSize: 20
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter

                }
                Text {
                    text: "Cost"
                    font.pixelSize: 20
                    font.bold: true
                    horizontalAlignment: Text.AlignRight

                }

            }

        }
    }

    Gradient {
        id: clubcolors
        GradientStop { position: 0.0; color: "#8EE2FE" }
        GradientStop { position: 0.6; color: "#7ED2EE" }
    }

    ListView {
        anchors.fill: parent
        model: fruitModel
        delegate: fruitDelegate
        header: bannercomponent
        footer: Rectangle {
            width: parent.width

            height: 30
            gradient: clubcolors
        }
        highlight: Rectangle {
            color: "lightgray"
        }

        focus: true
    }
}
