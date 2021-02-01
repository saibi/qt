import QtQuick 2.9
import QtQuick.Window 2.3

Window {
    visible: true
    id: root
    width: 140; height: 400; color: "#D8D8D8"

    Image {
        id: rocket
        x: (parent.width - width)/2; y: 40
        source: 'images/rocket.png'
    }

    Text {
        y: rocket.y + rocket.height + 20
        width: root.width

        horizontalAlignment: Text.AlignHCenter
        text: "Rocket"
    }
}

