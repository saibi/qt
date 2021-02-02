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

        Image {
            id: butterfly
            source: "images/butterfly.png"
            sourceSize: Qt.size(parent.width, parent.height)
            smooth: true
            visible: false
        }

        Blend {
            anchors.fill: lenna
            source: lenna
            foregroundSource: butterfly
            mode: "average"
            cached: true
        }
    }

}
