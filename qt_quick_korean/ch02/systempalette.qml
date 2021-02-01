import QtQuick 2.9
import QtQuick.Window 2.3

Window {
    visible: true; width: 640; height: 480
    Rectangle {
        width: 640; height: 480
        color: myPalette.window
        SystemPalette {
            id: myPalette
            colorGroup: SystemPalette.Active
        }
        Text {
            id: myText
            anchors.fill: parent
            text: "Hello!";
            font.pixelSize: 32
            color: myPalette.windowText
        }
    }

}
