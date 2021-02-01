import QtQuick 2.9
import QtQuick.Window 2.3


Window {
    visible: true
    width: 200
    height: 200

    Loader {
        id: pageLoader

        anchors.top: myRect.bottom
        onStatusChanged: {
            if ( pageLoader.status == Loader.Null )
                console.log('null')
            else if ( pageLoader.status == Loader.Ready )
                console.log('ready')
            else if ( pageLoader.status == Loader.Loading )
                console.log('loading')
        }
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
