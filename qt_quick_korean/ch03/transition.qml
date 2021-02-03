import QtQuick 2.9
import QtQuick.Window 2.3

Window {
    width: 150
    height: 150
    visible: true

    Rectangle {

        width: 150
        height: 150
        color: "black"

        Image {
            id: player

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            source: "images/player.png"
        }

        states: [
            State {
                name: "up"
                PropertyChanges { target: player; rotation: 0 }
            },

            State {
                name: "down"
                PropertyChanges { target: player; rotation: 180 }
            }
        ]
        state: "up"

        transitions: [
            Transition {
                from: "*"
                to: "*"

                PropertyAnimation {
                    target: player
                    properties: "rotation"
                    duration: 1000
                }
            }

        ]


        MouseArea {
            anchors.fill: parent
            onClicked: parent.state == "up" ? parent.state = "down" : parent.state = "up"
        }

    }
}
