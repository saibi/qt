import QtQuick 2.9
import QtQuick.Window 2.3

Window {
    visible: true
    title: qsTr("Hello")

    width: 200
    height: 200

    MouseArea {
        anchors.fill: parent
        onClicked: label.moveTo(mouse.x, mouse.y)
    }

    Text {
        id: label

        function moveTo(newX, newY) {
            label.x = newX;
            label.y = newY;
        }

        text: "Move me!"
    }
}
