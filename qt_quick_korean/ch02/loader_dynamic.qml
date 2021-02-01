import QtQuick 2.9
import QtQuick.Window 2.3

Window {
    visible: true;

    width: 400
    height: 400

    Rectangle {
        id: root
        width: 400
        height: 400

        Loader {
            id: myLoader
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: menu1Button.top
            onLoaded: { source: "menu1.qml" }
        }

        Rectangle {
            id: menu1Button

            anchors.left: parent.left

            anchors.bottom: parent.bottom

            color: "gray"
            width: parent.width /2
            height: 100

            Text {
                anchors.centerIn: parent
                text: "first menu"
                font.bold: true
                font.pixelSize: 20
                color: "white"

            }
            MouseArea {
                anchors.fill: parent
                onClicked: root.state = "menu1"
            }
        }

        Rectangle {
            id: menu2Button

            anchors.right: parent.right

            anchors.bottom: parent.bottom

            color: "gray"
            width: parent.width /2
            height: 100

            Text {
                anchors.centerIn: parent
                text: "second menu"
                font.bold: true
                font.pixelSize: 20
                color: "white"

            }
            MouseArea {
                anchors.fill: parent
                onClicked: root.state = "menu2"
            }
        }

        state: "menu1"
        states: [
            State {
                name: "menu1"
                PropertyChanges { target:menu1Button; color: "green"; }
                PropertyChanges { target: myLoader; source: "menu1.qml"; }

            },
            State {
                name: "menu2"
                PropertyChanges { target:menu2Button; color: "green"; }
                PropertyChanges { target: myLoader; source: "menu2.qml"; }

            }
        ]

    }





}
