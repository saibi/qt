import QtQuick 2.9
import QtQuick.Window 2.3

Window {
    width: 150
    height: 250
    visible: true

    Rectangle {

        anchors.fill:parent

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
        state: "on"

        MouseArea {
            anchors.fill: parent
            onClicked: parent.state == "on" ? parent.state = "off" : parent.state = "on"
        }

        transitions: [
            Transition {
                from: "off"
                to: "on"
                PropertyAnimation {
                    target: onElement
                    properties: "color"
                    duration: 1000
                }

            },
            Transition {
                from: "on"
                to: "off"
                PropertyAnimation {
                    target: offElement
                    properties: "color"
                    duration: 1000
                }

            }

        ]
    }
}
