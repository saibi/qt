import QtQuick 2.9
import QtQuick.Window 2.3


Window {
    visible: true; width: 400; height: 100
    Row {
        Repeater {
            model: 3
            anchors.top: parent.top
            Rectangle {
                width: 100; height: 40
                border.width: 1
                color: "yellow"
            }
        }
    }

}
