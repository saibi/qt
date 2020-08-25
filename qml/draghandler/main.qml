import QtQuick 2.12
import QtQuick.Window 2.10

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Rectangle {
        width: 480
        height: 480
        color: "#22222222"

        Repeater {
            model: 2

            Image {
                id: ball
                source: "resources/pngbarn.png"
                width: 180
                height: 180
                x : 80 + index * 240
                y : 240

                DragHandler {
                    onActiveChanged: if ( !active ) anim.restart(point.velocity)

                }

                NumberAnimation {
                    id: anim
                    target: ball
                }
            }
        }
    }
}
