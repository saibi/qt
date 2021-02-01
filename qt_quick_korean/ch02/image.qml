import QtQuick 2.9
import QtQuick.Window 2.3

Window {
    visible: true
    width: 500; height: 500

    Rectangle {
        width: 500; height: 500

        color: "white"

        Image {
            x:20; y: 30
            source: "images/qtlogo.png"
        }
    }
}
