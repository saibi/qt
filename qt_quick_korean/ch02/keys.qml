import QtQuick 2.9
import QtQuick.Window 2.3

Window {
    visible: true
    width: 400
    height: 400

    Rectangle {
        width: 240
        height: 200
        color: "white"

        Image {
            id: logo
            x: 20
            y: 20
            source: "images/qtlogo.png"
            transformOrigin: Item.Center

            Behavior on rotation {
                NumberAnimation {
                    duration: 250
                }
            }
        }

        Keys.onPressed: {
            if (event.key === Qt.Key_Left) {
                logo.rotation = (logo.rotation - 10 ) % 360
            } else if ( event.key === Qt.Key_Right ) {
                logo.rotation = (logo.rotation + 10 ) % 360
            }
        }

        focus: true
    }
}
