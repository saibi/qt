import QtQuick 2.9
import QtQuick.Window 2.3

Window {

    visible: true
    width: Screen.width /4
    height: Screen.height / 4

    Flickable {
        width: Screen.width /4
        height: Screen.height /4

        contentWidth: Screen.width
        contentHeight: Screen.height * 2
        interactive: true

        Image {
            id: ground
            anchors.fill: parent
            source: "images/ground.jpg"
            sourceSize.width: Screen.width
            sourceSize.height: Screen.height *2
        }

        Image {
            id: player
            source: "images/player.png"
            x: Screen.width / 8
            y: Screen.height /8

        }
    }
}
