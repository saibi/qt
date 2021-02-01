import QtQuick 2.9
import QtQuick.Window 2.3

Window {
    visible: true
    width: 400
    height: 400

    Loader {
        id: pageLoader
        anchors.top: myRect.bottom
    }

    Rectangle {
        id: myRect
        width: 200; height: 100
        color: "yellow"

        Text {
            anchors.centerIn: parent
            text: "Main qml"
            font.bold: true
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked:
        {
            pageLoader.source = "page1.qml"
        }
    }

}
