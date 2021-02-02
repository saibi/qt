import QtQuick 2.9
import QtQuick.Window 2.3

Window {
    width: 630
    height: 430
    visible: true
    Image {
        source: "images/qtlogo.png"
        x: 10
        y: 20

        NumberAnimation on x {
            from: 10
            to: 350
            duration: 2000
        }
    }
}
