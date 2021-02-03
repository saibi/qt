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

    SequentialAnimation {
        NumberAnimation {
            target: logo
            property: "scale"

            from: 0.1
            to: 1.0

            duration: 1000

            easing.type: Easing.InOutBack
        }
        PauseAnimation {
            duration: 1000
        }
        NumberAnimation {
            target: logo
            property: "scale"

            from: 1.0
            to: 0.0

            duration: 1000

            easing.type: Easing.InOutCubic
        }

        running: true

    }


}
