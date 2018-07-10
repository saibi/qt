//import QtQuick 2.6
//import QtQuick.Window 2.2

//Window {
//    visible: true
//    width: 640
//    height: 480
//    title: qsTr("Hello World")
//}

import QtQuick 2.0
import QtQuick.Window 2.2

Window {
    visible: true
    width: 360
    height: 360
    Text {
        anchors.centerIn: parent
        text: "Hello World !" + " My size is " + parent.width + " x " + parent.height + " !"
    }
    onHeightChanged: print("new size: ", width, "x", height)
    MouseArea {
        anchors.fill: parent
        onClicked: {
            console.log("I was \"" + parent + "\"!")
            console.log("Bye for now!")
            Qt.quit();
        }
    }
}
