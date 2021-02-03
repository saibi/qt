import QtQuick 2.9
import QtQuick.Window 2.3
import QtQuick.XmlListModel 2.0

Window {
    width: 400
    height: 200

    visible: true

    color: "#404040"


    XmlListModel {
        id: xmlModel
        source: "item.xml"
        query: "//item"
        XmlRole { name: "title"; query: "string()" }
        XmlRole { name: "link"; query: "@link/string()" }
    }

    Component {
        id: xmlDelegate
        Item {
            width: parent.width
            height: 64

            Rectangle {
                width: Math.max(childrenRect.width + 16, parent.width )
                height: 60
                clip: true

                color: "#505060"
                border.color: "#8080b0"
                radius: 8

                Column {
                    Text { x: 6; color: "white"; font.pixelSize: 32; text: title }
                    Text { x: 6; color: "white"; font.pixelSize: 16; text: link }

                }
            }
        }
    }

    ListView {
        anchors.fill: parent
        anchors.margins: 4
        model: xmlModel
        delegate: xmlDelegate
    }
}
