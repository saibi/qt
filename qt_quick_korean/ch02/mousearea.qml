import QtQuick 2.9
import QtQuick.Window 2.3

Window {
    visible: true
    width: 300
    height: 300

    Rectangle {
        width: parent.width
        height: parent.height
        color: "lightblue"

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            text: "Press me"; font.pixelSize: 48

            MouseArea {
                anchors.fill: parent
                onPressed: {
                    parent.color = "green"
                    console.log("Press")
                }
                onReleased: {
                    parent.color = "black"
                    console.log("Release")
                }
            }
        }
    }

}
