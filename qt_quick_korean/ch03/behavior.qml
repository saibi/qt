import QtQuick 2.9
import QtQuick.Window 2.3

Window {
    width: 330
    height: 330
    visible: true

    Rectangle {
        id: rect
        width: 330
        height: 330
        color: "red"

        Behavior on width {
            NumberAnimation { duration: 1000 }

        }

        MouseArea {
            anchors.fill: parent
            onClicked: {

                if ( rect.width > 150 )
                    rect.width = 20
                else
                    rect.width = 330
            }
        }
    }
}
