import QtQuick 2.9
import QtQuick.Window 2.3

Window {
    visible: true
    width: 400
    height: 112

    Rectangle {
        width: 400
        height: 112
        color: "lightblue"

        TextInput {
            anchors.left: parent.left
            anchors.right: parent.right
            y: 16
            text: "Embedded Programming"
            font.pixelSize: 32
            color: focus ? "black" : "gray"
            focus: true

        }

        TextInput {
            anchors.left: parent.left
            anchors.right: parent.right
            y: 64
            text: "Thread programming"
            font.pixelSize: 32
            color: focus ? "black" : "gray"

        }
    }
}
