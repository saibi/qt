import QtQuick 2.9
import QtQuick.Window 2.3
import QtGraphicalEffects 1.0

Window {
    visible: true
    width: 300
    height: 300

    Item {
        anchors.fill: parent

        Image {
            id: lenna
            source: "images/lenna.png"

            sourceSize: Qt.size(parent.width, parent.height)
            smooth: true
            visible: false
        }
        Rectangle {
            id: displacement
            color: Qt.rgba(0.5, 0.5, 1.0, 1.0)
            anchors.fill: parent
            visible: false
            Image {
                anchors.centerIn: parent
                source: "images/glass_normal.png"
                sourceSize: Qt.size(parent.width/2, parent.height/2)
                smooth: true
            }
        }

        Displace {
            anchors.fill: lenna
            source: lenna
            displacementSource: displacement
            displacement: 0.9
        }
    }

}
