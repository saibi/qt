import QtQuick 2.9
import QtQuick.Window 2.2

Window {
    visible: true
    width: 80
    height: 80
    title: qsTr("Window")


    // example code : start

    Rectangle {
        id: root
        width: parent.width
        height: parent.height
        color: "lightgrey"

        Column {
            id: clockText
            anchors.centerIn: root
            spacing: 20

            Text {
                id: timeText
                anchors.horizontalCenter: parent.horizontalCenter
                text: "13:45"
            }
            Text {
                id: dateText
                anchors.horizontalCenter: parent.horizontalCenter
                text: "23.02.2012"
            }
        }

        MouseArea {
            anchors.fill: root
            onClicked: {
                Qt.quit();
            }
        }
    }

    // example code : end
}






// default code
//import QtQuick 2.9
//import QtQuick.Window 2.2

//Window {
//    visible: true
//    width: 640
//    height: 480
//    title: qsTr("Hello World")
//}
