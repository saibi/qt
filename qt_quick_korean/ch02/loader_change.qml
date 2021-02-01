import QtQuick 2.9
import QtQuick.Window 2.3

Window {
    visible: true;

    width: 200
    height: 200

    Loader {
        id: squareLoader
        onLoaded: {
            console.log("Width : " + squareLoader.item.width);
        }
    }

    Component.onCompleted: {
        squareLoader.setSource("excomponent.qml", { "color": "blue"} );

    }

    Rectangle {
        anchors.top: squareLoader.bottom
        width: 200
        height: 200
        color: "green"

        MouseArea {
            anchors.fill: parent

            onClicked: {
                squareLoader.setSource("excomponent.qml", {"width": 200} )
            }
        }
    }
}

