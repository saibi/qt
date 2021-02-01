import QtQuick 2.9
import QtQuick.Window 2.3

Window {
    visible: true; width: 400; height: 400

    Rectangle {
        id: rect
        width: 400
        height: 400
        color: "blue"

        MouseArea {
            anchors.fill: parent
            onClicked: {
                rect.color = Qt.rgba(Math.random(), Math.random(), Math.random(), 1)
                console.log("Clicked mouse at", mouse.x, mouse.y)
            }
        }
    }
}
