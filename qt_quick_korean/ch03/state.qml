import QtQuick 2.9
import QtQuick.Window 2.3

Window {
    width: 150
    height: 250
    visible: true

    Rectangle {
        id: rect
        width: parent.width

        height: parent.height

        Rectangle {
            id: onElement
            x: 25
            y: 15
            width: 100
            height: 100

        }

        Rectangle {
            id: offElement
            x: 25
            y: 135
            width: 100
            height: 100
        }

        states: [
            State {
                name: "on"
                PropertyChanges { target: onElement; color: "red" }
                PropertyChanges { target: offElement; color: "black" }
            },

            State {
                name: "off"
                PropertyChanges { target: onElement; color: "black" }
                PropertyChanges { target: offElement; color: "green" }

            }

        ]

        MouseArea {
            anchors.fill: parent
            onClicked: parent.state == "on" ? parent.state = "off" : parent.state = "on"
        }

        state: "on"
    }
}
