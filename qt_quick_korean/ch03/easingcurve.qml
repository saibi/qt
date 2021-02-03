import QtQuick 2.9
import QtQuick.Window 2.3

Window {
    width: 300
    height: 300
    visible: true

    Image {
        id: logo
        source: "images/qtlogo.png"
        anchors.centerIn: parent
    }

    NumberAnimation {
        target: logo
        property: "scale"

        from: 0.1
        to: 1.0

        duration: 2000

        easing.type: "OutExpo"
        running: true
    }

}
