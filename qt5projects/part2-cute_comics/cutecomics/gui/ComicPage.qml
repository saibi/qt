import QtQuick 2.9
import QtQuick.Layouts 1.3

Item {
    id: page

    property alias panelsRepeater: panelsRepeater
    property alias panelAdder: panelAdder
    property alias panelRemover: panelRemover

    width: 480
    height: width

    Rectangle {
        anchors.fill: parent
    }

    Item {
        id: drawingSurface
        anchors.fill: parent
        anchors.margins: 32

        GridLayout {
            id: panelsGrid
            anchors.fill: parent
            rows: 1
            columns: panelsRepeater.count
            Repeater {
                id: panelsRepeater
                model: 0
            }
        }
    }

    PanelButton {
        id: panelAdder
        anchors.right: parent.right
        text: "+"
    }

    PanelButton {
        id: panelRemover
        anchors.top: panelAdder.bottom
        anchors.right: parent.right
        text: "-"
    }


/* PanelButton

    Item {
        id: panelAdder

        width: 40
        height: width

        anchors.right: parent.right
        opacity: 0.5

        Text {
            text: "+"
            anchors.centerIn: parent
            font.pixelSize: 40
        }

        MouseArea {
            anchors.fill: parent

        }
    }
*/
}
