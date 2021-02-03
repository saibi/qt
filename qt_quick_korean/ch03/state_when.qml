import QtQuick 2.9
import QtQuick.Window 2.3

Window {
    width: 300
    height: 100
    visible: true

    Rectangle {

        anchors.fill:parent
        color: "#ccffcc"

        TextInput {
            id: textField
            text: "Hello world."
            font.pointSize: 24
            anchors.left: parent.left
            anchors.leftMargin: 4
            anchors.verticalCenter: parent.verticalCenter

        }

        Image {
            id: clearButton
            source: "images/clear.svg"
            anchors.right: parent.right
            anchors.rightMargin: 4
            anchors.verticalCenter: textField.verticalCenter

            MouseArea {
                anchors.fill: parent
                onClicked: textField.text = ""
            }
        }

        states: [
            State {
                name: "with text"
                when: textField.text != ""

                PropertyChanges { target: clearButton; opacity: 1.0 }
            },

            State {
                name: "without text"
                when: textField.text == ""

                PropertyChanges { target: clearButton; opacity: 0.25 }
                PropertyChanges { target: textField; focus: true }
            }
        ]
        state: "with text"
    }
}
