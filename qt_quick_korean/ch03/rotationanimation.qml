import QtQuick 2.9
import QtQuick.Window 2.3

Window {
    width: 330
    height: 330
    visible: true
    Image {
        id: logo
        source: "images/qtlogo.png"
        anchors.centerIn: parent
        smooth: true

        RotationAnimation on rotation {
            from: 45
            to: 315

            direction: RotationAnimation.Shortest
            duration: 3000
        }
    }
}
