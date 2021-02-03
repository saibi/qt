import QtQuick 2.9
import QtQuick.Window 2.3

Window {
    width: 300
    height: 300
    visible: true

    Rectangle {
        id: rect
        width: 50
        height: 50
        color: "red"

        Behavior on x { SpringAnimation { spring: 2; damping: 0.1 } }
        Behavior on y { SpringAnimation { spring: 2; damping: 0.1 } }

    }

    MouseArea {

        anchors.fill: parent
        onClicked: {
            rect.x = mouse.x - rect.width / 2
            rect.y = mouse.y - rect.height / 2
        }
    }
}
