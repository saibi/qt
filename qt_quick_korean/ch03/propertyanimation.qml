import QtQuick 2.9
import QtQuick.Window 2.3

Window {
    width: 330
    height: 330
    visible: true
    Image {
        id: pro
        source: "images/qtlogo.png"
        x: 50
        y: 40
        width: 50
        height: 50
    }

    PropertyAnimation {
        target: pro
        properties: "width, height"
        from: 0
        to: 250
        duration: 1000
        running: true
    }
}
